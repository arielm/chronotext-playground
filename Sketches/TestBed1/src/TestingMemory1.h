/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * TODO: DOCUMENT FURTHER...
 */

#pragma once

#include "Testing/TestingBase.h"

#include "chronotext/system/MemoryInfo.h"

class Unit
{
public:
    Unit(size_t size);
    ~Unit();
    
    size_t size() const;
    uint8_t* data() const;
    
    const std::string write() const;
    
protected:
    size_t _size;
    uint8_t *_data;
};

class Measure
{
public:
    chr::MemoryInfo before;
    chr::MemoryInfo after;
    int64_t balance = -1;
    
    void begin();
    void end();
    
    const std::string write() const;
    
protected:
    bool began = false;
};

class TestingMemory1 : public TestingBase
{
public:
    void setup() final;
    void shutdown() final;
    
    /*
     * PASSING VIA update() IS (CURRENTLY) NECESSARY, IN ORDER TO BE PROPERLY NOTIFIED UPON "MEMORY WARNING"
     */
    void update() final;
    
protected:
    std::vector<std::shared_ptr<Unit>> units;
    
    bool adding;
    bool removing;
    bool done;
    
    size_t unitDataSize;
    size_t unitCount;
};
