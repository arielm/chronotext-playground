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
 * 1) ADD PAUSE/RESUME CAPABILITIES TO TestingBase AND THEN MAKE USE OF SoundManager::pause()/release()
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
        throw EXCEPTION(TestingSound2, "SoundManager CAN'T BE INITIALIZED");
    }
    
    // ---
    
    if (initSound(InputSource::getResource("53466__dobroide__random-spanish-words.wav")))
    {
        soundManager->system->playSound(sound, soundManager->masterGroup, true, &channel);
        
        channel->setVolume(1);
        channel->setPaused(false);
    }
}

void TestingSound2::shutdown()
{
    uninitSound();
    
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
            soundManager->update();
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

bool TestingSound2::initSound(InputSource::Ref source)
{
    FMOD_CREATESOUNDEXINFO exinfo;
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    
    string name;
    
    if (source->isFile())
    {
        name = source->getFilePath().string();
    }
    else if ((source->getType() == InputSource::TYPE_RESOURCE) || (source->getType() == InputSource::TYPE_ASSET))
    {
#if defined(ANDROID)
        name = source->getFilePathHint();
        
        exinfo.fileuseropen = &android_open;
        exinfo.fileuserclose = &android_close;
        exinfo.fileuserread = &android_read;
        exinfo.fileuserseek =  &android_seek;
        exinfo.fileuserdata = this;
#else
        throw EXCEPTION(SoundManager, "PLATFORM DOES NOT SUPPORT FILE-STREAMING");
#endif
    }
    
    FMOD_RESULT result = soundManager->system->createStream(name.data(), FMOD_DEFAULT, &exinfo, &sound);
    
    if (result)
    {
        LOGI << "UNABLE TO LOAD SOUND | REASON: " << FMOD_ErrorString(result) << endl;
        return false;
    }
    
    return true;
}

void TestingSound2::uninitSound()
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
