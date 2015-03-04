/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "HttpTask.h"

#include "chronotext/utils/Utils.h"

#include "Poco/Buffer.h"
#include <Poco/Net/HTTPBasicCredentials.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/NetException.h>

using namespace std;
using namespace ci;
using namespace chr;
using namespace Poco;
using namespace Poco::Net;

atomic<bool> HttpTask::LOG_VERBOSE (false);
atomic<bool> HttpTask::SIMULATE_NETWORK_FAILURE (false); // XXX: PROBABLY TEMPORARY

namespace httptask
{
    bool sslInitialized = false;
}

HttpTask::HttpTask(const Request &request, Handler *handler)
:
request(request),
handler(handler),
state(STATE_UNDEFINED),
downloadedSize(0),
session(nullptr),
ssl(false)
{}

void HttpTask::run()
{
    state = STATE_STARTED;
    
    bool hasPendingHttpError = false;
    Error pendingHttpError;

    try
    {
        URI uri(request.uri);
        
        if (!request.destinationPath.empty())
        {
            response.filePath = request.destinationPath;
        }
        else if (!request.destinationFolder.empty())
        {
            string filename = extractFilename(uri);
        
            if (filename.empty())
            {
                throw Error(ERROR_INVALID_FILENAME_IN_URI);
            }
            else
            {
                response.filePath = request.destinationFolder / filename;
            }
        }
   
        // ---
        
        bool hasParams = false;
        HTMLForm form;
        
        if (!request.params.empty())
        {
            hasParams = true;
            
            for (auto entry : request.params)
            {
                form.add(entry.first, entry.second);
            }
        }
        
        // ---

        beginSession(uri);
        session->setTimeout(Timespan(request.timeoutDuration, 0));
        
        HTTPRequest req(request.method, uri.getPathAndQuery(), HTTPMessage::HTTP_1_1);
        
        if (!request.username.empty() && !request.password.empty())
        {
            HTTPBasicCredentials(request.username, request.password).authenticate(req);
        }

        for (auto entry : request.headers)
        {
            req.set(entry.first, entry.second);
        }
        
        if (hasParams)
        {
            form.prepareSubmit(req);
        }
        
        if (handler)
        {
            post([=]{ handler->requestStarted(request); });
        }
        
        /*
         * AFTER requestStarted CALLBACK
         * BEFORE HTTP REQUEST IS SENT
         */
        failIfRequired();

        /*
         * WARNING: WILL THROW A Poco::Net::HostNotFoundException WHEN THERE IS NO NETWORK
         */
        ostream &os = session->sendRequest(req);
        
        if (hasParams && (request.method == HTTPRequest::HTTP_POST))
        {
            form.write(os);
        }
        
        if (handler)
        {
            post([=]{ handler->requestSent(request); });
        }
        
        cancelIfRequired();
        failIfRequired();
        
        /*
         * WARNING: THIS PART CAN CRASH ON iOS!
         *
         * HOW TO REPRODUCE:
         * 1) RUN A PYTHON SimpleHTTPServer (IT IS CAPABLE OF SERVING FILES ONLY ONCE AT A TIME)
         * 2) DOWNLOAD SOME LARGE FILE FROM THE SERVER USING ANOTHER DEVICE
         * 3) TRY TO DOWNLOAD THE SAME FILE WITH YOUR iOS DEVICE
         * 4) NOTHING SHOULD BE DOWNLOADED AND AFTER THE TIME-OUT DELAY, YOUR iOS DEVICE SHOULD CRASH
         *
         * EXPECTED BEHAVIOR:
         * - WE SHOULD HAVE RECEIVED A Poco::Exception
         * - OR AT LEAST SOME EXCEPTION (UNFORTUNATELY, A CATCH-ALL IS NOT HELPING...)
         */
        HTTPResponse res;
        istream &is = session->receiveResponse(res);
        
        response.status = res.getStatus();
        response.contentType = res.getContentType();
        response.contentLength = res.getContentLength();
        
        /*
         * TODO:
         * 1) INCLUDE RESPONSE-HEADERS IN response
         * 2) POST A responseReceived CALLBACK (PRE-REQUISITE: UPGRADE THE CALLBACK NAMING SCHEME)
         */
        
        cancelIfRequired();
        failIfRequired();
        
        if (response.status == HTTPResponse::HTTPStatus::HTTP_FOUND)
        {
            throw Error(ERROR_UNSUPPORTED_REDIRECT);
        }
        else if ((response.status < 200) || (response.status >= 300))
        {
            Error error(ERROR_HTTP, res.getReason());
            
            if (request.readPayloadOnFailure && (request.method != HTTPRequest::HTTP_HEAD))
            {
                hasPendingHttpError = true;
                pendingHttpError = error;
            }
            else
            {
                throw error;
            }
        }

        // ---

        /*
         * NO EXCEPTION IS THROWN UPON TIME-OUT WHEN USING HTTPS
         * THIS IS THEREFORE THE ONLY WAY TO TACKLE THE ISSUE
         *
         * REGARDING HTTP CONNECTIONS: THE FOLLOWING SHOULD NEVER HAPPEN, BUT WHO KNOWS?
         */
        if (ssl && (response.contentLength == HTTPResponse::UNKNOWN_CONTENT_LENGTH))
        {
            throw Error(ERROR_INVALID_CONTENT_LENGTH);
        }
        
        if (request.sizeLimit)
        {
            if ((response.contentLength == HTTPResponse::UNKNOWN_CONTENT_LENGTH) || (response.contentLength > request.sizeLimit))
            {
                Error error((response.contentLength == HTTPResponse::UNKNOWN_CONTENT_LENGTH) ? ERROR_INVALID_CONTENT_LENGTH : ERROR_CONTENT_LIMIT_EXCEEDED);

                if (handler)
                {
                    post([=]{ handler->requestAborted(request, error); });
                }

                state = STATE_ABORTED;
                throw error;
            }
        }
        
        // ---
        
        if (request.method != HTTPRequest::HTTP_HEAD)
        {
            if (handler)
            {
                post([=]{ handler->requestProgress(request, 0, response.contentLength); });
            }
            
            if (hasPendingHttpError || response.filePath.empty())
            {
                if (response.contentLength != HTTPResponse::UNKNOWN_CONTENT_LENGTH)
                {
                    response.content.reserve(response.contentLength);
                }
                
                readToString(is, response.content, request.bufferSize);
            }
            else
            {
                fs::create_directories(response.filePath.parent_path());
                
                fs::ofstream os(response.filePath, ios::binary);
                readToFile(is, os, request.bufferSize);
            }
        }
        
        // ---
        
        const Poco::Exception *networkException = session->networkException();
        
        if (networkException)
        {
            networkException->rethrow();
        }
        
        /*
         * NO EXCEPTION IS THROWN UPON TIME-OUT WHEN USING HTTPS
         * THIS IS THEREFORE THE ONLY WAY TO TACKLE THE ISSUE
         *
         * REGARDING HTTP CONNECTIONS: THE FOLLOWING SHOULD NEVER HAPPEN, BUT WHO KNOWS?
         */
        if (request.method != HTTPRequest::HTTP_HEAD)
        {
            if ((response.contentLength != HTTPResponse::UNKNOWN_CONTENT_LENGTH) && (response.contentLength != downloadedSize))
            {
                throw Error(ERROR_INCOMPLETE_CONTENT);
            }
        }
    }
    catch (Error &e)
    {
        if (state == STATE_STARTED)
        {
            if (handler)
            {
                if (e.getMessage().empty())
                {
                    string message;
                    
                    switch (e.getCode())
                    {
                        case ERROR_UNSUPPORTED_REDIRECT:
                            message = "UNSUPPORTED REDIRECT";
                            break;
                            
                        case ERROR_UNSUPPORTED_PROTOCOL:
                            message = "UNSUPPORTED PROTOCOL";
                            break;
                            
                        case ERROR_INCOMPLETE_CONTENT:
                            message = "INCOMPLETE CONTENT";
                            break;
                            
                        case ERROR_INVALID_CONTENT_LENGTH:
                            message = "INVALID CONTENT-LENGTH";
                            break;
                            
                        case ERROR_CONTENT_LIMIT_EXCEEDED:
                            message = "CONTENT-LIMIT EXCEEDED";
                            break;
                            
                        case ERROR_INVALID_FILENAME_IN_URI:
                            message = "INVALID FILENAME IN URI";
                            break;
                            
                        case ERROR_SIMULATED_NETWORK_FAILURE:
                            message = "ERROR SIMULATED NETWORK FAILURE";
                            break;
                            
                        default:
                            message = "UNDEFINED ERROR";
                            break;
                    }
                    
                    LOGI_IF(LOG_VERBOSE) << "HttpTask - STATUS: " << response.status << " | ERROR: [" << e.getCode() << "] " << message << endl;
                    post([=]{ handler->requestFailed(request, response, Error(e.getCode(), message)); });
                }
                else
                {
                    LOGI_IF(LOG_VERBOSE) << "HttpTask - STATUS: " << response.status << " | HTTP ERROR: " << e.getMessage() << endl;
                    post([=]{ handler->requestFailed(request, response, e); });
                }
            }
            
            state = STATE_FAILED;
        }
    }
    catch (Poco::Exception &e)
    {
        if (state == STATE_STARTED)
        {
            if (handler)
            {
                ErrorCode code = ERROR_UNDEFINED;

                try
                {
                    e.rethrow();
                }
                catch (InvalidAddressException&)
                {
                    code = ERROR_INVALID_ADDRESS;
                }
                catch (ServiceNotFoundException&)
                {
                    code = ERROR_SERVICE_NOT_FOUND;
                }
                catch (ConnectionRefusedException&)
                {
                    code = ERROR_CONNECTION_REFUSED;
                }
                catch (HostNotFoundException&)
                {
                    code = ERROR_HOST_NOT_FOUND;
                }
                catch (NoAddressFoundException&)
                {
                    code = ERROR_NO_ADDRESS_FOUND;
                }
                catch (MessageException&)
                {
                    code = ERROR_MALFORMED_MESSAGE;
                }
                catch (NotAuthenticatedException&)
                {
                    code = ERROR_NOT_AUTHENTICATED;
                }
                catch (...)
                {}
                
                /*
                 * WARNING:
                 * CAPTURING e.displayText() VIA A LAMBDA DISCARDS INHERITANCE
                 * WHICH IN TURN PRODUCES MISLEADING MESSAGES (E.G. "Exception: http://foo.com" INSTEAD OF "Host not found: http://foo.com")
                 */
                string message = e.displayText();
                
                LOGI_IF(LOG_VERBOSE) << "HttpTask - STATUS: " << response.status << " | POCO ERROR: [" << code << "] " << message << endl;
                post([=]{ handler->requestFailed(request, response, Error(code, message)); });
            }
            
            state = STATE_FAILED;
        }
    }
    catch (...)
    {
        if (state == STATE_STARTED)
        {
            if (handler)
            {
                LOGI_IF(LOG_VERBOSE) << "HttpTask - STATUS: " << response.status << " | UNHANDLED ERROR" << endl;
                post([=]{ handler->requestFailed(request, response, Error(ERROR_UNDEFINED)); });
            }
            
            state = STATE_FAILED;
        }
    }
    
    if (state == STATE_STARTED)
    {
        if (handler)
        {
            if (!response.filePath.empty())
            {
                post([=]{ handler->requestSucceeded(request, response); });
            }
            else
            {
                if (hasPendingHttpError)
                {
                    LOGI_IF(LOG_VERBOSE) << "HttpTask - STATUS: " << response.status << " | HTTP ERROR [CUSTOM PAYLOAD]" << endl;
                    post([=]{ handler->requestFailed(request, response, pendingHttpError); });
                }
                else
                {
                    post([=]{ handler->requestSucceeded(request, response); });
                }
            }
        }
        
        state = hasPendingHttpError ? STATE_FAILED : STATE_SUCCEEDED;
    }

    // ---
    
    if (handler)
    {
        post([=]{ handler->requestEnded(request); });
    }
    
    endSession();
}

