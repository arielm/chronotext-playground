/*
 * PLAYGROUND IN SYNC WITH ResRework BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT 50ddf77
 * IOS: COMMIT 50ddf77
 * ANDROID: COMMIT 50ddf77
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
