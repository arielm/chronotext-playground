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
using namespace chr::zf;

void TestingMemoryMapping::setup()
{
    FontManager::LOG_VERBOSE = true;
    fontManager = make_shared<FontManager>();
    
    LOGI << endl << "MEMORY INFO - BEFORE: " << getMemoryInfo() << endl << endl;
}

void TestingMemoryMapping::shutdown()
{
    FontManager::LOG_VERBOSE = false;
    fontManager.reset();
    
    LOGI << endl << "MEMORY INFO - AFTER: " << getMemoryInfo() << endl;
}

void TestingMemoryMapping::update()
{
    if (!loaded && !font)
    {
        auto before = getMemoryInfo();
        font = fontManager->getFont(InputSource::getAsset("droid-sans-fallback.xml"), ZFont::Properties2d(32));
        LOGI << utils::format::bytes(memory::compare(before, getMemoryInfo())) << endl << endl;
        
        loaded = true;
    }
    
    if (loaded && font)
    {
        auto before = getMemoryInfo();
        fontManager->unload(font);
        LOGI << utils::format::bytes(memory::compare(before, getMemoryInfo())) << endl;
        
        font.reset();
    }
}
