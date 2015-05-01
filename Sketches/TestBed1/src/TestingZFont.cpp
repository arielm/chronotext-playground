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
        
        if (force || false) testCache1();
        if (force || false) testMap1();
        if (force || true) testMap2();
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

void TestingZFont::testCache1()
{
    LOGI << getCacheValue1(observable1, 123, false) << endl << endl; // 5 COPIES (INCLUDING INSERTION INTO MAP)
    LOGI << getCacheValue1(observable1, 123, false) << endl << endl; // 2 COPIES
}

string TestingZFont::getCacheValue1(const ObservableString &key1, int key2, bool key3)
{
    auto it = cache1.left.find(tie(key3, key2, key1));
    
    if (it != cache1.left.end())
    {
        return it->second;
    }
    
    auto value = createValue(key2, key3);
    cache1.insert(typename container_type::value_type(forward_as_tuple(key3, key2, key1), value));
    
    return value;
}

// ---

void TestingZFont::testMap1()
{
    LOGI << getMapValue1(observable1, 123, false) << endl << endl; // 2 COPIES (INCLUDING INSERTION INTO MAP)
    LOGI << getMapValue1(observable1, 123, false) << endl << endl; // 1 COPY
}

string TestingZFont::getMapValue1(const ObservableString &key1, int key2, bool key3)
{
    auto found = map1.find(forward_as_tuple(key3, key2, key1)); // A Key MUST BE CREATED AND THEREFORE key1 WILL BE COPIED
    
    if (found != map1.end())
    {
        return found->second;
    }
    
    auto value = createValue(key2, key3);
    map1.emplace(forward_as_tuple(key3, key2, key1), value);
    
    return value;
}

// ---

void TestingZFont::testMap2()
{
    LOGI << getMapValue2(observable1, 123, false) << endl << endl; // 1 COPY (INSERTION INTO MAP)
    LOGI << getMapValue2(observable1, 123, false) << endl << endl; // 0 COPIES
}

string TestingZFont::getMapValue2(const ObservableString &key1, int key2, bool key3)
{
    auto found = map2.find(key1);
    
    if (found != map2.end())
    {
        return found->second;
    }
    
    
    auto value = createValue(key2, key3);
    map2.emplace(key1, value);
    
    return value;
}
