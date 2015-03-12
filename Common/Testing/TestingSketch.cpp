/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingSketch.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

void TestingSketch::shutdown()
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

void TestingSketch::update()
{
    if (getElapsedFrames() == 1)
    {
        begin();
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

// ---

void TestingSketch::addTouch(int index, float x, float y)
{
    for (auto test : frameTests)
    {
        test->addTouch(index, x, y);
    }
}

void TestingSketch::updateTouch(int index, float x, float y)
{
    for (auto test : frameTests)
    {
        test->updateTouch(index, x, y);
    }
}

void TestingSketch::removeTouch(int index, float x, float y)
{
    for (auto test : frameTests)
    {
        test->removeTouch(index, x, y);
    }
}

// ---

bool TestingSketch::keyDown(const KeyEvent &keyEvent)
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

bool TestingSketch::keyUp(const KeyEvent &keyEvent)
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
