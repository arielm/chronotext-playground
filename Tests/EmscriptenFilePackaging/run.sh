#!/bin/sh

rm -rf build
mkdir -p build
cd build

emcc ../main.cpp \
  -Wno-warn-absolute-paths -std=c++11 \
  -I${BOOST_PATH}/dist/emscripten/include \
  -L${BOOST_PATH}/dist/emscripten/lib -lboost_system -lboost_filesystem -lboost_iostreams

if [ $? != 0 ]; then
  echo "BUILD FAILED!"
  exit 1
fi

js a.out.js "$@"
