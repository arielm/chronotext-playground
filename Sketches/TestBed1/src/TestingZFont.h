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

#include <boost/functional/hash.hpp>

#include <unordered_map>
#include <functional>

typedef std::tuple<bool, int, ObservableString> Key;

struct KeyHasher
{
    std::size_t operator()(const Key& k) const
    {
        using boost::hash_value;
        using boost::hash_combine;
        
        using std::hash;
        using std::string;
        using std::get;
        
        std::size_t seed = 0;
        
        hash_combine(seed, hash<string>()(string(get<2>(k)))); // NOT EFFECTIVE BUT FINE FOR THE PURPOSE OF TESTING
        hash_combine(seed, hash_value(get<1>(k)));
        hash_combine(seed, hash_value(get<0>(k)));
        
        return seed;
    }
};

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
    
    //
    
    std::unordered_map<Key, std::string, KeyHasher> map3;
    
    void testMap3();
    std::string getMapValue3(const ObservableString &key1, int key2, bool key3);
};
