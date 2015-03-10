/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#pragma once

#include "chronotext/utils/Utils.h"

#include "cinder/app/KeyEvent.h"

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
    
    static std::map<std::string, bool> registry;
    
    static void lock(const std::string &value);
    static void unlock(const std::string &value);
    static bool isLocked(const std::string &value);

    // ---
    
    static ci::fs::path getPublicDirectory();
    static void dumpDirectory(const ci::fs::path &directoryPath);
    static std::vector<ci::fs::path> getFiles(const ci::fs::path &directoryPath);
};
