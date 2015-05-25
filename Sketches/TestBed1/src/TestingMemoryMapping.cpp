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

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::zf;

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
                
                // ---
                
                auto data2 = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
                
                if (data2 != MAP_FAILED)
                {
                    LOGI << std::addressof(data2) << endl;
                    munmap(data2, size);
                }
                
                munmap(data1, size);
            }
            
            close(fd);
        }
    }
}
