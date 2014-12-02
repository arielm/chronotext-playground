/*
 * RESULTS OF THE CURRENT ITERATION:
 *
 * IOS: ACCURATE "FREE" AND "USED" MEMORY REPORT, APPARENTLY RIGHT FROM THE START
 * ANDROID: ACCURATE "FREE" MEMORY REPORT, BUT CAN BE AFFECTED BY "EXTERNAL FACTORS" (E.G. LIBRARY LOADING)
 * OSX: NOT RELIABLE
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
    
    static std::string writeMemoryStats();
    static std::string writeMB(int64_t bytes, int precision = 2);
};
