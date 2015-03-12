/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

/*
 * TODO:
 *
 * 1) ADD PAUSE/RESUME CAPABILITIES TO TestingBase AND THEN MAKE USE OF SoundManager::pause()/resume()
 *
 * 2) TEST SoundManager WHEN INITIALIZED WITH "FMOD_INIT_THREAD_UNSAFE"
 */

#include "TestingSound2.h"

#include "chronotext/Context.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

void TestingSound2::setup()
{
    if (isLocked("FMOD"))
    {
        throw EXCEPTION(TestingSound2, "FMOD IS ALREADY IN USE");
    }
    else
    {
        lock("FMOD");
    }
    
    assert(!soundManager);
    soundManager = make_shared<SoundManager>();
    
    if (!soundManager->init())
    {
        throw EXCEPTION(SoundManager, "UNABLE TO INITIALIZE SoundManager");
    }
    
    // ---
    
    createSound(InputSource::getResource("53466__dobroide__random-spanish-words.wav"));
    channel = soundManager->playSound(sound);
}

void TestingSound2::shutdown()
{
    destroySound();
    
    soundManager->uninit();
    soundManager.reset();
    
    unlock("FMOD");
}

void TestingSound2::update()
{
    soundManager->update();
    
    if (channel)
    {
        bool playing;
        channel->isPlaying(&playing);
        
        if (!playing)
        {
            channel = nullptr;
        }
    }
}

void TestingSound2::addTouch(int index, float x, float y)
{
    if (!paused)
    {
        paused = true;
        pausedIndex = index;
        
        if (channel)
        {
            channel->setPaused(true);
        }
    }
}

void TestingSound2::removeTouch(int index, float x, float y)
{
    if (paused && (index == pausedIndex))
    {
        paused = false;
        pausedIndex = -1;
        
        if (channel)
        {
            channel->setPaused(false);
        }
    }
}

void TestingSound2::createSound(InputSource::Ref source)
{
    if (source->hasFileName())
    {
        FMOD_CREATESOUNDEXINFO exinfo;
        memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
        exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
        
#if defined(ANDROID)
        
        exinfo.fileuseropen = &android_open;
        exinfo.fileuserclose = &android_close;
        exinfo.fileuserread = &android_read;
        exinfo.fileuserseek =  &android_seek;
        
        exinfo.fileuserdata = this; // NOT CURRENTLY USED
        
#endif

        FMOD_RESULT result = soundManager->system->createStream(source->getFilePath().c_str(), FMOD_DEFAULT, &exinfo, &sound);
        
        if (result)
        {
            throw EXCEPTION(SoundManager, "FMOD: UNABLE TO CREATE FILE-STREAM | REASON: " + SoundManager::writeError(result));
        }
    }
    else
    {
        throw EXCEPTION(SoundManager, "FMOD: FILE-STREAMING IS NOT SUPPORTED ON THIS PLATFORM");
    }
}

void TestingSound2::destroySound()
{
    if (channel)
    {
        channel->stop();
        channel = nullptr;
    }

    if (sound)
    {
        sound->release();
        sound = nullptr;
    }
}

#if defined(ANDROID)

FMOD_RESULT TestingSound2::android_open(const char *name, unsigned int *filesize, void **handle, void *userdata)
{
    AAsset* asset = AAssetManager_open(FileHelper::getAssetManager(), name, AASSET_MODE_UNKNOWN);
    
    if (!asset)
    {
        return FMOD_ERR_FILE_NOTFOUND;
    }
    
    *filesize = AAsset_getLength(asset);
    *handle = asset;
    
    return FMOD_OK;
}

FMOD_RESULT TestingSound2::android_close(void *handle, void *userdata)
{
    if (!handle)
    {
        return FMOD_ERR_INVALID_PARAM;
    }
    
    AAsset_close((AAsset*)handle);
    
    return FMOD_OK;
}

FMOD_RESULT TestingSound2::android_read(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata)
{
    if (!handle)
    {
        return FMOD_ERR_INVALID_PARAM;
    }
    
    if (bytesread)
    {
        auto read = AAsset_read((AAsset*)handle, buffer, sizebytes);
        
        if (read < 0)
        {
            return FMOD_ERR_FILE_BAD;
        }
        else if (read < sizebytes)
        {
            return FMOD_ERR_FILE_EOF;
        }
        
        *bytesread = read;
    }
    
    return FMOD_OK;
}

FMOD_RESULT TestingSound2::android_seek(void *handle, unsigned int pos, void *userdata)
{
    if (!handle)
    {
        return FMOD_ERR_INVALID_PARAM;
    }
    
    AAsset_seek((AAsset*)handle, pos, SEEK_SET);
    
    return FMOD_OK;
}

#endif
