/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#pragma once

#include "Testing/TestingBase.h"
#include "ObservableString.h"

#include <boost/bimap.hpp>
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/set_of.hpp>

class TestingZFont : public TestingBase
{
public:
    void setup() final;
    void shutdown() final;
    void run(bool force = false) final;
    
    // ---
    
    void testLayoutAdvance();
    
    // ---
    
    ObservableString observable1 = "foo";
    
    typedef std::tuple<bool, int, ObservableString> Key;
    
    static std::string createValue(int key2, bool key3)
    {
        return ci::toString(key2) + "|" + ci::toString(key3);
    }

    static bool compareValues(const std::string &value, int key2, bool key3)
    {
        return (value == ci::toString(key2) + "|" + ci::toString(key3));
    }

    //
    
    typedef boost::bimaps::bimap<
    boost::bimaps::set_of<Key>,
    boost::bimaps::list_of<std::string>
    > container_type;
    
    container_type cache1;
    
    void testCache1();
    std::string getCacheValue1(const ObservableString &key1, int key2, bool key3);
    
    //
    
    std::map<Key, std::string> map1;
    
    void testMap1();
    std::string getMapValue1(const ObservableString &key1, int key2, bool key3);
    
    //

    std::multimap<ObservableString, std::string> map2;
    
    void testMap2();
    std::string getMapValue2(const ObservableString &key1, int key2, bool key3);
};
