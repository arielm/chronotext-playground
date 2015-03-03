#include "Sketch.h"

#if defined(TARGET1)
#   include "TestingSound.h"
#elif defined(TARGET2)
#   include "TestingNetwork.h"
#endif

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
#if defined(TARGET1)
        addFrameTest<TestingSound>(true);
#elif defined(TARGET2)
        addFrameTest<TestingNetwork>(true);
#endif
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

#if defined(TARGET1)
    gl::clear(Color(1, 0, 0), false);
#elif defined(TARGET2)
    gl::clear(Color(0, 0, 1), false);
#endif
    
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
