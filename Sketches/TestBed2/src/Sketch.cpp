/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "Sketch.h"

#if defined(TARGET1)
#   include "TestingSound1.h"
#   include "TestingSound2.h"
#endif

#if defined(TARGET2)
#   include "TestingNetwork.h"
#endif

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
    
#if defined(TARGET1) && defined(TARGET2)
    gl::clear(Color(1, 0, 1), false);
#elif defined(TARGET1)
    gl::clear(Color(1, 0, 0), false);
#elif defined(TARGET2)
    gl::clear(Color(0, 0, 1), false);
#endif
    
    gl::color(Color::white());
    utils::gl::drawGrid(getWindowBounds(), 64, Vec2f(0, clock()->getTime() * 60));
}

void Sketch::begin()
{
#if defined(TARGET1)
    addFrameTest<TestingSound1>(false);
    addFrameTest<TestingSound2>(true);
#endif
    
#if defined(TARGET2)
    addFrameTest<TestingNetwork>(true);
#endif
}
