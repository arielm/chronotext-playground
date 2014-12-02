#include "Sketch.h"

#include "TestingMisc.h"
#include "TestingMemory.h"

#include "chronotext/utils/GLUtils.h"

#if defined(CINDER_ANDROID)

#include "chronotext/android/cinder/JNI.h"
#include "chronotext/android/cinder/CinderDelegate.h"

#endif

using namespace std;
using namespace ci;
using namespace chr;

Sketch::Sketch()
:
CinderSketch()
{}

void Sketch::setup()
{
    if (true)
    {
        LOGI << "DISPLAY INFO: " << getDisplayInfo() << endl;
        LOGI << "WINDOW INFO: " << getWindowInfo() << endl;
    }
    
    TestingBase::execute<TestingMisc>(true);
    
#if defined(CINDER_ANDROID)
    jstring query = (jstring)context::delegate->callObjectMethodOnJavaListener("getMemoryInfo", "()Ljava/lang/String;");
    
    if (query)
    {
        JNIEnv *env = jvm::env();
        
        const char *chars = env->GetStringUTFChars(query, nullptr);
        LOGI << chars << endl;
        env->ReleaseStringUTFChars(query, chars);
    }
#else
    if (true)
    {
        frameTest = make_shared<TestingMemory>();
        frameTest->setup();
    }
#endif
    
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
    drawGrid(getWindowBounds(), 64, Vec2f(0, clock().getTime() * 60));
}
