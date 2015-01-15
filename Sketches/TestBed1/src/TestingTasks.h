/*
 * TODO:
 *
 * 1) PRINT TIME-STAMPS DURING test1
 */

#pragma once

#include "TestingBase.h"

class TestingTasks : public TestingBase
{
public:
    void run(bool force = false) final;
    
    void test1();
};
