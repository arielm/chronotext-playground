#include "TestingSound.h"

#include "chronotext/Context.h"

#include "cinder/Rand.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

void TestingSound::setup()
{
    if (!soundManager)
    {
        SoundManager::LOG_VERBOSE = true;
        SoundManager::PROBE_MEMORY = true;

        soundManager = make_shared<SoundManager>();
        soundManager->setup(MAX_CHANNELS);
        
        soundManager->addListener(this);
        
        // ---
        
        for (auto &name : {"drumloop.wav", "jaguar.wav", "swish.wav"})
        {
            effects.emplace_back(soundManager->getEffect(InputSource::getAsset(name)));
        }
    }
}

void TestingSound::shutdown()
{
    if (soundManager)
    {
        soundManager->shutdown();
        soundManager.reset();
        
        SoundManager::LOG_VERBOSE = false;
    }
}

void TestingSound::update()
{
    if (soundManager)
    {
        soundManager->update();
    }
}

void TestingSound::addTouch(int index, float x, float y)
{
    auto effect = effects[Rand::randInt(effects.size())];
    soundManager->playEffect(effect);
}

bool TestingSound::keyDown(const KeyEvent &keyEvent)
{
    switch (CinderDelegate::getCode(keyEvent))
    {
        case KeyEvent::KEY_d:
            soundManager->discardEffects();
            return true;
    }
    
    return false;
}

void TestingSound::handleEvent(const SoundManager::Event &event)
{
    LOGI << event << endl;
}
