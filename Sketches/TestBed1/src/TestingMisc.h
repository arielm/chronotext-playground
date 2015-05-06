/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#pragma once

#include "Testing/TestingBase.h"

#include "ObservableString.h" // TODO: CONSIDER MOVING TO Testing/ObservableString

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
    void testReadTextFile();
    void testReadXmlFile();
    void testReadU16StringToString();
    void testReadStringToU16String();
    
    void testStringToIntToString();
    void testFileSystem();
    
    // ---
    
    ObservableString createWithRVO1();
    ObservableString createWithRVO2();
    ObservableString&& observeWhilePreservingRVO(ObservableString &&s);
    
    void testRVOAndCopyElision();
    
    // ---
    
    void testMapInsertion();
};
