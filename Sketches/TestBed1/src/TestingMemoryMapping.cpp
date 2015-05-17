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
    FontManager::PROBE_MEMORY = true;
    
    fontManager = make_shared<FontManager>();
    
    LOGI << endl << "MEMORY INFO - BEFORE: " << getMemoryInfo() << endl << endl;
}

void TestingMemoryMapping::shutdown()
{
    fontManager.reset();

    FontManager::LOG_VERBOSE = false;
    FontManager::PROBE_MEMORY = false;
    
    LOGI << endl << "MEMORY INFO - AFTER: " << getMemoryInfo() << endl;
}

void TestingMemoryMapping::update()
{
    if (!loaded && !fonts[0])
    {
        fonts[0] = fontManager->getFont(InputSource::getAsset("droid-sans-fallback.xml"), ZFont::Properties2d(32));
        fonts[1] = fontManager->getFont(InputSource::getAsset("droid-sans-fallback.xml"), ZFont::Properties2d(64));
        
        loaded = true;
    }
    
    if (loaded && fonts[0])
    {
        fontManager->unload(fonts[0]);
        fonts[0].reset();
        
        fontManager->unload(fonts[1]);
        fonts[1].reset();
    }
}
