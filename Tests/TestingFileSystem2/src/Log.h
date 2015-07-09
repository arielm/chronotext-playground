#pragma once

#include "Platform.h"

#include <sstream>
#include <iostream>

#if defined(CHR_PLATFORM_ANDROID)
#  include <android/log.h>
#endif

namespace chr
{
  class Log
  {
  public:
    enum Level
    {
      ERROR,
      WARN,
      INFO,
      DEBUG 
    };

    Log(Level level = Level::INFO)
    :
    level(level)
    {}

    ~Log()
    {
#if defined(CHR_PLATFORM_ANDROID)
      __android_log_write(ANDROID_LOG_INFO, "CHR", ss.str().data());
#else
      std::cout << ss.rdbuf() << std::flush;
#endif
    }

    std::stringstream& stream()
    {
      return ss;
    }

  protected:
    Level level;
    std::stringstream ss;
  };
}

#define LOGI chr::Log(chr::Log::Level::INFO).stream()
#define LOGD chr::Log(chr::Log::Level::DEBUG).stream()
