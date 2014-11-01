#include "Sketch.h"

#include "chronotext/Log.h"
#include "chronotext/InputSource.h"
#include "chronotext/system/SystemManager.h"
#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup()
{
    /*
     * TESTING NEW LOGGING MECHANISM
     */

    LOG << TAG("CRITICAL") << "warning: " << 999 << endl;
    
    /*
     * TESTING NEW EXCEPTION MECHANISM
     */
    
    try
    {
        InputSource::loadResource("undefined.png");
    }
    catch (EXCEPTION_TYPE(InputSource) &e)
    {
        LOGI << e << endl;
    }
    catch (exception &e)
    {
        LOGI << e.what() << endl;
    }
    
    /*
     * TESTING VARIOUS INFO STRUCTURES
     */
    
    LOGI << "SYSTEM INFO: " << SystemManager::getSystemInfo() << endl; // TODO: USE THE FORTHCOMING chr::Context INSTEAD OF SINGLETON PATTERNS
    LOGI << "DISPLAY INFO: " << getDisplayInfo() << endl;
    LOGI << "WINDOW INFO: " << getWindowInfo() << endl;
    
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
    drawGrid(getWindowBounds(), 64, Vec2f(0, getElapsedSeconds() * 60));
}
