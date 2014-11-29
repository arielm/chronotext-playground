#include "Sketch.h"

#include "chronotext/Log.h"
#include "chronotext/InputSource.h"
#include "chronotext/utils/GLUtils.h"

#include "chronotext/utils/FileCapture.h"
#include "chronotext/system/SystemManager.h"

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
     * TESTING NEW FILE-CAPTURE SYSTEM
     *
     * TODO: ENHANCED API, E.G.
     *
     * FileCapture capture(stdout).setTrimTrailing(true);
     * printf("foo\n");
     * if (capture == "foo") cerr << capture << endl;
     */
    
    {
        FileCapture capture(stdout);
        printf("hello from stdout");
        cerr << "[" << capture.flushAsString() << "]" << endl;
    }
    
    {
        FileCapture capture(stderr);
        fprintf(stderr, "hello from stderr\n");
        cout << "[" << capture.flushAsString(true) << "]" << endl;
    }
    
    // ---
    
    /*
     * TESTING NEW LOGGING SYSTEM (CURRENTLY ON HOLD...)
     */

    LOG << "nothing special for now" << ' ' << std::hex << 255 << std::dec << endl;
    
    // ---
    
    /*
     * TESTING NEW EXCEPTION SYSTEM
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
    
    // ---
    
    /*
     * TESTING VARIOUS INFO STRUCTURES
     */
    
    LOGI << "SYSTEM INFO: " << SystemManager::getSystemInfo() << endl; // TODO: USE THE FORTHCOMING chr::context INSTEAD OF SUCH SINGLETON PATTERNS
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
