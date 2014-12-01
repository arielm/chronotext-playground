#include "TestingMemory.h"

#if defined(CINDER_COCOA)

#import <sys/sysctl.h>
#import <mach/host_info.h>
#import <mach/mach_host.h>
#import <mach/task_info.h>
#import <mach/task.h>

#elif defined(CINDER_ANDROID)

#include "chronotext/android/cinder/JNI.h"

#endif

using namespace std;
using namespace ci;
using namespace chr;

void TestingMemory::setup()
{
    files = getFiles(getPublicDirectory() / "test.bundle");
    fileIndex = 0;
    done = false;
    
    LOGI << endl << "BEFORE: " << endl;
    dumpMemoryStats();
}

void TestingMemory::update()
{
    if (!done)
    {
        if (fileIndex < files.size())
        {
            const auto &file = files[fileIndex++];
            
            auto inputSource = InputSource::getFile(file);
            inputSource->setFilePathHint(file.filename().string());
            
            textureManager.getTexture(inputSource);
            dumpMemoryStats();
        }
        else
        {
            done = true;
            textureManager.discard();
            
            LOGI << endl << "AFTER: " << endl;
            dumpMemoryStats();
        }
    }
}

void TestingMemory::dumpMemoryStats()
{
#if defined(CINDER_COCOA)
    
    /*
     * SOURCE: http://stackoverflow.com/a/6095158/50335
     */
    
    int mib[6];
    mib[0] = CTL_HW;
    mib[1] = HW_PAGESIZE;
    
    int pagesize;
    size_t length = sizeof(pagesize);
    
    if (sysctl(mib, 2, &pagesize, &length, NULL, 0) < 0)
    {
        fprintf(stderr, "sysctl FAILURE");
        return;
    }

    vm_statistics_data_t vmstat;
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    
    if (host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &count) != KERN_SUCCESS)
    {
        fprintf(stderr, "host_statistics FAILURE");
        return;
    }
    
//  double total = vmstat.wire_count + vmstat.active_count + vmstat.inactive_count + vmstat.free_count;
//  double wired = vmstat.wire_count / total;
//  double active = vmstat.active_count / total;
//  double inactive = vmstat.inactive_count / total;
//  double free = vmstat.free_count / total;
    
    task_basic_info_64_data_t info;
    unsigned size = sizeof(info);
    task_info(mach_task_self(), TASK_BASIC_INFO_64, (task_info_t)&info, &size);
    
    LOGI << prettyBytes(vmstat.free_count * pagesize) << " | " << prettyBytes((vmstat.free_count + vmstat.inactive_count) * pagesize) << " | " << prettyBytes(info.resident_size) << endl << endl;
    
#elif defined(CINDER_ANDROID)
    
    /*
     * XXX: THE FOLLOWING IS USELESS, SINCE IT IS NOT AFFECTED BY TEXTURE-MEMORY
     */
    
    JNIEnv *env = java::getJNIEnv();
    
    jclass debugClass = env->FindClass("android/os/Debug");

    jmethodID getNativeHeapSizeMethod = env->GetStaticMethodID(debugClass, "getNativeHeapSize", "()J");
    jlong heapSize = env->CallStaticLongMethod(debugClass, getNativeHeapSizeMethod);

    jmethodID getNativeHeapFreeSizeMethod = env->GetStaticMethodID(debugClass, "getNativeHeapFreeSize", "()J");
    jlong heapFreeSize = env->CallStaticLongMethod(debugClass, getNativeHeapFreeSizeMethod);
    
    jmethodID getNativeHeapAllocatedSizeMethod = env->GetStaticMethodID(debugClass, "getNativeHeapAllocatedSize", "()J");
    jlong heapAllocatedSize = env->CallStaticLongMethod(debugClass, getNativeHeapAllocatedSizeMethod);
    
    LOGI << prettyBytes(heapSize) << " | " << prettyBytes(heapFreeSize) << " | " << prettyBytes(heapAllocatedSize) << endl << endl;
    
#endif
}
