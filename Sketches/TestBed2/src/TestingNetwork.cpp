#include "TestingNetwork.h"

#include "chronotext/Context.h"

using namespace std;
using namespace ci;
using namespace chr;

void TestingNetwork::setup()
{
    HttpTask::LOG_VERBOSE = true;
    
    HttpTask::Request request("https://raw.githubusercontent.com/arielm/patches/Xcode5/cinder_0.8.5/lib/libcinder-iphone-sim.a.zip");
    
    taskId = taskManager().registerTask(make_shared<HttpTask>(request, this));
    taskManager().addTask(taskId);
}

void TestingNetwork::shutdown()
{}

void TestingNetwork::addTouch(int index, float x, float y)
{
    taskManager().cancelTask(taskId);
}

// --- HttpTask::Handler ---

void TestingNetwork::requestStarted(const HttpTask::Request &request)
{
    LOGI << "HttpTask::requestStarted: " << request.uri << endl;
}

void TestingNetwork::requestCancelled(const HttpTask::Request &request)
{
    LOGI << "HttpTask::requestCancelled" << endl;
}

void TestingNetwork::requestFailed(const HttpTask::Request &request, const HttpTask::Response &response, HttpTask::Error error)
{
    LOGI << "HttpTask::requestFailed: " << error.getMessage() << endl; // TODO: HttpTask::Error SHOULD DELIVER A MESSAGE AS-MEANINGFUL-AS WHAT IS CURRENTLY PRINTED VIA Http::LOG_VERBOSE
}

void TestingNetwork::requestSucceeded(const HttpTask::Request &request, const HttpTask::Response &response)
{
    LOGI << "HttpTask::requestSucceeded" << endl;
}

void TestingNetwork::requestProgress(const HttpTask::Request &request, streamsize downloaded, streamsize total)
{
    LOGI << "HttpTask::requestProgress: " << utils::format::percent(downloaded / double(total)) << endl;
}
