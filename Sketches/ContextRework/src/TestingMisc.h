#pragma once

#include "TestingBase.h"

class TestingMisc : public TestingBase
{
public:
    void run(bool force = false);
    
    void testFileCapture();
    void testNewLogging();
    void testNewException();
    void testSystemInfo();
};