/*
 * IT WOULD BE TEMPTING TO USE session->abort()
 * BUT IT'S LEADING TO A CRASH ON OSX AND iOS
 * WHEN THE session IS DELETED (HTTPS ONLY)
 *
 * ANYWAY, IT SEEMS THAT WE DON'T REALLY NEED THAT BECAUSE
 * DELETING session IS PROPERLY CLOSING THE UNDERLYING SOCKET
 */
void HttpTask::cancelIfRequired()
{
    if (isAwaitingCancellation())
    {
        if (state == STATE_STARTED)
        {
            if (handler)
            {
                post([=]{ handler->requestCancelled(request); });
            }

            state = STATE_CANCELLED;
            
            /*
             * DUMMY EXCEPTION, USED FOR LOOP-BREAKING
             */
            throw Error();
        }
    }
}

/*
 * XXX: PROBABLY TEMPORARY
 */
void HttpTask::failIfRequired()
{
    if (SIMULATE_NETWORK_FAILURE)
    {
        throw Error(ERROR_SIMULATED_NETWORK_FAILURE);
    }
}

void HttpTask::beginSession(const URI &uri)
{
    if (uri.getScheme() == "http")
    {
        session = new HTTPClientSession(uri.getHost(), uri.getPort());
    }
    else if (uri.getScheme() == "https")
    {
        ssl = true;
        
        /*
         * THIS IS STATIC INITIALIZATION: WILL HAPPEN ONLY ONCE
         *
         * THE "CORRESPONDING" Net::uninitializeSSL() IS NEVER CALLED
         * BECAUSE IT'S NOT MANDAYORY AND BECAUSE THERE IS NO OBVIOUS
         * EVENT FOR IT TO TAKE PLACE ANYWAY...
         */
        if (!httptask::sslInitialized)
        {
            Net::initializeSSL();
            httptask::sslInitialized = true;
        }
        
        SharedPtr<InvalidCertificateHandler> pInvalidCertHandler = new AcceptCertificateHandler(false);
        Context::Ptr pContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        SSLManager::instance().initializeServer(nullptr, pInvalidCertHandler, pContext);
        
        session = new HTTPSClientSession(uri.getHost(), uri.getPort(), pContext);
    }
    else
    {
        throw Error(ERROR_UNSUPPORTED_PROTOCOL);
    }
}

