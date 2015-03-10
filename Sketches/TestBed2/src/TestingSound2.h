/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * ...
 */

#pragma once

#include "TestingBase.h"

#include "chronotext/sound/SoundManager.h"

class TestingSound2 : public TestingBase
{
public:
    void setup() final;
    void shutdown() final;
    
    void update() final;
    
    void addTouch(int index, float x, float y) final;
    void removeTouch(int index, float x, float y) final;
    
protected:
};
