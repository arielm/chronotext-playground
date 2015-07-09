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
#  include <windows.h>
#  undef ERROR // SEE https://google-glog.googlecode.com/svn/trunk/doc/glog.html#windows
#elif defined(CHR_PLATFORM_ANDROID)
#  include <android/asset_manager_jni.h>
#endif

#if defined(CHR_PLATFORM_ANDROID)
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
  fs::path getExecutablePath(int argc, const char *argv[])
  {
    switch (CHR_PLATFORM)
    {
      case PLATFORM_IOS:
      case PLATFORM_ANDROID:
        if (argc > 0)
        {
          return fs::path(std::string(argv[0])).parent_path();
        }

      default:
        return fs::current_path();
    }
  }

  fs::path getResourcePath(const fs::path &executablePath, const fs::path &fileName)
  {
    switch (CHR_PLATFORM)
    {
      case PLATFORM_IOS:
      case PLATFORM_ANDROID:
        return executablePath / fileName;

      case PLATFORM_EMSCRIPTEN:
        return fs::path("resources") / fileName;

      default:
        return executablePath / "resources" / fileName;
    }
  }

#if defined(CHR_PLATFORM_ANDROID)
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

#if defined(CHR_PLATFORM_MINGW)
  int checkResource(int resId)
  {
	  HRSRC infoHandle = ::FindResource(NULL, MAKEINTRESOURCE(resId), RT_RCDATA);

	  if (infoHandle)
	  {
      return ::SizeofResource(NULL, infoHandle);
	  }

   return ::GetLastError();
  }
#elif defined(CHR_PLATFORM_ANDROID)
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
