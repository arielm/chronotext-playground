/*
 * PLAYGROUND IN SYNC WITH ResRework BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT b33c7e3
 * IOS: COMMIT b33c7e3
 * ANDROID: COMMIT b33c7e3
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "TestingBase.h"

class Sketch : public chr::CinderSketch
{
public:
    Sketch()
    :
    CinderSketch()
    {}
    
    void setup() final;
    void shutdown() final;
    
    void update() final;
    void draw() final;
    
protected:
    std::shared_ptr<TestingBase> frameTest;
};
