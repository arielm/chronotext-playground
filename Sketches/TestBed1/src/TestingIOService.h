#pragma once

#include "TestingBase.h"

#include "chronotext/os/Task.h"

class TestingIOService : public TestingBase
{
public:
    int frameCount = 0;
    
    std::shared_ptr<boost::asio::io_service> io;
    std::shared_ptr<boost::asio::io_service::work> ioWork;
    
    void setup() final;
    void shutdown() final;
    void update() final;
    
    void post(std::function<void()> &&fn);
    
    void callback1();
    void callback2();
    void callback3();
};

// ---

class TestTask1 : public chr::Task
{
public:
    TestTask1(TestingIOService &context);
    void run() final;
    
protected:
    TestingIOService &context;
};
