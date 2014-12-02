/*
 * PLAYGROUND IN SYNC WITH ContextRework BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT ffa9f65
 * IOS: COMMIT ffa9f65
 * ANDROID: COMMIT ffa9f65
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
