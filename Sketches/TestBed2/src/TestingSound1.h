/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * AMONG OTHER THINGS:
 *
 * - UNLIKE TestingSound2: THIS TEST DEMONSTRATES HOW TO RESPOND WHEN FMOD IS NOT PROPERLY INITIALIZED
 * - TODO: ADD INFO REGARDING THIS TEST'S ADDITIONAL FEATURES
 */

/*
 * TODO:
 *
 * 1) ADD PAUSE/RESUME CAPABILITIES TO TestingBase AND THEN MAKE USE OF SoundManager::pause()/resume()
 *
 * 2) TEST MEMORY-CONSUMPTION FURTHER:
 *    - E.G. WITH Effect::Request::forceMemoryLoad
 *
 * 3) MORE TODO'S IN chronotext/sound/SoundManager.h
 */

#pragma once

#include "TestingBase.h"

#include "chronotext/sound/SoundManager.h"

class TestingSound1 : public TestingBase, public chr::SoundManager::Listener
{
public:
    static constexpr int MAX_CHANNELS = 6; // A DELIBERATLY-LOW VALUE, IN ORDER TO WITNESS "CHANNEL INTERRUPTION"
    
    void setup() final;
    void shutdown() final;
    
    /*
     * PASSING VIA update() IS NECESSARY WHEN WORKING WITH FMOD
     */
    void update() final;
    
    void addTouch(int index, float x, float y) final;
    bool keyDown(const ci::app::KeyEvent &keyEvent) final;
    
    void handleEvent(const chr::SoundManager::Event &event) final;
    
protected:
    std::shared_ptr<chr::SoundManager> soundManager;
    std::vector<chr::Effect::Ref> effects;
};
