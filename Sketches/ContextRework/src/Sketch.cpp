#include "Sketch.h"

#include "TestingMisc.h"
#include "TestingMemory.h"
#include "TestingTasks.h"

#include "chronotext/Context.h"
#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

void Sketch::setup()
{
    if (true)
    {
        LOGI << "WINDOW INFO: " << getWindowInfo() << endl;
        LOGI << "DISPLAY INFO: " << getDisplayInfo() << endl;
    }
    
    // ---
    
    TestingBase::execute<TestingMisc>(false);
    TestingBase::execute<TestingTasks>(false);

    if (true)
    {
        frameTest = make_shared<TestingMemory>();
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
     * PASSING VIA update() IS NECESSARY IN ORDER TO BE PROPERLY NOTIFIED UPON "MEMORY WARNING" ON IOS
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
