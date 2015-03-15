/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * PLAYGROUND IN SYNC WITH:
 *
 * 1) new-chronotext-toolkit:
 *    https://github.com/arielm/new-chronotext-toolkit/tree/v4.0.0
 *
 * 2) hb-icu BLOCK:
 *    https://github.com/arielm/hb-icu/tree/toolkit-4.0.0
 *
 * 3) Freetype BLOCK:
 *    https://github.com/arielm/Freetype/tree/toolkit-4.0.0
 */

#pragma once

#include "Testing/TestingSketch.h"

class Sketch : public TestingSketch
{
public:
    void setup() final;
    void draw() final;
    
    void begin() final;
};
