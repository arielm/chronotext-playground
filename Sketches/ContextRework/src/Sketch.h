/*
 * PLAYGROUND IN SYNC WITH ContextRework BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT 3ee645a
 * IOS: COMMIT 3ee645a
 * ANDROID: COMMIT 3ee645a
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "TestingBase.h"

class Sketch : public chr::CinderSketch
{
public:
    Sketch();
    
    void setup();
    void shutdown();
    
    void update();
    void draw();
    
protected:
    std::shared_ptr<TestingBase> frameTest;
};
