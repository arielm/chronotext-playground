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
    
    // ---
    
    static bool fail(const std::string &file = "", int line = 0, const std::string &reason = "");
    static void logFailure(chr::Log& log, const std::string &file, int line, const std::string &reason);
};

#define CHR_TEST_BEGIN(TITLE) LOGI << std::endl << TITLE << std::endl;
#define CHR_TEST_END() LOGI << std::endl;
#define CHR_TEST(COND, FN) if ((COND)) { CHR_TEST_BEGIN(#FN); FN(); CHR_TEST_END(); }

#define CHR_CHECK(COND, ...) (COND) ? true : TestingBase::fail(__FILE__, __LINE__, ##__VA_ARGS__)
