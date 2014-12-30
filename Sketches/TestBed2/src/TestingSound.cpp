#include "TestingSound.h"

#include "chronotext/Context.h"

#include "cinder/Rand.h"

using namespace std;
using namespace ci;
using namespace chr;

using namespace context;

void TestingSound::setup()
{
    if (!soundManager)
    {
        soundManager = make_shared<SoundManager>();
        soundManager->setup(MAX_CHANNELS);
     
        Effect::VERBOSE = true;
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

void TestingSound::handleEvent(const SoundManager::Event &event)
{
    LOGI << event << endl;
}
