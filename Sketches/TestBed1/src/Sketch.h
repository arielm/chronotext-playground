/*
 * PLAYGROUND IN SYNC WITH develop BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT 6a5956b
 * IOS: COMMIT 6a5956b
 * ANDROID: COMMIT 6a5956b
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
    
    void addTouch(int index, float x, float y) final;
    void updateTouch(int index, float x, float y) final;
    void removeTouch(int index, float x, float y) final;
    
    bool keyDown(const ci::app::KeyEvent &keyEvent) final;
    bool keyUp(const ci::app::KeyEvent &keyEvent) final;
    
protected:
    std::vector<std::shared_ptr<TestingBase>> frameTests;
    
    template<class TestingBase>
    static void executeTest(bool proceed = true, bool force = false);
    
    template<class TestingBase>
    void addFrameTest(bool proceed = true);
};
