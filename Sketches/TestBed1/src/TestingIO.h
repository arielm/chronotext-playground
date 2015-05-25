/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * TESTED ON OSX, iOS AND EMSCRIPTEN (STANDALONE)
 *
 * TODO: TEST ON ANDROID (PRE-REQ: COMPILE BOOST 1.53 WITH iostreams)
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
