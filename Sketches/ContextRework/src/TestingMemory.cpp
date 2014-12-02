#include "TestingMemory.h"

#include "chronotext/system/MemoryManager.h"

using namespace std;
using namespace ci;
using namespace chr;

void TestingMemory::setup()
{
    files = getFiles(getPublicDirectory() / "test.bundle"); // CONTAINS A BUNCH OF HUGE PNG IMAGES
    fileIndex = 0;
    done = false;
    
    LOGI << endl << "BEFORE: " << writeMemoryStats() << endl;
}

void TestingMemory::update()
{
    if (!done)
    {
        if (fileIndex < files.size())
        {
            const auto &file = files[fileIndex++];
            
            auto inputSource = InputSource::getFile(file);
            inputSource->setFilePathHint(file.filename().string());
            
            // ---
            
            bool useMipmap;
            TextureRequest::Flags flags;
            
            if (false)
            {
                useMipmap = false;
                flags = TextureRequest::Flags::FLAGS_NONE;
            }
            else
            {
                useMipmap = false;
                flags = TextureRequest::Flags::FLAGS_POT;
            }
            
            TextureRequest textureRequest(inputSource, useMipmap, flags);

            // --
            
            LOGI << endl << writeMemoryStats() << endl;
            textureManager.getTexture(textureRequest);
            LOGI << writeMemoryStats() << endl;
        }
        else
        {
            textureManager.discard();
            done = true;
            
            LOGI << endl << "AFTER: " << writeMemoryStats() << endl;
        }
    }
    else
    {
        LOGI << writeMemoryStats() << endl;
    }
}

string TestingMemory::writeMemoryStats()
{
    auto memoryInfo = memory::getInfo();
    
    stringstream s;
    
    s << "{";
    s << writeMB(memoryInfo.free);

    if (memoryInfo.total > 0)
    {
        s << " | " << writeMB(memoryInfo.total);
    }
    
    if (memoryInfo.used > 0)
    {
        s << " | " << writeMB(memoryInfo.used);
    }
    
    s << "}";
    
    return s.str();
    
}

string TestingMemory::writeMB(int64_t bytes, int precision)
{
    if (bytes < 0)
    {
        return "?";
    }
    
    stringstream s;
    s << fixed << setprecision(precision) << bytes / (1024.0 * 1024.0) << " MB";
    
    return s.str();
}
