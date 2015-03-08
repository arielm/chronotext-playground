/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * PLAYGROUND IN SYNC WITH:
 *
 * 1) develop BRANCH IN https://github.com/new-chronotext-toolkit
 *    OSX: COMMIT c141034
 *    IOS: COMMIT c141034
 *    ANDROID: COMMIT c141034
 *
 * 2) master BRANCH IN https://github.com/arielm/Freetype
 *
 * 3) master BRANCH IN https://github.com/arielm/hb-icu
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
