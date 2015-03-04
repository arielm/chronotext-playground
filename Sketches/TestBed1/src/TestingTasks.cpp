/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingTasks.h"
#include "TestTask.h"

#include "chronotext/Context.h"

using namespace std;
using namespace ci;
using namespace chr;

void TestingTasks::run(bool force)
{
    if (force || true)
    {
        if (force || true) test1();
    }
}

void TestingTasks::test1()
{
    TaskManager::LOG_VERBOSE = true;
    TaskManager::MAX_CONCURRENT_THREADS = 2;
    
    taskManager().addTask(make_shared<TestTask>("two", 1, 0.5));
    taskManager().addTask(make_shared<TestTask>("one", 0.5, 0.75));
    taskManager().addTask(make_shared<TestTask>("three", 0.125, 0.25));
    
    LOGI << "BEFORE ONE" << endl;
    Task::sleep(2);
    LOGI << "BEFORE THREE" << endl;
}
