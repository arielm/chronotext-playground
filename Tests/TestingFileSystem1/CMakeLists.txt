cmake_minimum_required (VERSION 3.2.2)

project(TestingFileSystem1)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem iostreams)

include_directories(
  ${Boost_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
  main.cpp
  "resources/test.txt"
)

if (PLATFORM MATCHES emscripten)
  em_link_pre_js(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/pre.js")

elseif (PLATFORM MATCHES ios)
# source_group(resources FILES "resources/test.txt")
# set_target_properties(${PROJECT_NAME} PROPERTIES RESOURCE "resources/test.txt")
  set_source_files_properties("resources/test.txt" PROPERTIES MACOSX_PACKAGE_LOCATION ${PROJECT_NAME}.app)
endif()

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
)

enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND run)
