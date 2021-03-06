/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#pragma once

#include "Testing/TestingBase.h"

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
    
    template<typename F>
    inline void post(F&& fn)
    {
        io->post(std::forward<F>(fn));
    }
    
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
