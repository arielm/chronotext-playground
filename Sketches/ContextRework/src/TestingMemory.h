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
    
    std::vector<ci::fs::path> files;
    int fileIndex;
    bool done;
    
    static void dumpMemoryStats();
    static std::string toMB(uint64_t bytes, int precision = 2);
};
