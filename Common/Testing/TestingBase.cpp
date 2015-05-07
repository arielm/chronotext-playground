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
    for (const auto &path : getFiles(directoryPath))
    {
        LOGI << FileHelper::relativizePath(directoryPath, path).string() << endl;
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

// ---

bool TestingBase::fail(const string &file, int line, const string &reason)
{
    LOGI_FN(logFailure, file, line, reason);
    return false;
}

void TestingBase::logFailure(Log& log, const std::string &file, int line, const std::string &reason)
{
    log << "FAILURE";
    
    if (!reason.empty())
    {
        log << ": " << reason;
    }
    
    if (!file.empty())
    {
        log << " [" << file; // TODO: POSSIBILITY TO EXCLUDE FULL-PATH (RELATIVE TO SOME PRE-DEFINABLE "ROOT" PATH)
        
        if (line > 0)
        {
            log << ":" << line;
        }
        
        log << "]";
    }
    
    log << endl;
}
