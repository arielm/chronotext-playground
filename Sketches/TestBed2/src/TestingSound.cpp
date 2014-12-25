#include "TestingSound.h"

#include "chronotext/Context.h"

#include "cinder/Rand.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace context;

int TestingSound::MAX_CHANNELS = 8; // XXX

void TestingSound::setup()
{
    if (!engine)
    {
        engine = make_shared<SoundEngine>();
        engine->setup(MAX_CHANNELS);
        
        engine->addListener(this);
        
        // ---
        
        for (auto &name : { "drumloop.wav", "jaguar.wav", "swish.wav" })
        {
            effects.emplace_back(engine->preloadEffect(InputSource::getAsset(name)));
        }
    }
}

void TestingSound::shutdown()
{
    if (engine)
    {
        engine->shutdown();
        engine.reset();
    }
}

void TestingSound::update()
{
    if (engine)
    {
        engine->update();
    }
}

void TestingSound::addTouch(int index, float x, float y)
{
    auto effect = effects[Rand::randInt(effects.size())];
    engine->playEffect(effect->getId());
}

void TestingSound::handleEvent(const SoundEngine::Event &event)
{
    LOGI << event << endl;
}
