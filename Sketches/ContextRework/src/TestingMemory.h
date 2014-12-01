#pragma once

#include "TestingBase.h"

class TestingMemory : public TestingBase
{
public:
    void run(bool force = false);
    
    void testMemoryLoad1();
    
    static void dumpMemoryStats();
};
