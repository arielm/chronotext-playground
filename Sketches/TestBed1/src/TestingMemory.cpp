#include "TestingMemory.h"

#include "chronotext/Context.h"

using namespace std;
using namespace ci;
using namespace chr;

void TestingMemory::setup()
{
    TextureManager::LOG_VERBOSE = true;
    TextureManager::PROBE_MEMORY = true;
    
    textureManager = make_shared<TextureManager>();

    // ---
    
    /*
     * THE FIRST WILL BE A PNG IMAGE OF OUR OWN
     */
    
    inputSources.emplace_back(InputSource::getAsset("U_512.png")); // 512x512 RGBA (SUPPOSED TO USE 1 MB)

    /*
     * ADDING A BUNCH OF HUGE PNG IMAGES...
     *
     *
     * REQUIRES A FOLDER NAMED "test.bundle":
     *
     * OSX: IN ~/Documents
     * IOS: IN THE APP'S DOCUMENT-FOLDER
     * ANDROID: IN THE DEVICE'S EXTERNAL FOLDER
     *
     *
     * WHY ADDING .bundle TO THE FOLDER NAME?
     * ALLOWS TO EASILY TRANSFER A "NON-FLAT FILE-SYSTEM" TO IOS DEVICES (VIA ITUNES)
     */

    for (const auto &file : getFiles(getPublicDirectory() / "test.bundle"))
    {
        auto inputSource = InputSource::getFile(file);
        inputSource->setFilePathHint(file.filename().string());
        
        inputSources.emplace_back(inputSource);
    }
    
    index = 0;
    done = false;
    
    LOGI << endl << "MEMORY INFO - BEFORE: " << getMemoryInfo() << endl;
}

void TestingMemory::shutdown()
{
    textureManager.reset();
    
    TextureManager::LOG_VERBOSE = false;
    TextureManager::PROBE_MEMORY = false;
}

void TestingMemory::update()
{
    if (!done)
    {
        auto size = true ? inputSources.size() : 10; // XXX
        size = std::min(size, inputSources.size());
        
        if (index < size)
        {
            auto inputSource = inputSources[index++];
            
            // ---
            
            bool useMipmap;
            Texture::Request::Flags flags;
            
            if (false)
            {
                useMipmap = false;
                flags = Texture::Request::FLAGS_NONE;
            }
            else
            {
                useMipmap = false;
                flags = Texture::Request::FLAGS_POT;
            }
            
            Texture::Request textureRequest(inputSource, useMipmap, flags);

            // --
            
//          LOGI << endl << getMemoryInfo() << endl;
            
            try
            {
                textureManager->getTexture(textureRequest);
            }
            catch (exception &e)
            {
                LOGI << e.what() << endl;
            }
            
//          LOGI << getMemoryInfo() << endl;
        }
        else
        {
            textureManager->discardTextures();
            done = true;
            
            LOGI << endl << "MEMORY INFO - AFTER: " << getMemoryInfo() << endl;
        }
    }
    else if (index > 0)
    {
        if (false)
        {
            LOGI << getMemoryInfo() << endl;
        }
   }
}
