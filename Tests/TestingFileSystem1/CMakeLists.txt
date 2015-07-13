cmake_minimum_required (VERSION 3.2.2)

project(TestingFileSystem1)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

set(CHR_COMMON "${CMAKE_CURRENT_LIST_DIR}/../common")

include_directories(
  "${CHR_COMMON}/include"
  ${Boost_INCLUDE_DIRS}
)

file(GLOB_RECURSE RESOURCES_FILES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  res/* # TODO: EXCLUDE .DS_Store FILES (NON-TRIVIAL)
)

add_executable(${PROJECT_NAME}
  src/main.cpp
  ${RESOURCES_FILES} # XXX: ONLY NECESSARY FOR iOS
)

if (PLATFORM MATCHES emscripten)
  add_definitions(-DCHR_FS_JS_NODE)
  em_link_pre_js(${PROJECT_NAME} "${CHR_COMMON}/cmake/emscripten/pre.js")

elseif (PLATFORM MATCHES ios)
  add_definitions(-DCHR_FS_BUNDLE)

  foreach(resource_file ${RESOURCES_FILES})
    get_filename_component(parent_dir ${resource_file} DIRECTORY)
    set_source_files_properties(${resource_file} PROPERTIES MACOSX_PACKAGE_LOCATION ${PROJECT_NAME}.app/${parent_dir})
  endforeach()
endif()

# ---

if (PLATFORM MATCHES android)
  configure_file("${CHR_COMMON}/cmake/android/install.push.sh.in" install.sh)

elseif (PLATFORM MATCHES osx|mxe)
  configure_file("${CHR_COMMON}/cmake/install.symlink.sh.in" install.sh)
endif()

# ---

if (PLATFORM MATCHES ios|osx)
  target_link_libraries(${PROJECT_NAME}
    "-framework Foundation"
    ${Boost_LIBRARIES}
  )
else()
  target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
  )
endif()

# ---

enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ./run.sh)
