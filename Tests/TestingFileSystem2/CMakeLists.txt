cmake_minimum_required (VERSION 3.2.2)

project(TestingFileSystem2)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

set(INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
)

set(LIBRARIES
  ${Boost_LIBRARIES}
)

set(SRC_FILES
  src/main.cpp
)

get_filename_component(CHR_PATH "${CMAKE_CURRENT_LIST_DIR}/../common" REALPATH)
include("${CHR_PATH}/cmake/test.complex.cmake")

add_test(NAME AllTests COMMAND ./run.sh)
#add_test(NAME MessageTest  COMMAND ./run.sh --gtest_filter=MessageTest.*)
#add_test(NAME PrintersTest COMMAND ./run.sh --gtest_filter=Print*Test.*)
