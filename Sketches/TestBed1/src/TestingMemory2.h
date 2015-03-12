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
 * 1) CHECK WHY TextureHelper::fetchPowerOfTwoTextureData SEEMS TO LEAK ON OSX
 *
 * 2) COMPARE BETWEEN CINDER'S IMAGE-SOURCES:
 *    - TESTED SO FAR:
 *      - ImageSourceFileQuartz AND ImageSourceFileStbImage (THE LATTER SEEMS MORE "EFFICIENT")
 *
 * 3) TEST WITH MIP-MAPPING
 *
 *
 * MORE INFO:
 *
 * - https://github.com/arielm/new-chronotext-toolkit/blob/ContextRework/src/chronotext/cocoa/system/MemoryManager.mm
 * - https://github.com/arielm/new-chronotext-toolkit/blob/ContextRework/src/chronotext/android/system/MemoryManager.cpp
 */

#pragma once

#include "Testing/TestingBase.h"

#include "chronotext/texture/TextureManager.h"

class TestingMemory2 : public TestingBase
{
public:
    void setup() final;
    void shutdown() final;
    
    /*
     * PASSING VIA update() IS (CURRENTLY) NECESSARY, IN ORDER TO BE PROPERLY NOTIFIED UPON "MEMORY WARNING"
     */
    void update() final;
    
protected:
    std::shared_ptr<chr::TextureManager> textureManager;
    
    std::vector<chr::InputSource::Ref> inputSources;
    int index;
    bool done;
};
