#!/bin/sh

setup()
{
  android update project --path . --name TestBed2 --target android-15

  echo "READY TO RUN:"
  echo "ndk-build -j$(sysctl hw.ncpu | awk '{print $2}')"
  echo "ant debug"
  echo "adb install -r bin/TestBed2-debug.apk"
}

if [ -z $CINDER_PATH ]
  then echo "CINDER_PATH MUST BE DEFINED!"
else
  setup
fi
