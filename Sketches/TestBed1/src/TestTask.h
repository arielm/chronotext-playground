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
