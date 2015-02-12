#include "chronotext/Context.h"

#include "Sketch.h"

namespace chr
{
    CinderSketch* createSketch()
    {
        return new Sketch();
    }
}

#pragma mark ----------------------------------------   IOS   ----------------------------------------

#if defined(CINDER_COCOA_TOUCH)

#include "Application.h"

int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([Application class]));
    }
}

#pragma mark ---------------------------------------- ANDROID ----------------------------------------

#elif defined(CINDER_ANDROID)

extern "C"
{}

#pragma mark ---------------------------------------- DESKTOP ----------------------------------------

#elif defined(CINDER_MAC)

#include "Application.h"

CINDER_APP_NATIVE(Application, ci::app::RendererGl(ci::app::RendererGl::AA_NONE))

#endif
