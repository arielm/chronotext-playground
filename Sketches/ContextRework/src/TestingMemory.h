/*
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
