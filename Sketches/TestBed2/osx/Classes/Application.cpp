/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "Application.h"

using namespace std;
using namespace ci;
using namespace chr;

void Application::applySettings(Settings *settings)
{
    if (loadEmulators(InputSource::getResource("emulators.json")))
    {
        emulate(settings, "IPHONE_3GS", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "IPHONE_4", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "IPHONE_5", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "IPAD_1", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "GALAXY_S", chr::DisplayInfo::ORIENTATION_PORTRAIT);
//      emulate(settings, "GALAXY_S4", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
//      emulate(settings, "NEXUS_7_2012", chr::DisplayInfo::ORIENTATION_LANDSCAPE);
    }
}
