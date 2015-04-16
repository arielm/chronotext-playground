/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#pragma once

#include "Testing/TestingBase.h"

class CustomString1
{
public:
    CustomString1(const std::string &s);
    CustomString1(const char *c);
    
    CustomString1(CustomString1 &&other);
    void operator=(CustomString1 &&other);
    
    ~CustomString1();
    
protected:
    char *bytes = nullptr;
    
    CustomString1(const CustomString1 &other) = delete;
    void operator=(const CustomString1 &other) = delete;
};

class TestingMisc : public TestingBase
{
public:
    void run(bool force = false) final;
    
    void testSharedPtrCasting();
    
    void testFileCapture();
    void testNewLogging();
    void testNewException();
    void testInputSourceRobustness();
    void testTimeFormat();
    void testDurationFormat();
    
    void testStringToIntToString();
    void testFileSystem();
    
    // ---
    
    void testCustomString1();
};
