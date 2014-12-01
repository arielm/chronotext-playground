#include "TestingMemory.h"

#if defined(CINDER_COCOA)

#import <sys/sysctl.h>
#import <mach/host_info.h>
#import <mach/mach_host.h>
#import <mach/task_info.h>
#import <mach/task.h>

#elif defined(CINDER_ANDROID)

#include "chronotext/android/cinder/JNI.h"

#include <fcntl.h>

#endif

using namespace std;
using namespace ci;
using namespace chr;

#if defined(CINDER_ANDROID)

static jlong getFreeMemoryImpl(const char* const sums[], const size_t sumsLen[], size_t num)
{
    int fd = open("/proc/meminfo", O_RDONLY);
    if (fd < 0) {
        LOGI << "Unable to open /proc/meminfo" << endl;
        return -1;
    }
    char buffer[256];
    const int len = read(fd, buffer, sizeof(buffer)-1);
    close(fd);
    if (len < 0) {
        LOGI << "Unable to read /proc/meminfo" << endl;
        return -1;
    }
    buffer[len] = 0;
    size_t numFound = 0;
    jlong mem = 0;
    char* p = buffer;
    while (*p && numFound < num) {
        int i = 0;
        while (sums[i]) {
            if (strncmp(p, sums[i], sumsLen[i]) == 0) {
                p += sumsLen[i];
                while (*p == ' ') p++;
                char* num = p;
                while (*p >= '0' && *p <= '9') p++;
                if (*p != 0) {
                    *p = 0;
                    p++;
                    if (*p == 0) p--;
                }
                mem += atoll(num) * 1024;
                numFound++;
                break;
            }
            i++;
        }
        p++;
    }
    return numFound > 0 ? mem : -1;
}

static jlong getFreeMemory()
{
    static const char* const sums[] = { "MemFree:", "Cached:", NULL };
    static const size_t sumsLen[] = { strlen("MemFree:"), strlen("Cached:"), 0 };
    return getFreeMemoryImpl(sums, sumsLen, 2);
}

static jlong getTotalMemory()
{
    static const char* const sums[] = { "MemTotal:", NULL };
    static const size_t sumsLen[] = { strlen("MemTotal:"), 0 };
    return getFreeMemoryImpl(sums, sumsLen, 1);
}

#endif

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
    
    uint64_t freeMemory = vmstat.free_count * pagesize;
    
#if defined(CINDER_COCOA_TOUCH)
    uint64_t totalMemory = (vmstat.wire_count + vmstat.active_count + vmstat.inactive_count + vmstat.free_count) * pagesize + info.resident_size;
#else
    uint64_t totalMemory = (vmstat.wire_count + vmstat.active_count + vmstat.inactive_count + vmstat.free_count) * pagesize;
#endif
    
    LOGI << toMB(freeMemory) << " | " << toMB(totalMemory) << endl << endl;
    
#elif defined(CINDER_ANDROID)

    /*
     * SOURCE: http://stackoverflow.com/a/18894037/50335
     *
     * ADDITIONAL REFERENCES:
     *
     * - https://android.googlesource.com/platform/frameworks/base/+/master/core/jni/android_util_Process.cpp
     * - http://grepcode.com/file/repository.grepcode.com/java/ext/com.google.android/android/4.1.1_r1/com/android/server/am/ActivityManagerService.java#5404
     * - http://grepcode.com/file/repository.grepcode.com/java/ext/com.google.android/android/4.1.1_r1/com/android/server/am/ProcessList.java#ProcessList.getMemLevel%28int%29
     */
    
    LOGI << toMB(getFreeMemory()) << " | " << toMB(getTotalMemory()) << endl << endl;
    
#endif
}

string TestingMemory::toMB(uint64_t bytes, int precision)
{
    stringstream s;
    s << fixed << setprecision(precision) << bytes / (1024.0 * 1024.0) << " MB";
    return s.str();
}
