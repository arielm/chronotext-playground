#pragma once

#include "TestingBase.h"

class TestingMisc : public TestingBase
{
public:
    void run(bool force = false) final;
    
    void testFileCapture();
    void testNewLogging();
    void testNewException();
    void testInputSource();
    
    void testSystemAndMemoryInfo();
    void testFileSystem();
};
