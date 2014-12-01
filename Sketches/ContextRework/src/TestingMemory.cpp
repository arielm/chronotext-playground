#include "TestingMemory.h"

#include "chronotext/texture/TextureManager.h"

#if defined(CINDER_COCOA)
#import <sys/sysctl.h>
#import <mach/host_info.h>
#import <mach/mach_host.h>
#import <mach/task_info.h>
#import <mach/task.h>
#endif

using namespace std;
using namespace ci;
using namespace chr;

void TestingMemory::run(bool force)
{
    if (force || true)
    {
        if (force || true) testMemoryLoad1();
    }
}

void TestingMemory::testMemoryLoad1()
{
    auto files = getFiles(getPublicDirectory() / "test.bundle");
    
    LOGI << endl << "BEFORE: " << endl;
    dumpMemoryStats();
    
    {
        TextureManager textureManager;
        
        for (auto &file : files)
        {
            auto inputSource = InputSource::getFile(file);
            inputSource->setFilePathHint(file.filename().string());
            
            textureManager.getTexture(inputSource);
            dumpMemoryStats();
        }
    }
    
    LOGI << endl << "AFTER: " << endl;
    dumpMemoryStats();
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
    
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    vm_statistics_data_t vmstat;
    
    if (host_statistics(mach_host_self (), HOST_VM_INFO, (host_info_t)&vmstat, &count) != KERN_SUCCESS)
    {
        fprintf(stderr, "host_statistics FAILURE");
        return;
    }
    
    double total = vmstat.wire_count + vmstat.active_count + vmstat.inactive_count + vmstat.free_count;
    double wired = vmstat.wire_count / total;
    double active = vmstat.active_count / total;
    double inactive = vmstat.inactive_count / total;
    double free = vmstat.free_count / total;
    
    task_basic_info_64_data_t info;
    unsigned size = sizeof(info);
    task_info(mach_task_self(), TASK_BASIC_INFO_64, (task_info_t)&info, &size);
    
    double unit = 1024 * 1024;
    printf("%3.1f MB | %3.1f MB | %3.1f MB\n\n", vmstat.free_count * pagesize / unit, (vmstat.free_count + vmstat.inactive_count) * pagesize / unit, info.resident_size / unit);
#endif
}
