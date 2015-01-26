/*
 * ...
 */

#pragma once

#include "TestingBase.h"

class TestingZFont : public TestingBase
{
public:
    void setup() final;
    void shutdown() final;
    void run(bool force = false) final;
    
    void test1();
};
