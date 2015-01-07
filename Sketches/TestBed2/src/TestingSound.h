/*
 * TODO:
 *
 * 1) TEST EFFECT-DISCARDING:
 *    - VIA KEYBOARD:
 *      - PRE-REQUISITE: KEYBOARD-HANDLING AT THE TestingBase (OR CinderSketch) LEVEL
 *    - VIA "DEVICE SHAKE"
 *
 * 2) TEST MEMORY-CONSUMPTION (OSX AND IOS) WITH Effect::Request::forceMemoryLoad
 *
 * 3) MORE TODO'S IN SoundManager.h
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
    void update() final;
    
    void addTouch(int index, float x, float y) final;
    
    void handleEvent(const chr::SoundManager::Event &event) final;
    
protected:
    std::shared_ptr<chr::SoundManager> soundManager;
    std::vector<chr::Effect::Ref> effects;
};
