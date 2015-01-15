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
