/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * FEATURES:
 *
 * 1) UNLIKE TestingSound1: THIS TEST FAILS WHENEVER FMOD IS NOT PROPERLY INITIALIZED
 *
 * 2) FILE-STREAMING:
 *    - INCLUDING FROM ANDROID-ASSETS
 *
 * 3) PROPER PAUSING/RESUMING OF PLAYING-SOUND
 */


#pragma once

#include "Testing/TestingBase.h"

#include "chronotext/sound/SoundManager.h"

class TestingSound2 : public TestingBase
{
public:
    void setup() final;
    void shutdown() final;
    
    /*
     * PASSING VIA update() IS NECESSARY WHEN WORKING WITH FMOD
     */
    void update() final;
    
    void addTouch(int index, float x, float y) final;
    void removeTouch(int index, float x, float y) final;
    
protected:
    std::shared_ptr<chr::SoundManager> soundManager;
    
    FMOD::Sound* sound = nullptr;
    FMOD::Channel *channel = nullptr;
    
    bool paused = false;
    int pausedIndex = -1;
    
    void createSound(chr::InputSource::Ref source); // CAN THROW
    void destroySound();
    
#if defined(CINDER_ANDROID)
    
    static FMOD_RESULT F_CALLBACK android_open(const char *name, unsigned int *filesize, void **handle, void *userdata);
    static FMOD_RESULT F_CALLBACK android_close(void *handle, void *userdata);
    static FMOD_RESULT F_CALLBACK android_read(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata);
    static FMOD_RESULT F_CALLBACK android_seek(void *handle, unsigned int pos, void *userdata);
    
#endif
};
