/*
 * PLAYGROUND IN SYNC WITH develop BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT c371b7b
 * IOS: COMMIT c371b7b
 * ANDROID: COMMIT c371b7b
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
