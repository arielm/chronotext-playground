/*
 * STUB FOR RUNNING A CinderSketch ON THE DESKTOP
 */

#pragma once

#include "chronotext/app/cinder/CinderDelegate.h"

class Application : public chr::CinderDelegate
{
public:
    void applySettings(Settings *settings) final;
};
