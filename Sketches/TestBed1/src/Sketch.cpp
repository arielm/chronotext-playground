/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "Sketch.h"

#include "TestingDemangling.h"
#include "TestingMisc.h"
#include "TestingMemory1.h"
#include "TestingMemory2.h"
#include "TestingTasks.h"
#include "TestingZFont.h"
#include "TestingStringKey.h"
#include "TestingIOService.h"
#include "TestingMemoryMapping.h"
#include "TestingIO.h"

#include "chronotext/Context.h"
#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

void Sketch::setup()
{
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
    utils::gl::drawGrid(getWindowBounds(), 64, Vec2f(0, clock()->getTime() * 60));
}

void Sketch::begin()
{
    executeTest<TestingDemangling>(false);
    executeTest<TestingMisc>(false);
    executeTest<TestingTasks>(false);
    executeTest<TestingZFont>(false);
    executeTest<TestingStringKey>(false);
    executeTest<TestingMemoryMapping>(true);
    executeTest<TestingIO>(false);
    
    addFrameTest<TestingMemory1>(false);
    addFrameTest<TestingMemory2>(false);
    addFrameTest<TestingIOService>(false);
}
