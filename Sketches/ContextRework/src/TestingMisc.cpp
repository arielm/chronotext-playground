#include "TestingMisc.h"

#include "chronotext/Context.h"
#include "chronotext/utils/FileCapture.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace context;

void TestingMisc::run(bool force)
{
    if (force || true)
    {
        if (force || false) testFileCapture();
        if (force || false) testNewLogging();
        if (force || false) testNewException();
        if (force || true) testInputSourceRobustness();
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
    catch (chr::Exception<InputSource> &e)
    {
        LOGI << e << endl; // I.E. SOMETHING WENT-WRONG WHILE LOADING
    }
    catch (exception &e)
    {
        LOGI << e.what() << endl; // E.G. SOMETHING WENT-WRONG WHILE DECODING THE IMAGE
    }
}

void TestingMisc::testInputSourceRobustness()
{
    InputSource::Ref nonInitialized;

    /*
     * WITHOUT SOME "EXTRA-CARE" IN THE IMPLEMENTATION, THE FOLLOWING WOULD (BADLY) CRASH:
     */

    /*
     * INSTEAD: IT "ONLY" RETURNS A "DEFAULT-VALUE" (E.G. false)
     */
    nonInitialized->isFile();
    nonInitialized->getFilePath();
    nonInitialized->getFilePathHint();
    nonInitialized->setFilePathHint("bar");
    nonInitialized->getURI();

    /*
     * INSTEAD: IT "ONLY" THROWS
     */
    if (true)
    {
        nonInitialized->loadDataSource();
    }
    else
    {
        nonInitialized->getSubSource("foo");
    }
}
