/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * TESTED ON OSX, iOS, ANDROID AND EMSCRIPTEN (STANDALONE)
 */

#pragma once

#include "Testing/TestingBase.h"

class TestingIO : public TestingBase
{
public:
    void run(bool force = false) final;
    
    void testIOStreams1();
    void testIOStreams2();
};
