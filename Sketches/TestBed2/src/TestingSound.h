#pragma once

#include "TestingBase.h"

#include "chronotext/incubator/sound/SoundEngine.h"

class TestingSound : public TestingBase, public SoundEngine::Listener
{
public:
    static int MAX_CHANNELS;
    
    void setup() final;
    void shutdown() final;
    void update() final;
    
    void handleEvent(const SoundEngine::Event &event) final;
    
protected:
    std::shared_ptr<SoundEngine> engine;
};
