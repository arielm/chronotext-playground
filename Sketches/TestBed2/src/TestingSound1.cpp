/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingSound1.h"

#include "chronotext/Context.h"

#include "cinder/Rand.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

void TestingSound1::setup()
{
    if (isLocked("FMOD"))
    {
        throw EXCEPTION(TestingSound1, "FMOD IS ALREADY IN USE");
    }
    else
    {
        lock("FMOD");
    }
    
    assert(!soundManager);
    soundManager = make_shared<SoundManager>();

    SoundManager::LOG_VERBOSE = true;
    SoundManager::PROBE_MEMORY = true;

    if (!soundManager->init(MAX_CHANNELS))
    {
        LOGI << "UNABLE TO INITIALIZE SoundManager" << endl;

        soundManager.reset();
        return;
    }
    
    // ---

    soundManager->addListener(this);

    for (auto &name : {"drumloop.wav", "jaguar.wav", "swish.wav"})
    {
        effects.emplace_back(soundManager->getEffect(InputSource::getResource(name)));
    }
}

void TestingSound1::shutdown()
{
    if (soundManager)
    {
        soundManager->uninit();
        soundManager.reset();
        
        SoundManager::LOG_VERBOSE = false;
    }
    
    unlock("FMOD");
}

void TestingSound1::update()
{
    if (soundManager)
    {
        soundManager->update();
    }
}

void TestingSound1::addTouch(int index, float x, float y)
{
    if (soundManager)
    {
        auto effect = effects[Rand::randInt(effects.size())];
        soundManager->playEffect(effect);
    }
}

bool TestingSound1::keyDown(const KeyEvent &keyEvent)
{
    switch (CinderDelegate::getCode(keyEvent))
    {
        case KeyEvent::KEY_d:
        {
            if (soundManager)
            {
                soundManager->discardEffects();
                return true;
            }
        }
    }
    
    return false;
}

void TestingSound1::handleEvent(const SoundManager::Event &event)
{
    LOGI << event << endl;
}
