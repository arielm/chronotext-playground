#include "Sketch.h"
#include "TestingMisc.h"

#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

Sketch::Sketch()
:
CinderSketch()
{}

void Sketch::setup()
{
    LOGI << "DISPLAY INFO: " << getDisplayInfo() << endl;
    LOGI << "WINDOW INFO: " << getWindowInfo() << endl;
    
    TestingBase::execute<TestingMisc>(true);
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::draw()
{
    gl::setMatricesWindow(getWindowSize(), true);
    gl::clear(Color::gray(0.5f), false);
    
    gl::color(Color::white());
    drawGrid(getWindowBounds(), 64, Vec2f(0, clock().getTime() * 60));
}
