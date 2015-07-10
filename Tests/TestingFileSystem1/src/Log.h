#pragma once

#include "Platform.h"

#include <sstream>
#include <iostream>

#if defined(CHR_FS_APK)
#  include <android/log.h>
#endif

namespace chr
{
  static constexpr int FATAL   = -3;
  static constexpr int ERROR   = -2;
  static constexpr int WARNING = -1;
  static constexpr int INFO    =  0;
  static constexpr int DEBUG   =  1;
  static constexpr int VERBOSE =  2;

  class Log
  {
  public:
    Log(int level = INFO)
    :
    level(level)
    {}

    ~Log()
    {
#if defined(CHR_FS_APK)
      int androidPriority;

      switch (level)
      {
        case FATAL:
          androidPriority = ANDROID_LOG_FATAL;
          break;

        case ERROR:
          androidPriority = ANDROID_LOG_ERROR;
          break;

        case WARNING:
          androidPriority = ANDROID_LOG_WARN;
          break;

        default:
        case INFO:
          androidPriority = ANDROID_LOG_INFO;
          break;

        case DEBUG:
          androidPriority = ANDROID_LOG_DEBUG;
          break;
      }

      __android_log_write(androidPriority, "CHR", ss.str().data());
#else
      if (level < 0)
      {
        std::cerr << ss.rdbuf();
      }
      else
      {
        std::cout << ss.rdbuf() << std::flush;  
      }
#endif

      if (level == FATAL)
      {
        abort();
      }
    }

    std::stringstream& stream()
    {
      return ss;
    }

  protected:
    int level;
    std::stringstream ss;
  };
}

#define LOGF chr::Log(chr::FATAL).stream()
#define LOGE chr::Log(chr::ERROR).stream()
#define LOGW chr::Log(chr::WARNING).stream()
#define LOGI chr::Log(chr::INFO).stream()
#define LOGD chr::Log(chr::DEBUG).stream()
