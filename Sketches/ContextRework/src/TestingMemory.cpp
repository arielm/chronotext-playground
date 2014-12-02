#include "TestingMemory.h"

#include "chronotext/system/MemoryManager.h"

using namespace std;
using namespace ci;
using namespace chr;

void TestingMemory::setup()
{
    files = getFiles(getPublicDirectory() / "test.bundle");
    fileIndex = 0;
    done = false;
    
    LOGI << endl << "BEFORE: " << endl;
    dumpMemoryStats();
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
            
            textureManager.getTexture(inputSource);
            dumpMemoryStats();
        }
        else
        {
            textureManager.discard();
            done = true;
            
            LOGI << endl << "AFTER: " << endl;
            dumpMemoryStats();
        }
    }
    else
    {
        dumpMemoryStats();
    }
}

void TestingMemory::dumpMemoryStats()
{
    auto memoryInfo = memory::getInfo();
    
    LOGI << toMB(memoryInfo.free) << " | " << toMB(memoryInfo.total) << " | " << toMB(memoryInfo.used) << endl << endl;
}

string TestingMemory::toMB(int64_t bytes, int precision)
{
    if (bytes < 0)
    {
        return "?";
    }
    
    stringstream s;
    s << fixed << setprecision(precision) << bytes / (1024.0 * 1024.0) << " MB";
    
    return s.str();
}
