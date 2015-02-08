#pragma once

#include "chronotext/utils/Utils.h"

class TestingBase
{
public:
    virtual ~TestingBase() {}
    
    virtual void setup() {}
    virtual void shutdown() {}
    
    virtual void run(bool force = false) {}
    virtual void update() {}
    
    virtual void addTouch(int index, float x, float y) {}
    virtual void updateTouch(int index, float x, float y) {}
    virtual void removeTouch(int index, float x, float y) {}
    
    virtual bool keyDown(const ci::app::KeyEvent &keyEvent) { return false; }
    virtual bool keyUp(const ci::app::KeyEvent &keyEvent) { return false; }
    
    // ---
    
    static ci::fs::path getPublicDirectory()
    {
#if defined(CINDER_ANDROID)
        return chr::FileHelper::getExternalDataPath();
#else
        return ci::getDocumentsDirectory();
#endif
    }
    
    static void dumpDirectory(const ci::fs::path &directoryPath)
    {
        if (ci::fs::exists(directoryPath) && ci::fs::is_directory(directoryPath))
        {
            for (ci::fs::directory_iterator current(directoryPath), end; current != end; ++current)
            {
                if (ci::fs::is_regular_file(current->status()))
                {
                    auto relativePath = chr::FileHelper::relativizePath(directoryPath, current->path());
                    LOGI << relativePath.string() << std::endl;
                }
            }
        }
    }

    static std::vector<ci::fs::path> getFiles(const ci::fs::path &directoryPath)
    {
        std::vector<ci::fs::path> files;
        
        if (ci::fs::exists(directoryPath) && ci::fs::is_directory(directoryPath))
        {
            for (ci::fs::directory_iterator current(directoryPath), end; current != end; ++current)
            {
                if (ci::fs::is_regular_file(current->status()))
                {
                    files.push_back(current->path());
                }
            }
        }
        
        return files;
    }
};
