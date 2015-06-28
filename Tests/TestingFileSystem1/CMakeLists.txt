cmake_minimum_required(VERSION 2.8)

project(TestingFileSystem1)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem iostreams)

include_directories(
  ${Boost_INCLUDE_DIRS}
)

add_executable(TestingFileSystem1 main.cpp)

if (EMSCRIPTEN)
  em_link_pre_js(TestingFileSystem1 "${CMAKE_CURRENT_LIST_DIR}/pre.js")
endif()

target_link_libraries(TestingFileSystem1
  ${Boost_LIBRARIES}
)

enable_testing()
add_test(NAME TestingFileSystem1 COMMAND run)
