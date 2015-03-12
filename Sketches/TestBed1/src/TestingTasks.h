/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * TODO:
 *
 * 1) PRINT TIME-STAMPS DURING test1
 */

#pragma once

#include "Testing/TestingBase.h"

class TestingTasks : public TestingBase
{
public:
    void run(bool force = false) final;
    
    void test1();
};
