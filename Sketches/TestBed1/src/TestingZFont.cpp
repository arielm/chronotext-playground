/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingZFont.h"

#include "chronotext/Context.h"
#include "chronotext/font/zf/FontManager.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::zf;

void TestingZFont::setup()
{
    FontManager::LOG_VERBOSE = true;
}

void TestingZFont::shutdown()
{
    FontManager::LOG_VERBOSE = false;
}

void TestingZFont::run(bool force)
{
    if (force || true)
    {
        if (force || true) test1();
    }
}

void TestingZFont::test1()
{
    FontManager fontManager;

    auto font = fontManager.getFont(InputSource::getAsset("DroidSerif_Regular.xml"), ZFont::Properties2d(32).setCrisp());
    auto lineLayout = font->getCachedLineLayout("Which way to the station?");
    
    LOGI << "LINE-LAYOUT ADVANCE: " << lineLayout->advance << endl;
}
