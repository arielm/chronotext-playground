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
 * 1) develop BRANCH IN https://github.com/new-chronotext-toolkit
 *    OSX: COMMIT 5ae6f4b
 *    IOS: COMMIT 5ae6f4b
 *    ANDROID: COMMIT 5ae6f4b
 *
 * 2) master BRANCH IN https://github.com/arielm/Freetype
 *
 * 3) master BRANCH IN https://github.com/arielm/hb-icu
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
