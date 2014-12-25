#include "TestingSound.h"

#include "chronotext/Context.h"

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

void TestingSound::handleEvent(const SoundEngine::Event &event)
{}
