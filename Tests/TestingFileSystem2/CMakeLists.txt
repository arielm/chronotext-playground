cmake_minimum_required (VERSION 3.2.2)

project(TestingFileSystem2)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

get_filename_component(CHR_COMMON "${CMAKE_CURRENT_LIST_DIR}/../common" REALPATH)

include_directories(
  "${CHR_COMMON}/src"
  ${Boost_INCLUDE_DIRS}
)

set(SRC_FILES
  src/main.cpp
  "${CHR_COMMON}/src/Log.cpp"
  "${CHR_COMMON}/src/Platform.cpp"
)

file(GLOB_RECURSE RESOURCES_FILES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  res/[^.]*
)

if (PLATFORM MATCHES mxe)
  add_definitions(-DCHR_FS_RC)

  set(DATA_RC "")
  set(DATA_CPP "")
  set(counter 128)

  foreach (resource_file ${RESOURCES_FILES})
    set(DATA_RC "${DATA_RC}${counter} RCDATA \"../../${resource_file}\"\r\n")
    set(DATA_CPP "{\"${resource_file}\", ${counter}}, ${DATA_CPP}")
    math(EXPR counter "${counter} + 1")
  endforeach()

  configure_file("${CHR_COMMON}/cmake/mxe/resources.rc.in" resources.rc)
  configure_file("${CHR_COMMON}/cmake/mxe/resources.cpp.in" resources.cpp)

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    "${CMAKE_CURRENT_BINARY_DIR}/resources.rc"
    "${CMAKE_CURRENT_BINARY_DIR}/resources.cpp"
  )

elseif (PLATFORM MATCHES android)
  add_definitions(-DCHR_FS_APK)

  configure_file("${CHR_COMMON}/cmake/android/AndroidManifest.xml.in" AndroidManifest.xml)
  configure_file("${CHR_COMMON}/cmake/android/ant.properties.in" ant.properties)
  configure_file("${CHR_COMMON}/cmake/android/MainActivity.java.in" "src/org/chronotext/${PROJECT_NAME}/MainActivity.java")

  configure_file("${CHR_COMMON}/cmake/android/tests/AndroidManifest.xml.in" tests/AndroidManifest.xml)
  configure_file("${CHR_COMMON}/cmake/android/tests/ant.properties.in" tests/ant.properties)
  configure_file("${CHR_COMMON}/cmake/android/tests/MainActivityTests.java.in" "tests/src/org/chronotext/${PROJECT_NAME}/MainActivityTests.java")

  configure_file("${CHR_COMMON}/cmake/android/install.instrument.sh.in" install.sh)
  configure_file("${CHR_COMMON}/cmake/android/run.instrument.sh.in" run.sh)

  set(LIBRARY_OUTPUT_PATH "libs/armeabi-v7a")

  add_library(${PROJECT_NAME} SHARED
    ${SRC_FILES}
    "${CHR_COMMON}/src/Bridge.cpp"
  )

elseif (PLATFORM MATCHES emscripten)
  add_definitions(-DCHR_FS_JS_EMBED)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --exclude-file *.DS_Store --embed-file ../../res")
  
  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )

elseif (PLATFORM MATCHES ios)
  add_definitions(-DCHR_FS_BUNDLE)

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${RESOURCES_FILES}
  )

  foreach (resource_file ${RESOURCES_FILES})
    get_filename_component(parent_dir ${resource_file} DIRECTORY)
    set_source_files_properties(${resource_file} PROPERTIES MACOSX_PACKAGE_LOCATION ${PROJECT_NAME}.app/${parent_dir})
  endforeach()

else()
  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )
endif()

# ---

if (PLATFORM MATCHES osx)
  configure_file("${CHR_COMMON}/cmake/install.symlink.sh.in" install.sh)
endif()

# ---

if (PLATFORM MATCHES ios|osx)
  target_link_libraries(${PROJECT_NAME}
    "-framework Foundation"
    ${Boost_LIBRARIES}
  )
elseif (PLATFORM MATCHES android)
  target_link_libraries(${PROJECT_NAME}
    log
    android
    ${Boost_LIBRARIES}
  )
else()
  target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
  )
endif()

# ---

enable_testing()

# add_test(NAME ${PROJECT_NAME} COMMAND ./run.sh)
  add_test(NAME ${PROJECT_NAME} COMMAND ./run.sh "--gtest_filter=MessageTest.*")
