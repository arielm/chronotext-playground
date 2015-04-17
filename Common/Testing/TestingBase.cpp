/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingBase.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

map<string, bool> TestingBase::registry;

void TestingBase::lock(const string &value)
{
    registry[value] = true;
}

void TestingBase::unlock(const string &value)
{
    registry[value] = false;
}

bool TestingBase::isLocked(const string &value)
{
    auto found = registry.find(value);
    
    if (found != registry.end())
    {
        return found->second;
    }
    
    return false;
}

// ---

fs::path TestingBase::getPublicDirectory()
{
#if defined(CINDER_ANDROID)
    return FileHelper::getExternalDataPath();
#else
    return getDocumentsDirectory();
#endif
}

void TestingBase::dumpDirectory(const fs::path &directoryPath)
{
    if (fs::exists(directoryPath) && fs::is_directory(directoryPath))
    {
        for (fs::directory_iterator current(directoryPath), end; current != end; ++current)
        {
            if (fs::is_regular_file(current->status()))
            {
                auto relativePath = FileHelper::relativizePath(directoryPath, current->path());
                LOGI << relativePath.string() << endl;
            }
        }
    }
}

vector<fs::path> TestingBase::getFiles(const fs::path &directoryPath)
{
    vector<fs::path> files;
    
    if (fs::exists(directoryPath) && fs::is_directory(directoryPath))
    {
        for (fs::directory_iterator current(directoryPath), end; current != end; ++current)
        {
            if (fs::is_regular_file(current->status()))
            {
                files.push_back(current->path());
            }
        }
    }
    
    return files;
}