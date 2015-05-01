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
        if (force || false) testMap2();
        if (force || true) testMap3();
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

/*
 * boost::bimaps WITH TUPLE KEY (INCLUDING STRING) USED IN set AND list
 *
 * PROS:
 * - BI-DIRECTIONALITY (COMMON-KEY SHARED BY set AND list)
 * - COMMON-KEY IS ONLY STORED ONCE
 *
 * CONS:
 * - LOTS OF TEMPORARIES
 *   - UPON INSERTION
 *   - UPON ACCESS
 */

void TestingZFont::testCache1()
{
    LOGI << endl;
    
    LOGI << getCacheValue1(observable1, 123, false) << endl << endl; // 4 TEMPORARIES + 1 COPY (INSERTION INTO MAP)
    
    LOGI << getCacheValue1(observable1, 123, false) << endl << endl; // 2 TEMPORARIES
}

string TestingZFont::getCacheValue1(const ObservableString &key1, int key2, bool key3)
{
    auto it = cache1.left.find(forward_as_tuple(key3, key2, key1));
    
    if (it != cache1.left.end())
    {
        return it->second;
    }
    
    auto value = createValue(key2, key3);
    cache1.insert(typename container_type::value_type(forward_as_tuple(key3, key2, key1), value));
    
    return value;
}

// ---

/*
 * std::map WITH TUPLE KEY (INCLUDING STRING)
 *
 * SIMILAR TO std::unordered_map IN TERM OF COPIES AND TEMPORARIES
 */

void TestingZFont::testMap1()
{
    LOGI << getMapValue1(observable1, 123, false) << endl << endl; // 1 TEMPORARY + 1 COPY (INSERTION INTO MAP)
    LOGI << getMapValue1(observable1, 456, true) << endl << endl << endl; // 1 COPY (INSERTION INTO MAP)
    
    LOGI << getMapValue1(observable1, 123, false) << endl << endl; // 1 TEMPORARY
    LOGI << getMapValue1(observable1, 456, true) << endl << endl << endl; // 1 TEMPORARY
}

string TestingZFont::getMapValue1(const ObservableString &key1, int key2, bool key3)
{
    auto found = map1.find(forward_as_tuple(key3, key2, key1)); // 1 TEMPORARY
    
    if (found != map1.end())
    {
        return found->second;
    }
    
    auto value = createValue(key2, key3);
    map1.emplace(forward_as_tuple(key3, key2, key1), value); // 1 COPY
    
    return value;
}

// ---

/*
 * std::multimap WITH STRING KEY
 *
 * PROS:
 * - NO TEMPORARIES
 *   - NOT UPON INSERTION
 *   - NOT UPON ACCESS
 *
 * CONS:
 * - MULTI-KEY ARE NOT STORED ONLY ONCE
 * - NECESSITY TO ITERATE OVER "EQUAL RANGE"
 */

void TestingZFont::testMap2()
{
    LOGI << getMapValue2(observable1, 123, false) << endl << endl; // 1 COPY (INSERTION INTO MAP)
    LOGI << getMapValue2(observable1, 456, true) << endl << endl << endl; // 1 COPY (INSERTION INTO MAP)
    
    LOGI << getMapValue2(observable1, 123, false) << endl << endl;
    LOGI << getMapValue2(observable1, 456, true) << endl << endl;
}

string TestingZFont::getMapValue2(const ObservableString &key1, int key2, bool key3)
{
    const auto found = map2.equal_range(key1);
    
    for (auto it = found.first; it != found.second; ++it)
    {
        if (compareValues(it->second, key2, key3))
        {
            return it->second;
        }
    }
    
    auto value = createValue(key2, key3);
    map2.emplace(key1, value); // COPY
    
    return value;
}

// ---

/*
 * TODO:
 *
 * 1) TEST WITH unordered_multimap
 *
 * 2) CONSIDER TESTING dense_hash_map
 *    https://google-sparsehash.googlecode.com/svn/trunk/doc/dense_hash_map.html
 */

/*
 * std::unordered_map WITH TUPLE KEY (INCLUDING STRING)
 *
 * SIMILAR TO std::map IN TERM OF COPIES AND TEMPORARIES
 */

void TestingZFont::testMap3()
{
    LOGI << getMapValue3(observable1, 123, false) << endl << endl; // 1 TEMPORARY + 1 COPY (INSERTION INTO MAP)
    LOGI << getMapValue3(observable1, 456, true) << endl << endl << endl; // 1 COPY (INSERTION INTO MAP)
    
    LOGI << getMapValue3(observable1, 123, false) << endl << endl; // 1 TEMPORARY
    LOGI << getMapValue3(observable1, 456, true) << endl << endl << endl; // 1 TEMPORARY
}

string TestingZFont::getMapValue3(const ObservableString &key1, int key2, bool key3)
{
    auto found = map3.find(forward_as_tuple(key3, key2, key1)); // 1 TEMPORARY
    
    if (found != map3.end())
    {
        return found->second;
    }
    
    auto value = createValue(key2, key3);
    map3.emplace(forward_as_tuple(key3, key2, key1), value); // 1 COPY
    
    return value;
}