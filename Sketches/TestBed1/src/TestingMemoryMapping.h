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

/*
 * STAGE 1: FORCING MEMORY-LOADING OF DroidSandFallback.ttf (3.9MB)
 *
 * - iOS AND ANDROID: MEMORY ALLOCATION AND RELEASE PROPERLY DETECTED
 * - OSX: MEMORY-RELEASE IS NOT PROPERLY DETECTED (SEE TestingMemory1)
 */

/*
 * STAGE 2: USING MEMORY-MAPPING 
 *
 * iOS AND OSX: VIA mmap
 * ANDROID: VIA (UNCOMPRESSED) ASSET
 */

#pragma once

#include "Testing/TestingBase.h"

#include "chronotext/font/zf/FontManager.h"

class TestingMemoryMapping : public TestingBase
{
public:
    std::shared_ptr<chr::zf::FontManager> fontManager;
    std::shared_ptr<chr::ZFont> font;
    bool loaded = false;
    
    void setup() final;
    void shutdown() final;
    
    void update() final;
};
