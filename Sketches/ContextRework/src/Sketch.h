/*
 * PLAYGROUND IN SYNC WITH ContextRework BRANCH IN new-chronotext-toolkit
 *
 * OSX AND ANDROID: HEAD
 * IOS: COMMIT d24d1e4
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
