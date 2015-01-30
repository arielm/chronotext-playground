#include "TestingMisc.h"

#include "chronotext/Context.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/incubator/utils/FileCapture.h"

using namespace std;
using namespace ci;
using namespace chr;

void TestingMisc::run(bool force)
{
    if (force || true)
    {
        if (force || false) testSharedPtrCasting();
        if (force || false) testFileCapture();
    }

    if (force || true)
    {
        if (force || false) testFileSystem();
        if (force || false) testSystemAndMemoryInfo();
    }

    if (force || true)
    {
        if (force || false) testNewLogging();
        if (force || false) testNewException();
        if (force || false) testInputSourceRobustness();
        if (force || false) testTimeFormat();
        if (force || false) testDurationFormat();
        if (force || true) testStringToIntToString();
    }
}

// ---

class ResourceItem
{
public:
    ResourceItem(shared_ptr<void> resource)
    :
    resource(resource)
    {}
    
    template <class T>
    inline std::shared_ptr<T> getResource()
    {
        if (this && resource)
        {
            return std::static_pointer_cast<T>(resource);
        }
        
        return nullptr;
    }
    
protected:
    shared_ptr<void> resource;
};

void TestingMisc::testSharedPtrCasting()
{
    TextureManager::LOG_VERBOSE = true;
    
    {
        TextureManager textureManager;
        
        auto texture = textureManager.getTexture(InputSource::getAsset("U_512.png"));
        LOGI << texture.use_count() << endl;
        
        auto resource = make_shared<ResourceItem>(texture);
        LOGI << texture.use_count() << endl;
        
        LOGI << resource->getResource<Texture>()->getSize() << endl;
        
        resource.reset();
        LOGI << texture.use_count() << endl;
    }
    
    TextureManager::LOG_VERBOSE = false;
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

void TestingMisc::testTimeFormat()
{
    LOGI << utils::format::time(255) << endl;
    LOGI << utils::format::time(4943) << endl;
    LOGI << utils::format::time(60) << endl;
    LOGI << utils::format::time(0.6, true) << endl;
}

void TestingMisc::testDurationFormat()
{
    LOGI << utils::format::duration(1.23456) << endl;
    LOGI << utils::format::duration(0.0123456) << endl;
    LOGI << utils::format::duration(0.000123456) << endl;
    LOGI << utils::format::duration(0.00000123456) << endl;
    LOGI << utils::format::duration(0.0000000123456) << endl;
    LOGI << utils::format::duration(0.000000000123456) << endl;
    LOGI << utils::format::duration(60) << endl;
}

// ---

/*
 * DEMONSTRATED:
 *
 * 1) "ROBUST" STRING-TO-INT CONVERSION:
 *    - ci::fromString<int>("2b") WOULD THROW (RELIES ON boost::lexical_cast BEHINDS THE SCENES)
 *    - std::stoi() WOULD BE THE PREFERRED (C++11) SOLUTION, BUT:
 *      - IT'S NOT SUPPORTED ON ANDROID (DETAILS FOLLOWING)
 *      - ALTERNATIVE: THE "OLD" std::atoi(), WHICH TAKES A const char* INSTEAD OF A std::string
 *
 * 2) INT-TO-STRING CONVERSION:
 *    - std::to_string() WOULD BE THE PREFERRED (C++11) SOLUTION, BUT:
 *      - IT'S NOT SUPPORTED ON ANDROID (DETAILS FOLLOWING)
 *      - ALTERNATIVE: ci::toString(), RELIES ON boost::lexical_cast BEHINDS THE SCENES
 *
 *
 * WHY STANDARD C++11 FEATURES LIKE stoi() AND to_string() ARE NOT SUPPORTED (AS OF NDK 10c) ON ANDROID?
 *
 * 1) DETAILS: http://stackoverflow.com/questions/17950814/how-to-use-stdstoul-and-stdstoull-in-android/18124627#18124627
 *
 * 2) SOLUTION TRIED: USING CRYSTAX NDK 10
 *    - https://www.crystax.net/en/android/ndk
 *    - stoi() AND to_string() DO WORK AS INTENDED, BUT:
 *      - boost::filesystem::path IS BROKEN, DUE TO std::use_facet, ETC.
 *        - DETAILS: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=51813#c9
 *        - RECOMPILING BOOST FOR ANDROID COULD HELP, BUT IT'S A HELL:
 *          - TRIED: https://github.com/MysticTreeGames/Boost-for-Android
 *            - NO SUCCESS
 *            - IN ANY-CASE, THIS STUFF IS OUTDATED
 *              AND IT DOESN'T SEEM TO COMPILE EFFICIENT CODE:
 *              - ONLY HANDLING ARM-V5 ARCHITECTURE, ETC.
 *        - ALTERNATIVES:
 *          - TRYING TO USE CRYSTAX'S "BOOST FOR ANDROID" BUILD:
 *            - https://github.com/crystax/android-platform-ndk/blob/master/build/tools/build-boost.sh
 *            - CONS:
 *              - NO BINARIES CURRENTLY AVAILABLE:
 *                - BUILDING IS DEFINITELY NOT-TRIVIAL
 *              - BASED ON BOOST 1.57:
 *                - WE CURRENTLY USE 1.53
 *                - 1.55 IS KNOWN TO "WORK AS INTENDED" WITH CINDER
 *                - 1.57 REMAINS A MYSTERY
 *              - DEPENDS ON ICU-LIB:
 *                - IT IS SUPPOSED TO SOLVE ISSUES RELATED TO "LOCALE"
 *                - WE'RE NOT NECESSARILY INTERESTED AT THIS STAGE
 *                - AND WE ALREADY HAVE OUR VERSION OF ICU (USED BY THE ZFONT SYSTEM)
 *            - TODO:
 *              - WAIT FOR THE NEXT RELEASE OF CRYSTAX, IN THE HOPE FOR SOME BINARIES
 *
 */

void TestingMisc::testStringToIntToString()
{
    auto releaseString = "3.5.2b";
    auto components = ci::split(releaseString, '.');

#if defined(CINDER_ANDROID)
    
    int major = (components.size() > 0) ? std::atoi(components[0].data()) : 0;
    int minor = (components.size() > 1) ? std::atoi(components[1].data()) : 0;
    int patch = (components.size() > 2) ? std::atoi(components[2].data()) : 0;
    
    LOGI << ci::toString(major) << "." << ci::toString(minor) << "." << ci::toString(patch) << endl;
    
#else
    
    int major = (components.size() > 0) ? std::stoi(components[0]) : 0;
    int minor = (components.size() > 1) ? std::stoi(components[1]) : 0;
    int patch = (components.size() > 2) ? std::stoi(components[2]) : 0;
    
    LOGI << std::to_string(major) << "." << std::to_string(minor) << "." << std::to_string(patch) << endl;
    
#endif
}