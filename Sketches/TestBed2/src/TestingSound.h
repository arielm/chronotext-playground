/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * TODO:
 *
 * 1) TEST EFFECT-DISCARDING ON DEVICES:
 *    - VIA "DEVICE SHAKE"
 *
 * 2) TEST MEMORY-CONSUMPTION (OSX AND IOS) WITH Effect::Request::forceMemoryLoad
 *
 * 3) MORE TODO'S IN chronotext/sound/SoundManager.h
 *
 * 4) PROVIDE CREDITS FOR INCLUDED SOUND SAMPLES
 */

#pragma once

#include "TestingBase.h"

#include "chronotext/sound/SoundManager.h"

class TestingSound : public TestingBase, public chr::SoundManager::Listener
{
public:
    static constexpr int MAX_CHANNELS = 6; // A DELIBERATLY-LOW VALUE, IN ORDER TO WITNESS "CHANNEL INTERRUPTION"
    
    void setup() final;
    void shutdown() final;
    
    /*
     * PASSING VIA update() IS NECESSARY IN ORDER TO WORK WITH THE SOUND-ENGINE
     */
    void update() final;
    
    void addTouch(int index, float x, float y) final;
    bool keyDown(const ci::app::KeyEvent &keyEvent) final;
    
    void handleEvent(const chr::SoundManager::Event &event) final;
    
protected:
    std::shared_ptr<chr::SoundManager> soundManager;
    std::vector<chr::Effect::Ref> effects;
};
