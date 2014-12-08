/*
 * PLAYGROUND IN SYNC WITH ContextRework BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT f7c1a9f
 * IOS: COMMIT f7c1a9f
 * ANDROID: COMMIT f7c1a9f
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
