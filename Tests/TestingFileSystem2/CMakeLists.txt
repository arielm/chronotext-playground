cmake_minimum_required (VERSION 3.2.2)

project(TestingFileSystem2)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

include_directories(
  ../common
  ${Boost_INCLUDE_DIRS}
)

set(SRC_FILES
  src/main.cpp
)

if (PLATFORM MATCHES mxe)
  add_definitions(-DCHR_FS_RC)

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    mxe/resources.rc
  )

elseif (PLATFORM MATCHES android)
  add_definitions(-DCHR_FS_APK)

  configure_file(android/AndroidManifest.xml.in AndroidManifest.xml)
  configure_file(android/ant.properties.in ant.properties)
  configure_file(android/MainActivity.java.in "src/org/chronotext/${PROJECT_NAME}/MainActivity.java")

  configure_file(android/tests/AndroidManifest.xml.in tests/AndroidManifest.xml)
  configure_file(android/tests/ant.properties.in tests/ant.properties)
  configure_file(android/tests/MainActivityTests.java.in "tests/src/org/chronotext/${PROJECT_NAME}/MainActivityTests.java")
  configure_file(android/tests/TestRunner.java.in "tests/src/org/chronotext/${PROJECT_NAME}/TestRunner.java")

  configure_file(android/install.sh.in install.sh)
  configure_file(android/test.sh.in test.sh)

  set(LIBRARY_OUTPUT_PATH "libs/armeabi-v7a")

  add_library(${PROJECT_NAME} SHARED
    ${SRC_FILES}
  )

elseif (PLATFORM MATCHES emscripten)
  add_definitions(-DCHR_FS_JS_EMBED)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --exclude-file *.DS_Store --embed-file ../../resources")
  
  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
  )

else()
  file(GLOB RESOURCES_FILES resources/*)

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${RESOURCES_FILES}
  )
endif()

if (PLATFORM MATCHES ios)
  add_definitions(-DCHR_FS_BUNDLE)
  set_source_files_properties(${RESOURCES_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION ${PROJECT_NAME}.app)

elseif (PLATFORM MATCHES android)
  install(CODE "execute_process(COMMAND ./install.sh)")

elseif (PLATFORM MATCHES osx)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND rm -f resources
    COMMAND ln -s \"${CMAKE_CURRENT_LIST_DIR}/resources\"
  )
endif()

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

enable_testing()

if (PLATFORM MATCHES android)
# add_test(NAME ${PROJECT_NAME} COMMAND ./test.sh)
  add_test(NAME ${PROJECT_NAME} COMMAND ./test.sh "--gtest_filter=MessageTest.*")
else()
  add_test(NAME ${PROJECT_NAME} COMMAND run)
endif()
