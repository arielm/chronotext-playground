/*
 * PLAYGROUND IN SYNC WITH ContextRework BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT 5f9ee34
 * IOS: COMMIT 5f9ee34
 * ANDROID: COMMIT 5f9ee34
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
