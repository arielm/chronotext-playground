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
        if (force || false) testLayoutAdvance();
        
        if (force || true) testStringCache1();
    }
}

// ---

void TestingZFont::testLayoutAdvance()
{
    FontManager fontManager;

    auto font = fontManager.getFont(InputSource::getAsset("DroidSerif_Regular.xml"), ZFont::Properties2d(32).setCrisp());
    auto lineLayout = font->getCachedLineLayout("Which way to the station?");
    
    LOGI << "LINE-LAYOUT ADVANCE: " << lineLayout->advance << endl;
}

// ---

void TestingZFont::testStringCache1()
{
    LOGI << getValue("foo", 123, false) << endl;
}

string TestingZFont::getValue(const ObservableString &key1, int key2, bool key3)
{
    auto it = cache.left.find(tie(key3, key2, key1));
    
    if (it != cache.left.end())
    {
        return it->second;
    }
    else
    {
        string value = string(key1) + "|" + toString(key2) + "|" + toString(key3);
        cache.insert(typename container_type::value_type(tie(key3, key2, key1), value));
        
        return value;
    }
}

