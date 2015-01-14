/*
 * PLAYGROUND IN SYNC WITH develop BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT 9da7ff1
 * IOS: COMMIT 9da7ff1
 * ANDROID: COMMIT 9da7ff1
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "TestingBase.h"

class Sketch : public chr::CinderSketch
{
public:
    void setup() final;
    void shutdown() final;
    
    void update() final;
    void draw() final;
    
protected:
    std::shared_ptr<TestingBase> frameTest;
};
