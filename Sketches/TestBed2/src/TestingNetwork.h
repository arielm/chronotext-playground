/*
 * ...
 */

#pragma once

#include "TestingBase.h"

#include "Tasks/HttpTask.h"

class TestingNetwork : public TestingBase
{
public:
    void setup() final;
    void shutdown() final;
    void run(bool force = false) final;
    
protected:
};
