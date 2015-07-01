#pragma once

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

namespace chr
{
  fs::path getExecutablePath(int argc, char *argv[])
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

      default:
        return executablePath / "resources" / fileName;
    }
  }
}
