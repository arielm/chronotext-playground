#pragma once

#include "TestingBase.h"

class TestingMisc : public TestingBase
{
public:
    void run(bool force = false) final;

    void testSharedPtrCasting();
    void testFileCapture();

    void testFileSystem();
    void testSystemAndMemoryInfo();

    void testNewLogging();
    void testNewException();
    void testInputSourceRobustness();
    void testTimeFormat();
    void testDurationFormat();
    void testStringToIntToString();
};
