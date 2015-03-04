/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestTask.h"

#include "chronotext/Log.h"

using namespace std;
using namespace chr;

TestTask::TestTask(const string &message, float delay1, float delay2)
:
message(message),
delay1(delay1),
delay2(delay2)
{}

void TestTask::run()
{
    sleep(delay1);
    LOGI << message << endl;
    sleep(delay2);
}
