#include "cinder/Cinder.h"

#if defined(CINDER_COCOA_TOUCH)

#include "AppDelegate.h"

int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}

#elif defined(CINDER_ANDROID)

#include "chronotext/android/cinder/CinderRendererJNI.h"

#include "Sketch.h"

extern "C"
{
	void android_main(struct android_app *state) {}
}

chr::CinderDelegate* createDelegate()
{
    chr::CinderDelegate *delegate = new chr::CinderDelegate();
    delegate->sketch = new Sketch(delegate, delegate);
    
    return delegate;
}

#else

#include "Application.h"

CINDER_APP_NATIVE(Application, ci::app::RendererGl(ci::app::RendererGl::AA_NONE))

#endif
