#include "TestingMemory.h"

#include "chronotext/system/Context.h"

using namespace std;
using namespace ci;
using namespace chr;

void TestingMemory::setup()
{
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
    
    LOGI << endl << "MEMORY INFO - BEFORE: " << memory::getInfo() << endl;
}

void TestingMemory::update()
{
    if (!done)
    {
        if (index < inputSources.size())
        {
            auto inputSource = inputSources[index++];
            
            // ---
            
            bool useMipmap;
            TextureRequest::Flags flags;
            
            if (false)
            {
                useMipmap = false;
                flags = TextureRequest::FLAGS_NONE;
            }
            else
            {
                useMipmap = false;
                flags = TextureRequest::FLAGS_POT;
            }
            
            TextureRequest textureRequest(inputSource, useMipmap, flags);

            // --
            
            LOGI << endl << memory::getInfo() << endl;
            textureManager.getTexture(textureRequest);
            LOGI << memory::getInfo() << endl;
        }
        else
        {
            textureManager.discard();
            done = true;
            
            LOGI << endl << "MEMORY INFO - AFTER: " << memory::getInfo() << endl;
        }
    }
    else if (index > 0)
    {
        LOGI << memory::getInfo() << endl;
    }
}