void HttpTask::endSession()
{
    if (session)
    {
        delete session;
        session = nullptr;
    }
}

/*
 * WARNING: NOTHING HAPPENS IF THE FILE-PATH IS NOT VALID
 */
void HttpTask::readToFile(istream& input, ofstream& ouput, size_t bufferSize)
{
    Poco::Buffer<char> buffer(bufferSize);
    
    while (true)
    {
        input.read(buffer.begin(), bufferSize);
        auto count = input.gcount();
        
        cancelIfRequired();
        failIfRequired();
        
        if (count > 0)
        {
            downloadedSize += count;
            ouput.write(buffer.begin(), count);
            
            if (handler)
            {
                post([=]{ handler->requestProgress(request, downloadedSize, response.contentLength); });
            }
        }
        else
        {
            break;
        }
    }
}

/*
 * TODO: SOME ROOM FOR OPTIMIZATION...
 */
void HttpTask::readToString(istream& input, string &output, size_t bufferSize)
{
    Poco::Buffer<char> buffer(bufferSize);
    
    while (true)
    {
        input.read(buffer.begin(), bufferSize);
        auto count = input.gcount();
        
        cancelIfRequired();
        failIfRequired();
        
        if (count > 0)
        {
            downloadedSize += count;
            output.append(buffer.begin(), count);
            
            if (handler)
            {
                post([=]{ handler->requestProgress(request, downloadedSize, response.contentLength); });
            }
        }
        else
        {
            break;
        }
    }
}

string HttpTask::extractFilename(const URI &uri)
{
    const string path = uri.getPath();
    size_t found = path.find_last_of("/");
    
    if (found != string::npos)
    {
        return path.substr(found + 1);
    }
    else
    {
        return path;
    }
}
