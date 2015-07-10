#!/bin/sh

rm -rf build
mkdir -p build
cd build

emcc ../main.cpp \
  -Wno-warn-absolute-paths -std=c++11 \
  -I${BOOST_PATH}/dist/emscripten/include \
  -L${BOOST_PATH}/dist/emscripten/lib -lboost_system -lboost_filesystem \
  --exclude-file *.DS_Store --embed-file ../resources

if [ $? != 0 ]; then
  echo "BUILD FAILED!"
  exit 1
fi

node a.out.js "$@"
