/*
 * PLAYGROUND IN SYNC WITH develop BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT 6f6517a
 * IOS: COMMIT 6f6517a
 * ANDROID: COMMIT 6f6517a
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
