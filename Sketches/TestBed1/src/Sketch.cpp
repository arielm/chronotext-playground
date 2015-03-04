/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "Sketch.h"

#include "TestingMisc.h"
#include "TestingMemory.h"
#include "TestingTasks.h"
#include "TestingZFont.h"
#include "TestingIOService.h"

#include "chronotext/Context.h"
#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

void Sketch::setup()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::shutdown()
{
    for (auto test : frameTests)
    {
        try
        {
            test->shutdown();
        }
        catch (exception &e)
        {
            LOGI << "TEST FAILED | REASON: " << e.what() << endl;
        }
    }
}

void Sketch::update()
{
    if (getElapsedFrames() == 1)
    {
        executeTest<TestingMisc>(false);
        executeTest<TestingTasks>(false);
        executeTest<TestingZFont>(false);
        
        addFrameTest<TestingMemory>(false);
        addFrameTest<TestingIOService>(true);
    }
    
    if (getElapsedFrames() >= 1)
    {
        for (auto test : frameTests)
        {
            try
            {
                test->update();
            }
            catch (exception &e)
            {
                LOGI << "TEST FAILED | REASON: " << e.what() << endl;
            }
        }
    }
}

void Sketch::draw()
{
    gl::setMatricesWindow(getWindowSize(), true);
    gl::clear(Color::gray(0.5f), false);
    
    gl::color(Color::white());
    utils::gl::drawGrid(getWindowBounds(), 64, Vec2f(0, clock()->getTime() * 60));
}

// ---

void Sketch::addTouch(int index, float x, float y)
{
    for (auto test : frameTests)
    {
        test->addTouch(index, x, y);
    }
}

void Sketch::updateTouch(int index, float x, float y)
{
    for (auto test : frameTests)
    {
        test->updateTouch(index, x, y);
    }
}

void Sketch::removeTouch(int index, float x, float y)
{
    for (auto test : frameTests)
    {
        test->removeTouch(index, x, y);
    }
}

// ---

bool Sketch::keyDown(const KeyEvent &keyEvent)
{
    for (auto test : frameTests)
    {
        if (test->keyDown(keyEvent))
        {
            return true;
        }
    }
    
    return false;
}

bool Sketch::keyUp(const KeyEvent &keyEvent)
{
    for (auto test : frameTests)
    {
        if (test->keyUp(keyEvent))
        {
            return true;
        }
    }
    
    return false;
}

// ---

template<class TestingBase>
void Sketch::executeTest(bool proceed, bool force)
{
    if (proceed)
    {
        auto test = make_shared<TestingBase>();
        
        try
        {
            test->setup();
            test->run(force);
            test->shutdown();
        }
        catch (exception &e)
        {
            LOGI << "TEST FAILED | REASON: " << e.what() << endl;
        }
    }
}

template<class TestingBase>
void Sketch::addFrameTest(bool proceed)
{
    if (proceed)
    {
        auto test = make_shared<TestingBase>();
        
        try
        {
            test->setup();
            frameTests.push_back(test);
        }
        catch (exception &e)
        {
            LOGI << "TEST FAILED | REASON: " << e.what() << endl;
        }
    }
}
