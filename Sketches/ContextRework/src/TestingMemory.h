/*
 * TODO:
 *
 * 1) CHECK WHY TextureHelper::fetchPowerOfTwoTextureData SEEMS TO LEAK ON OSX
 *
 * 2) COMPARE BETWEEN CINDER'S IMAGE-SOURCES
 *    - TEST "SPECIALIZED PNG HANDLING" (CF CINDER'S ImageFileBasicApp SAMPLE):
 *      - #include "cinder/ImageSourcePng.h"
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

#include "TestingBase.h"

#include "chronotext/texture/TextureManager.h"

class TestingMemory : public TestingBase
{
public:
    void setup();
    void update();
    
protected:
    chr::TextureManager textureManager;
    
    std::vector<chr::InputSourceRef> inputSources;
    int index;
    bool done;
};
