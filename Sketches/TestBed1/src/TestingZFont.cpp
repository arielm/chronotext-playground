/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingZFont.h"

#include "chronotext/Context.h"
#include "chronotext/utils/MathUtils.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::zf;

void TestingZFont::setup()
{
    FontManager::LOG_VERBOSE = true;
    
    fontManager.loadConfig(InputSource::getAsset("font-config.xml"));
}

void TestingZFont::shutdown()
{
    FontManager::LOG_VERBOSE = false;
}

void TestingZFont::run(bool force)
{
    if (force || true)
    {
        if (force || true) testLayoutAdvance();
    }
}

// ---

void TestingZFont::testLayoutAdvance()
{
    auto font = fontManager.getFont("sans-serif");
    auto lineLayout = font->getLineLayout("Which way to the station?");
    
    assert(utils::math::approximatelyEqual(lineLayout->advance, 367.266f, 0.001f));
}
