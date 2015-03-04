/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#pragma once

#include "chronotext/os/Task.h"

#include <string>

class TestTask : public chr::Task
{
public:
    TestTask(const std::string &message, float delay1, float delay2);
    
    void run() final;
    
protected:
    std::string message;
    float delay1;
    float delay2;
};
