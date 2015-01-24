#include "TestingNetwork.h"

#include "chronotext/Context.h"

#include <Poco/Net/SSLManager.h>

using namespace std;
using namespace ci;
using namespace chr;
using namespace Poco;

void TestingNetwork::setup()
{
    Net::initializeSSL();
}

void TestingNetwork::shutdown()
{
    Net::uninitializeSSL();
}

void TestingNetwork::run(bool force)
{
    // TODO: DOWNLOAD FILE FROM GITHUB (HTTPS) USING HttpTask, WITH PROGRESS CALLBACKS
}
