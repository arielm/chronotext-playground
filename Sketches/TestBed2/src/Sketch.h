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
    
    void addTouch(int index, float x, float y) final;
    void updateTouch(int index, float x, float y) final;
    void removeTouch(int index, float x, float y) final;
    
protected:
    std::shared_ptr<TestingBase> frameTest;
};
