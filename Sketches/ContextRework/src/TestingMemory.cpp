#include "TestingMemory.h"

#include "chronotext/system/MemoryManager.h"

using namespace std;
using namespace ci;
using namespace chr;

void TestingMemory::setup()
{
    /*
     * THE FIRST WILL BE A PNG IMAGE OF OUR OWN
     */
    
    inputSources.emplace_back(InputSource::getAsset("U_512.png")); // 512x512 RGBA (SUPPOSED TO USE 1 MB)

    /*
     * ADDING A BUNCH OF HUGE PNG IMAGES...
     *
     *
     * REQUIRES A FOLDER NAMED "test.bundle":
     *
     * OSX: IN ~/Documents
     * IOS: IN THE APP'S DOCUMENT-FOLDER
     * ANDROID: IN THE DEVICE'S EXTERNAL FOLDER
     *
     *
     * WHY ADDING .bundle TO THE FOLDER NAME?
     * ALLOWS TO EASILY TRANSFER A "NON-FLAT FILE-SYSTEM" TO ANY IOS DEVICE (VIA ITUNES)
     */

    for (const auto &file : getFiles(getPublicDirectory() / "test.bundle"))
    {
        auto inputSource = InputSource::getFile(file);
        inputSource->setFilePathHint(file.filename().string());
        
        inputSources.emplace_back(inputSource);
    }
    
    index = 0;
    done = false;
    
    LOGI << endl << "MEMORY INFO - BEFORE: " << writeMemoryInfo() << endl;
}

void TestingMemory::update()
{
    if (!done)
    {
        if (index < inputSources.size())
        {
            auto inputSource = inputSources[index++];
            
            // ---
            
            bool useMipmap;
            TextureRequest::Flags flags;
            
            if (false)
            {
                useMipmap = false;
                flags = TextureRequest::FLAGS_NONE;
            }
            else
            {
                useMipmap = false;
                flags = TextureRequest::FLAGS_POT;
            }
            
            TextureRequest textureRequest(inputSource, useMipmap, flags);

            // --
            
            LOGI << endl << writeMemoryInfo() << endl;
            textureManager.getTexture(textureRequest);
            LOGI << writeMemoryInfo() << endl;
        }
        else
        {
            textureManager.discard();
            done = true;
            
            LOGI << endl << "MEMORY INFO - AFTER: " << writeMemoryInfo() << endl;
        }
    }
    else if (index > 0)
    {
        LOGI << writeMemoryInfo() << endl;
    }
}

string TestingMemory::writeMemoryInfo(double unit, int precision)
{
    auto memoryInfo = memory::getInfo();

    stringstream s;
    s << fixed << setprecision(precision + 2) << "{";
    
    if (memoryInfo.free > 0)
    {
        s << "free: " << memoryInfo.free / unit;
    }
    
    if (memoryInfo.used > 0)
    {
        s << ", used: " << memoryInfo.used / unit;
    }
    
    if (memoryInfo.ratio > 0)
    {
        s << ", ratio: " << memoryInfo.ratio;
    }
    
    s << "}";
    
    return s.str();
}

string TestingMemory::writeMB(int64_t bytes, int precision)
{
    if (bytes < 0)
    {
        return "";
    }
    
    stringstream s;
    s << fixed << setprecision(precision) << bytes / (1024.0 * 1024.0) << " MB";
    
    return s.str();
}
