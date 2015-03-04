/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * STUB FOR RUNNING A CinderSketch ON iOS
 */

#pragma once

#import "CinderBridge.h"

@interface Application : NSObject <UIApplicationDelegate>
{
    CinderBridge *bridge;
    UIWindow *window;
}

@end
