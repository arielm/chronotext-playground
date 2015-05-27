#!/bin/sh

setup()
{
  rm -f local.properties
  rm -rf bin
  rm -rf gen

  android update project --path . --name TestBed2 --target android-16

  echo "" >> local.properties
  echo "cinder.dir=$CINDER_PATH" >> local.properties
  echo "chr.src.dir=$CINDER_PATH/blocks/new-chronotext-toolkit/android/src" >> local.properties

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
