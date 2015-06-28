cmake_minimum_required(VERSION 2.8)

project(TestingFileSystem1)

add_executable(TestingFileSystem1 main.cpp)

if (EMSCRIPTEN)
  em_link_pre_js(TestingFileSystem1 "${CMAKE_CURRENT_LIST_DIR}/pre.js")
  em_link_post_js(TestingFileSystem1 "${CMAKE_CURRENT_LIST_DIR}/post.js")
endif()

enable_testing()
add_test(NAME TestingFileSystem1 COMMAND run)
