/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingMemoryMapping.h"

#include "chronotext/font/zf/FontManager.h"
#include "chronotext/Context.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <boost/iostreams/device/file_descriptor.hpp>

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::zf;

namespace io = boost::iostreams;

void TestingMemoryMapping::setup()
{
    FontManager::LOG_VERBOSE = true;
    FontManager::PROBE_MEMORY = true;
    
    LOGI << endl << "MEMORY INFO - BEFORE: " << getMemoryInfo() << endl << endl;
}

void TestingMemoryMapping::shutdown()
{
    FontManager::LOG_VERBOSE = false;
    FontManager::PROBE_MEMORY = false;
    
    LOGI << endl << "MEMORY INFO - AFTER: " << getMemoryInfo() << endl;
}

void TestingMemoryMapping::run(bool force)
{
    if (force || true)
    {
        CHR_TEST(force || true, testSameFontAllocation);
        CHR_TEST(force || true, testSameFDAllocation);
    }
}

// ---

/*
 * TESTING system/MemoryBuffer (USED BEHIND THE SCENES WHEN ACCESSING DroidSansFallback.ttf):
 *
 * RESULTS FOR OSX, iOS AND ANDROID:
 * - A NEW MEMORY AREA IS MAPPED (OR ALLOCATED, IN THE CASE OF COMPRESSED ANDROID ASSETS) EACH TIME
 * 
 * CONCLUSION:
 * - SOME CACHING SHOULD TAKE PLACE (I.E. AT THE system/MemoryManager LEVEL)
 */
void TestingMemoryMapping::testSameFontAllocation()
{
    auto fontManager = make_shared<FontManager>();
    
    fontManager->getFont(InputSource::getAsset("droid-sans-fallback.xml"), ZFont::Properties2d(32));
    fontManager->getFont(InputSource::getAsset("droid-sans-fallback.xml"), ZFont::Properties2d(64));
}

// ---

vector<string> execute(const string &command)
{
    vector<string> lines;
    
#if defined(CINDER_MAC)
    auto pipe = popen(command.data(), "r");
    
    if (pipe)
    {
        io::file_descriptor_source source(fileno(pipe), io::close_handle);
        io::stream<io::file_descriptor_source> stream(source);
        
        string line;
        
        while (getline(stream, line))
        {
            lines.emplace_back(line);
        }
        
        pclose(pipe);
    }
#endif
    
    return lines;
}

/*
 * POSSIBLE (PARTIAL) OUTPUT OF THE lsof COMMAND ON OSX:
 *
 *    COMMAND    PID   USER   FD     TYPE             DEVICE  SIZE/OFF    NODE NAME
 * 1  TestBed1 44262 arielm  cwd      DIR                1,4       102 3597717 /Users/.../Build/Products/Debug
 * 2  TestBed1 45159 arielm  txt      REG                1,4   1308237 1705086 /Users/.../test.bundle/background.png
 * 3  TestBed1 44262 arielm  txt      REG                1,4  14376960  637827 /System/Library/Extensions/.../MacOS/AppleIntelHD5000GraphicsGLDriver
 * 4  TestBed1 44262 arielm  txt      REG                1,4    622896  713123 /usr/lib/dyld
 * 5  TestBed1 44262 arielm  txt      REG                1,4 383804959  958833 /private/var/db/dyld/dyld_shared_cache_x86_64h
 * 6  TestBed1 44262 arielm    0r     CHR               16,7       0t0    1083 /dev/ttys007
 * 7  TestBed1 44262 arielm    1w     CHR               16,7     0t692    1083 /dev/ttys007
 * 8  TestBed1 44262 arielm    2u     CHR               16,7       0t0    1083 /dev/ttys007
 * 9  TestBed1 45159 arielm   15r     REG                1,4   1308237 1705086 /Users/.../test.bundle/background.png
 *
 * "FD" VALUES FOR THE background.png ENTRIES:
 * txt: I.E. THE FILE IS MEMORY-MAPPED
 * 15r: I.E. THE FILE IS OPENED IN READ-MODE
 */
vector<string> filter(const fs::path &filePath)
{
    vector<string> result;
    
#if defined(CINDER_MAC)
    auto lines = execute("lsof -p " + ci::toString(getpid()));
    
    for (const auto &line : lines)
    {
        if (boost::ends_with(line, filePath.string()))
        {
            result.emplace_back(line);
        }
    }
#endif
    
    return result;
}

/*
 * TESTING CONSECUTIVE MEMORY-MAPPING WITH UNIQUE FILE-DESCRIPTOR
 *
 * RESULTS FOR OSX, iOS AND ANDROID:
 * - A NEW MEMORY AREA IS MAPPED EACH TIME
 *
 * CONCLUSION:
 * - SOME CACHING SHOULD TAKE PLACE (I.E. AT THE system/MemoryManager LEVEL)
 */
void TestingMemoryMapping::testSameFDAllocation()
{
    auto filePath = getPublicDirectory() / "test.bundle/background.png"; // 1.3MB
    auto fd = open(filePath.c_str(), O_RDONLY);
    
    if (fd != -1)
    {
        struct stat stats;
        
        if ((fstat(fd, &stats) != -1) && (stats.st_size > 0))
        {
            size_t size = stats.st_size;
            auto data1 = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
            
            if (data1 != MAP_FAILED)
            {
                LOGI << std::addressof(data1) << endl;
             
                /*
                 * MAPPED-FILE APPEARS TWICE, WITH THE FOLLOWING "FD" VALUES:
                 * txt
                 * 15r
                 */
                LOGI << filter(filePath).size() << endl;

                // ---
                
                auto data2 = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
                
                if (data2 != MAP_FAILED)
                {
                    /*
                     * data2 IS DIFFERENT FROM data1
                     */
                    LOGI << std::addressof(data2) << endl;
                    
                    /*
                     * MAPPED-FILE APPEARS TWICE...
                     */
                    LOGI << filter(filePath).size() << endl;
                    
                    munmap(data2, size);
                }

                /*
                 * MAPPED-FILE APPEARS TWICE...
                 */
                LOGI << filter(filePath).size() << endl;
                
                munmap(data1, size);
            }
            
            /*
             * MAPPED-FILE APPEARS ONCE ("FD" IS 15r)
             */
            LOGI << filter(filePath).size() << endl;

            close(fd);
        }
    }
}
