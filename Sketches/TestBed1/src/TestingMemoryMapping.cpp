/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingMemoryMapping.h"

#include "chronotext/Context.h"

using namespace std;
using namespace ci;
using namespace chr;

void TestingMemoryMapping::setup()
{
    LOGI << endl << "MEMORY INFO - BEFORE: " << getMemoryInfo() << endl << endl;
}

void TestingMemoryMapping::shutdown()
{
    LOGI << endl << "MEMORY INFO - AFTER: " << getMemoryInfo() << endl;
}

void TestingMemoryMapping::update()
{}
