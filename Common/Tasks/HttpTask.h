/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#pragma once

#include "chronotext/os/Task.h"

#include "cinder/FileSystem.h"

#include <Poco/Exception.h>
#include <Poco/URI.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPClientSession.h>

#include <iostream>
#include <atomic>

class HttpTask : public chr::Task
{
public:
    static std::atomic<bool> LOG_VERBOSE;
    static std::atomic<bool> SIMULATE_NETWORK_FAILURE; // XXX
    
    struct Request
    {
        std::string uri;
        std::string username;
        std::string password;
        ci::fs::path destinationPath;
        ci::fs::path destinationFolder;
        int id;
        
        long timeoutDuration;
        size_t bufferSize;
        std::streamsize sizeLimit;
        
        std::string method;
        bool readPayloadOnFailure;

        std::map<std::string, std::string> headers;
        std::map<std::string, std::string> params;
        
        Request(const std::string &uri)
        :
        uri(uri),
        id(0),
        timeoutDuration(10),
        bufferSize(8192),
        sizeLimit(0),
        method(Poco::Net::HTTPRequest::HTTP_GET),
        readPayloadOnFailure(false)
        {}
    };
    
    struct Response
    {
        int status;
        std::streamsize contentLength;
        std::string contentType;
        std::string content;
        ci::fs::path filePath;
        
        Response()
        :
        status(0),
        contentLength(0)
        {}
    };

    enum ErrorCode
    {
        /*
         * INTERNAL
         */
        ERROR_UNDEFINED = 0,
        ERROR_UNSUPPORTED_PROTOCOL = 1,
        ERROR_UNSUPPORTED_REDIRECT = 2,
        ERROR_INCOMPLETE_CONTENT = 3,
        ERROR_INVALID_CONTENT_LENGTH = 4,
        ERROR_CONTENT_LIMIT_EXCEEDED = 5,
        ERROR_INVALID_FILENAME_IN_URI = 6,
        ERROR_SIMULATED_NETWORK_FAILURE = 7,
        
        /*
         * HTTP
         */
        ERROR_HTTP = 30,
        
        /*
         * POCO
         */
        ERROR_INVALID_ADDRESS = 40,
        ERROR_SERVICE_NOT_FOUND = 41,
        ERROR_CONNECTION_REFUSED = 42,
        ERROR_HOST_NOT_FOUND = 43,
        ERROR_NO_ADDRESS_FOUND = 44,
        ERROR_MALFORMED_MESSAGE = 45,
        ERROR_NOT_AUTHENTICATED = 46,
    };
    
    class Error : public std::exception
    {
    public:
        ~Error() throw() {}
        
        Error(ErrorCode code = ERROR_UNDEFINED, const std::string &message = "")
        :
        code(code),
        message(message)
        {}
        
        ErrorCode getCode() const
        {
            return code;
        }
        
        std::string getMessage() const
        {
            return message;
        }
        
    protected:
        ErrorCode code;
        std::string message;
    };
    
    class Handler
    {
    public:
        virtual void requestStarted(const Request &request) {}
        virtual void requestSent(const Request &request) {}
        virtual void requestCancelled(const Request &request) {}
        virtual void requestFailed(const Request &request, const Response &response, Error error) {}
        virtual void requestAborted(const Request &request, Error error) {}
        virtual void requestSucceeded(const Request &request, const Response &response) {}
        virtual void requestEnded(const Request &request) {}
        virtual void requestProgress(const Request &request, std::streamsize downloaded, std::streamsize total) {}
    };

    HttpTask(const Request &request, Handler *handler = nullptr);
    
protected:
    enum State
    {
        STATE_UNDEFINED,
        STATE_STARTED,
        STATE_SUCCEEDED,
        STATE_FAILED,
        STATE_CANCELLED,
        STATE_ABORTED
    };
    
    Request request;
    Response response;
    Handler *handler;
    
    State state;
    std::streamsize downloadedSize;

    Poco::Net::HTTPClientSession *session;
    bool ssl;
    
    void run() final;

    void cancelIfRequired();
    void failIfRequired(); // XXX
    
    void beginSession(const Poco::URI &uri);
    void endSession();
    
    void readToFile(std::istream& input, std::ofstream& ouput, std::size_t bufferSize = 8192);
    void readToString(std::istream& input, std::string &output, std::size_t bufferSize = 8192);
    
    static std::string extractFilename(const Poco::URI &uri);
};
