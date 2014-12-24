#include "TestingMisc.h"

#include "chronotext/Context.h"
#include "chronotext/utils/FileCapture.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace context;

void TestingMisc::run(bool force)
{
    if (force || false)
    {
        if (force || true) testFileCapture();
        if (force || true) testNewLogging();
        if (force || true) testNewException();
    }
    
    if (force || true)
    {
        if (force || false) testSystemAndMemoryInfo();
        if (force || false) testFileSystem();
    }
}

// ---

/*
 * ACCESSING HIERARCHICAL FILE-SYSTEM
 *
 * OSX: ANY PATH IS VALID
 * IOS: A COCOA "BUNDLE FOLDER" MUST BE USED (AND COPIED TO THE DEVICE VIA ITUNES-FILE-SHARING)
 * ANDROID: THE "EXTERNAL FOLDER" CAN BE USED
 */

void TestingMisc::testFileSystem()
{
    dumpDirectory(getPublicDirectory() / "test.bundle");
}

void TestingMisc::testSystemAndMemoryInfo()
{
    LOGI << "SYSTEM INFO: " << getSystemInfo() << endl;
    LOGI << "MEMORY INFO: " << getMemoryInfo() << endl;
}

// ---

void TestingMisc::testFileCapture()
{
    /*
     * TODO: ENHANCED API, E.G.
     *
     * FileCapture capture(stdout).setTrimTrailing(true);
     * printf("foo\n");
     * if (capture == "foo") cerr << capture << endl;
     */
    
    {
        FileCapture capture(stdout);
        printf("hello from stdout");
        cerr << "[" << capture.flushAsString() << "]" << endl;
    }
    
    {
        FileCapture capture(stderr);
        fprintf(stderr, "hello from stderr\n");
        cout << "[" << capture.flushAsString(true) << "]" << endl;
    }
}

void TestingMisc::testNewLogging()
{
    /*
     * TODO: DEMONSTRATE "THREAD-BUFFERING" CAPABILITIES
     */
    
    LOGI << "thread-buffered and iomanip-aware" << ' ' << std::hex << 255 << std::dec << endl;
}

void TestingMisc::testNewException()
{
    try
    {
        InputSource::loadResource("undefined.png");
    }
    catch (EXCEPTION_TYPE(InputSource) &e)
    {
        LOGI << e << endl;
    }
    catch (exception &e)
    {
        LOGI << e.what() << endl;
    }
}
