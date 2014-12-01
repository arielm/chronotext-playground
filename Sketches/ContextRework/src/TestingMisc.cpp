#include "TestingMisc.h"

#include "chronotext/InputSource.h"
#include "chronotext/utils/FileCapture.h"
#include "chronotext/system/SystemManager.h"

using namespace std;
using namespace ci;
using namespace chr;

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
        if (force || false) testSystemInfo();
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

void TestingMisc::testSystemInfo()
{
    LOGI << "SYSTEM INFO: " << SystemManager::getSystemInfo() << endl; // TODO: USE THE FORTHCOMING chr::context INSTEAD OF SUCH SINGLETON PATTERNS
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
     * NEW LOGGING SYSTEM IS CURRENTLY ON HOLD...
     */
    
    LOG << "nothing special for now" << ' ' << std::hex << 255 << std::dec << endl;
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
