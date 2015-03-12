/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "TestingBase.h"

class TestingSketch : public chr::CinderSketch
{
public:
    void shutdown() final;
    void update() final;
    
    void addTouch(int index, float x, float y) final;
    void updateTouch(int index, float x, float y) final;
    void removeTouch(int index, float x, float y) final;
    
    bool keyDown(const ci::app::KeyEvent &keyEvent) final;
    bool keyUp(const ci::app::KeyEvent &keyEvent) final;
    
    virtual void begin() = 0;
    
protected:
    std::vector<std::shared_ptr<TestingBase>> frameTests;
    
    template<class TestingBase>
    void executeTest(bool proceed = true, bool force = false)
    {
        if (proceed)
        {
            auto test = std::make_shared<TestingBase>();
            
            try
            {
                test->setup();
                test->run(force);
                test->shutdown();
            }
            catch (std::exception &e)
            {
                LOGI << "TEST FAILED | REASON: " << e.what() << std::endl;
            }
        }
    }
    
    template<class TestingBase>
    void addFrameTest(bool proceed = true)
    {
        if (proceed)
        {
            auto test = std::make_shared<TestingBase>();
            
            try
            {
                test->setup();
                frameTests.push_back(test);
            }
            catch (std::exception &e)
            {
                LOGI << "TEST FAILED | REASON: " << e.what() << std::endl;
            }
        }
    }
};
