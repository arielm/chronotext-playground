/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * TODO:
 *
 * 1) TEST mmap ON OSX, iOS AND ANDROID
 *
 * 2) TEST FILE-MAPPING:
 *    - FOR LOADING IMAGES
 *    - WITH FREETYPE AND HARFBUZZ
 */

#pragma once

#include "Testing/TestingBase.h"

class TestingMemoryMapping : public TestingBase
{
public:
    void setup() final;
    void shutdown() final;
    
    /*
     * PASSING VIA update() IS (CURRENTLY) NECESSARY, IN ORDER TO BE PROPERLY NOTIFIED UPON "MEMORY WARNING"
     */
    void update() final;
};
