/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingIOService.h"

#include "chronotext/Context.h"

using namespace std;
using namespace ci;
using namespace chr;

void TestingIOService::setup()
{
    TaskManager::LOG_VERBOSE = true;
    
    io = make_shared<boost::asio::io_service>();
    ioWork = make_shared<boost::asio::io_service::work>(*io);
    
    // ---

    /*
     * WILL NOT BE EXECUTED IMMEDIATELY (I.E. ONLY WITHIN THE NEXT "POLL")
     *
     * UNLIKE DOCUMENTED: NO DIFFERENCE BETWEEN io_service::post AND io_service::dispatch
     */
    post([=] { callback1(); });

    taskManager().addTask(make_shared<TestTask1>(*this));
}

void TestingIOService::shutdown()
{
    io->stop();
}

void TestingIOService::update()
{
    frameCount++;
    io->poll();
}

void TestingIOService::callback1()
{
    LOGI << "CALLBACK 1 | " << frameCount << endl;
}

void TestingIOService::callback2()
{
    LOGI << "CALLBACK 2 | " << frameCount << endl;
    
    /*
     * WILL BE EXECUTED IMMEDIATELY (I.E. WHEN INVOKED FROM WITHIN THE CURRENT "POLL")
     *
     * UNLIKE DOCUMENTED: NO DIFFERENCE BETWEEN io_service::post AND io_service::dispatch
     */
    post([=] { callback3(); });
}

void TestingIOService::callback3()
{
    LOGI << "CALLBACK 3 | " << frameCount << endl;
}

// ---

TestTask1::TestTask1(TestingIOService &context)
:
context(context)
{}

void TestTask1::run()
{
    Timer timer(true);
    LOGI << "ZERO - " << utils::format::duration(timer.getSeconds()) << " | " << context.frameCount << endl;
    
    Task::sleep(1.0f);
    LOGI << "ONE - " << utils::format::duration(timer.getSeconds()) << " | " << context.frameCount << endl;
    
    /*
     * WILL BE EXECUTED WITHIN THE "NEXT" POLL
     */
    context.post([=] { context.callback2(); });
    
    Task::sleep(1.0f);
    LOGI << "TWO - " << utils::format::duration(timer.getSeconds()) << " | " << context.frameCount << endl;
}
