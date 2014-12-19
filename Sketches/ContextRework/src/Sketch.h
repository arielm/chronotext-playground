/*
 * PLAYGROUND IN SYNC WITH ContextRework BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT ff80b3c
 * IOS: COMMIT ff80b3c
 * ANDROID: COMMIT ff80b3c
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "TestingBase.h"

class Sketch : public chr::CinderSketch
{
public:
    Sketch();
    
    void setup() final;
    void shutdown() final;
    
    void update() final;
    void draw() final;
    
protected:
    std::shared_ptr<TestingBase> frameTest;
};
