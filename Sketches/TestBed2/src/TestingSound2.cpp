/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingSound2.h"

#include "chronotext/Context.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

void TestingSound2::setup()
{
    if (isLocked("FMOD"))
    {
        throw EXCEPTION(TestingSound2, "FMOD IS ALREADY IN USE");
    }
    else
    {
        lock("FMOD");
    }
}

void TestingSound2::shutdown()
{
    unlock("FMOD");
}

void TestingSound2::update()
{
}

void TestingSound2::addTouch(int index, float x, float y)
{
}

void TestingSound2::removeTouch(int index, float x, float y)
{
}
