/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#pragma once

#include "Testing/TestingBase.h"

#include "chronotext/font/zf/FontManager.h"

class TestingZFont : public TestingBase
{
public:
    chr::zf::FontManager fontManager;
    
    void setup() final;
    void shutdown() final;
    void run(bool force = false) final;
    
    // ---
    
    void testLayoutAdvance();
};
