/*
 * PLAYGROUND IN SYNC WITH ContextRework BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT 9526476
 * IOS: COMMIT 9526476
 * ANDROID: COMMIT 9526476
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "TestingBase.h"

class Sketch : public chr::CinderSketch
{
public:
    Sketch();
    
    void setup() override;
    void shutdown() override;
    
    void update() override;
    void draw() override;
    
protected:
    std::shared_ptr<TestingBase> frameTest;
};
