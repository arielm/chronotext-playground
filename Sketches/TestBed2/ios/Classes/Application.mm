/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#import "Application.h"

#include "chronotext/Context.h"

@implementation Application

- (BOOL) application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    bridge = [[CinderBridge alloc] init];

    // ---
    
    /*
     * "SPECIAL" REQUIREMENT: USING ANTI-ALIASING, ONLY FOR IPAD-2
     */
    
    GLKViewDrawableMultisample multisample = GLKViewDrawableMultisampleNone;
    
    /*
     * CONSTRAINT: SYSTEM-INFO DEPENDS ON CinderBridge INTIALIZATION
     */
    if (chr::getSystemInfo().generation == chr::SystemInfo::GENERATION_IPAD_2)
    {
        multisample = GLKViewDrawableMultisample4X;
    }
    
    // ---
    
    /*
     * THIS WILL AFFECT THE FORTHCOMING GL-CONTEXT
     */
    bridge.viewControllerProperties = [NSDictionary dictionaryWithObjectsAndKeys:
      [NSNumber numberWithInt:kEAGLRenderingAPIOpenGLES1], kGLViewControllerPropertyRenderingAPI,
      [NSNumber numberWithInt:UIInterfaceOrientationMaskLandscape], kGLViewControllerPropertyInterfaceOrientationMask,
      [NSNumber numberWithInt:GLKViewDrawableDepthFormatNone], kGLViewControllerPropertyDepthFormat,
      [NSNumber numberWithInt:multisample], kGLViewControllerPropertyMultisample,
      nil];
    
    window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    window.backgroundColor = [UIColor blackColor];
    
    [window setRootViewController:bridge.viewController];
    [window makeKeyAndVisible];
    
    return YES;
}

- (void) dealloc
{
    [window release];
    [bridge release];
    
    [super dealloc];
}

// ---

/*
 * SEE THE PARALLEL ANDROID VERSION FOR MORE DETAILS REGARDING THE 2 FOLLOWING CALLBACKS:
 * https://github.com/arielm/chronotext-playground/blob/demo/Sketches/TestBed2/android/src/org/chronotext/TestBed2/MainActivity.java#L52-142
 *
 * THE SITUATION IS SIMPLIER ON IOS WHERE THE "APP" AND THE "SKETCH"
 * LIVE BOTH ON THE SAME THREAD AND "LANGUAGE BARRIERS" ARE NOT AN ISSUE
 *
 * STILL: THE SAME CONSTRAINTS APPLY (E.G. AVOIDING "DIRECT COMMUNICATION" BETWEEN THE TWO SIDES),
 * IN ORDER TO PROVIDE A SINGLE AND CONSISTENT API
 */

- (void) handleEvent:(int)eventId
{
    switch (eventId)
    {
        case SKETCH_DID_INIT:
        {
//          [Mixpanel sharedInstanceWithToken:MIXPANEL_TOKEN];
            break;
        }
            
        case SKETCH_DID_UNINIT:
        {
            /*
             * XXX: NEVER ACTUALLY CALLED ON IOS
             *
             * EVEN HAS BEEN "TERMINATED FOR REAL", E.G.
             * - USER HAS WIPED IT FROM THE LIST OF BACKGROUND APPS
             * - SYSTEM HAS JETISONIZED IT IN ORDER TO RECLAIM RESOURCES
             */
            break;
        }
    }
}

- (void) handleMessage:(int)what body:(NSString*)body
{
    switch (what)
    {}
}

@end
