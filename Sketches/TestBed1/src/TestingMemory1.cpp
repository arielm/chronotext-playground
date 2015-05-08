/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingMemory1.h"

#include "chronotext/Context.h"

using namespace std;
using namespace ci;
using namespace chr;

/*
 * TODO: TRY TO CAUSE SOME OUT-OF-MEMORY EXCEPTION
 */
Unit::Unit(size_t size)
:
_size(size)
{
//  LOGI << __PRETTY_FUNCTION__ << endl;

    /*
     * USING THE () OPERATOR WOULD ZERO-FILL THE BLOCK
     *
     * OTHERWISE:
     *
     * - ON OSX: MEMORY IS NOT FILLED (AND EVEN NOT "RESERVED")
     * - TODO: TEST ON iOS AND ANDROID
     */
    _data = new uint8_t[size];
    
    for (auto i = 0; i < size; i++)
    {
        _data[i] = rand() % 256;
    }
}

Unit::~Unit()
{
//  LOGI << __PRETTY_FUNCTION__ << endl;
    
    delete[] _data;
}

size_t Unit::size() const
{
    return _size;
}

uint8_t* Unit::data() const
{
    return _data;
}

const string Unit::write() const
{
    stringstream ss;
    
    ss << "UNIT: {" <<
    "size: " << utils::format::bytes(_size) << ", " <<
    this <<
    "}";
    
    return ss.str();
}

// ---

void Measure::begin()
{
    assert(!began);
    
    before = getMemoryInfo();
    began = true;
}

void Measure::end()
{
    assert(began && (balance == -1));
    
    after = getMemoryInfo();
    balance = memory::compare(before, after);
}

const string Measure::write() const
{
    stringstream ss;
    
    ss << "MEASURE: {" <<
    "balance: " << utils::format::bytes(balance) << ", " <<
    "memory-info: " << utils::format::write(after) <<
    "}";
    
    return ss.str();
}

// ---

void TestingMemory1::setup()
{
    LOGI << endl << "MEMORY INFO - BEFORE: " << getMemoryInfo() << endl << endl;
    
    if (system::platform() == system::PLATFORM_OSX)
    {
        /*
         * DEMONSTRATING THE "MEMORY-RELEASE DETECTION" ISSUE ON OSX
         *
         * UNSOLVED:
         * - IT STARTS TO WORK-AS-INTENDED ONLY AFTER 16 FRAMES!
         * - MINIMAL ALLOCATION-SIZE IN ORDER TO REPRODUCE: 256KB
         *
         * INTERESTING FACT:
         * - NO MEMORY IS ACTUALLY "RESERVED" UNTIL DATA IS WRITTEN!
         */
        
        unitDataSize = 256 * 1024; // 256KB
        unitCount = 20;
    }
    else
    {
        /*
         * DEMONSTRATING "MEMORY WARNING" ON IOS OR ANDROID
         *
         * ADJUST unitCount ACCORDINGLY...
         */
        
        unitDataSize = 2048 * 1024; // 2MB
        unitCount = 300;
    }
    
    adding = false;
    removing = false;
    done = false;
    
    units.clear();
}

void TestingMemory1::shutdown()
{
    LOGI << endl << "MEMORY INFO - AFTER: " << getMemoryInfo() << endl;
}

void TestingMemory1::update()
{
    if (!done)
    {
        if (!adding && !removing)
        {
            adding = true;
            
            LOGI << "---------- ADDING " << unitCount << " UNITS ----------" << endl << endl;
        }
        
        if (adding)
        {
            Measure measure;
            measure.begin();
            
            units.emplace_back(new Unit(unitDataSize));
            
            measure.end();
            
            LOGI << units.back()->write() << " | " << measure.write() << endl;
            
            // --
            
            if (units.size() == unitCount)
            {
                adding = false;
                removing = true;
                
                LOGI << endl << "---------- REMOVING " << unitCount << " UNITS ----------" << endl << endl;
            }
        }
        else if (removing)
        {
            Measure measure;
            measure.begin();
            
            string tmp = units.front()->write();
            units.erase(units.begin());
            
            measure.end();
            
            LOGI << tmp << " | " << measure.write() << endl;
            
            // --
            
            if (units.empty())
            {
                removing = false;
                done = true;
            }
        }
    }
}
