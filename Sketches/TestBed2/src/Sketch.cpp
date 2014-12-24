#include "Sketch.h"

#include "TestingSound.h"

#include "chronotext/Context.h"
#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

void Sketch::setup()
{
    if (true)
    {
        frameTest = make_shared<TestingSound>();
        frameTest->setup();
    }
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::shutdown()
{
    if (frameTest)
    {
        frameTest->shutdown();
    }
}

void Sketch::update()
{
    /*
     * PASSING VIA update() IS NECESSARY IN ORDER TO WORK WITH SOUND-ENGINE
     */
    
    if (frameTest)
    {
        frameTest->update();
    }
}

void Sketch::draw()
{
    gl::setMatricesWindow(getWindowSize(), true);
    gl::clear(Color::gray(0.5f), false);
    
    gl::color(Color::white());
    utils::gl::drawGrid(getWindowBounds(), 64, Vec2f(0, clock().getTime() * 60));
}