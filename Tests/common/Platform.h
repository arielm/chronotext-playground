#pragma once

#include <vector>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace chr
{
  enum
  {
    PLATFORM_OSX,
    PLATFORM_IOS,
    PLATFORM_IOS_SIM,
    PLATFORM_ANDROID,
    PLATFORM_EMSCRIPTEN,
    PLATFORM_MINGW,
  };
}

#if defined(__APPLE__)
#  include <TargetConditionals.h>
#  define CHR_PLATFORM_COCOA
#  if TARGET_OS_IPHONE
#    if TARGET_IPHONE_SIMULATOR
#      define CHR_PLATFORM chr::PLATFORM_IOS_SIM
#      define CHR_PLATFORM_IOS_SIM 1
#    else
#      define CHR_PLATFORM chr::PLATFORM_IOS
#      define CHR_PLATFORM_IOS 1
#    endif // TARGET_IPHONE_SIMULATOR
#  else
#    define CHR_PLATFORM chr::PLATFORM_OSX
#    define CHR_PLATFORM_OSX 1
#  endif // TARGET_OS_IPHONE
#elif defined(__ANDROID__)
#  define CHR_PLATFORM chr::PLATFORM_ANDROID
#  define CHR_PLATFORM_ANDROID 1
#elif defined(__EMSCRIPTEN__)
#  define CHR_PLATFORM chr::PLATFORM_EMSCRIPTEN
#  define CHR_PLATFORM_EMSCRIPTEN 1
#elif defined(__MINGW32__)
#  define CHR_PLATFORM chr::PLATFORM_MINGW
#  define CHR_PLATFORM_MINGW 1
#else
#  error UNSUPPORTED PLATFORM
#endif // __APPLE__

#if defined(CHR_PLATFORM_MINGW)
# include <windows.h>
# undef ERROR // SEE https://google-glog.googlecode.com/svn/trunk/doc/glog.html#windows
#elif defined(CHR_PLATFORM_COCOA)
# include <CoreFoundation/CoreFoundation.h>
#endif

#if defined(CHR_FS_APK)
# include <android/asset_manager_jni.h>

  namespace chr
  {
    namespace android
    {
      extern JavaVM *vm;
      extern jobject activity;

      extern AAssetManager *assetManager;
      extern std::string internalDataPath;
      extern std::string externalDataPath;
      extern std::string apkPath;
    }
  }
#endif

namespace chr
{
  bool hasFileResources()
  {
#if defined(CHR_FS_APK) || defined(CHR_FS_RC)
    return false;
#else
    return true;
#endif
  }

  fs::path getResourcePath(const fs::path &relativePath)
  {
    fs::path basePath;

#if defined(CHR_FS_APK) || defined(CHR_FS_RC)
    return "";
#elif defined(CHR_FS_BUNDLE)
    CFBundleRef bundle = CFBundleGetMainBundle();
    CFURLRef url = CFBundleCopyBundleURL(bundle);
    CFStringRef tmp = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);

    CFIndex length = CFStringGetLength(tmp);
    CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8);
    char *buffer = (char*)malloc(maxSize);
    CFStringGetCString(tmp, buffer, maxSize, kCFStringEncodingUTF8);
              
    basePath = buffer; // FIXME: ONLY WORKS FOR "FLAT-PACKAGED-FILES" INSIDE iOS APP
              
    CFRelease(url);
    CFRelease(tmp);
    free(buffer);
#elif defined(CHR_PLATFORM_ANDROID)
   static char buf[PATH_MAX];
   auto len = readlink("/proc/self/exe", buf, PATH_MAX - 1);
   assert(len > 0);
   basePath = fs::path(std::string(buf, len)).parent_path();
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
   basePath = "resources";
#else
   basePath = fs::current_path() / "resources";
#endif

   return basePath / relativePath;
  }

#if defined(CHR_FS_APK)
  std::string toString(JNIEnv *env, jstring s)
  {
    std::string result;

    if (s)
    {
      const char *chars = env->GetStringUTFChars(s, nullptr);
                
      if (chars)
      {
        result.assign(chars);
        env->ReleaseStringUTFChars(s, chars);
      }
    }

    return result;
  }

  std::vector<std::string> toStrings(JNIEnv *env, jobjectArray a)
  {
    std::vector<std::string> result;

    auto size = env->GetArrayLength(a);
    result.reserve(size);

    for (auto i = 0; i < size; i++)
    {
      result.emplace_back(chr::toString(env, (jstring)env->GetObjectArrayElement(a, i)));
    }

    return result;
  }
#endif

#if defined(CHR_FS_RC)
  int checkResource(int resId)
  {
	  HRSRC infoHandle = ::FindResource(NULL, MAKEINTRESOURCE(resId), RT_RCDATA);

	  if (infoHandle)
	  {
      return ::SizeofResource(NULL, infoHandle);
	  }

   return ::GetLastError();
  }
#elif defined(CHR_FS_APK)
  int checkResource(const fs::path &fileName)
  {
    AAsset *asset = AAssetManager_open(android::assetManager, fileName.c_str(), AASSET_MODE_UNKNOWN);

    if (asset)
    {
      auto size = AAsset_getLength(asset);
      AAsset_close(asset);

      return size;
    }

    return -1;
  }
#endif
}
