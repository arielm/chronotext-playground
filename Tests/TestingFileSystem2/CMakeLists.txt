cmake_minimum_required (VERSION 3.2.2)

project(TestingFileSystem2)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

include_directories(
  ${Boost_INCLUDE_DIRS}
)

set(SRC_FILES
  src/main.cpp
)

if (PLATFORM MATCHES mxe)
  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    mxe/resources.rc
  )

elseif (PLATFORM MATCHES android)
  configure_file(android/AndroidManifest.xml.in AndroidManifest.xml)
  configure_file(android/ant.properties.in ant.properties)
  configure_file(android/MainActivity.java.in "src/org/chronotext/${PROJECT_NAME}/MainActivity.java")

  set(LIBRARY_OUTPUT_PATH "libs/armeabi-v7a")

  add_library(${PROJECT_NAME} SHARED
    ${SRC_FILES}
  )

else()
  file(GLOB RESOURCES_FILES resources/*)

  add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${RESOURCES_FILES}
  )
endif()

if (PLATFORM MATCHES emscripten)
  em_link_pre_js(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/emscripten/pre.js")

elseif (PLATFORM MATCHES ios)
  set_source_files_properties(${RESOURCES_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION ${PROJECT_NAME}.app)

elseif (PLATFORM MATCHES android)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND android update project --path . --name ${PROJECT_NAME} --target android-16
    COMMAND ant debug
  )

  install(CODE "execute_process(
    COMMAND adb install -r bin/${PROJECT_NAME}-debug.apk"
  )

elseif (PLATFORM MATCHES osx)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND rm -f resources
    COMMAND ln -s \"${CMAKE_CURRENT_LIST_DIR}/resources\"
  )
endif()

enable_testing()

if (PLATFORM MATCHES android)
  target_link_libraries(${PROJECT_NAME}
    log
    android
    ${Boost_LIBRARIES}
  )

  add_test(NAME ${PROJECT_NAME}
    COMMAND adb shell am start -S org.chronotext.${PROJECT_NAME}/.MainActivity
  )

else()
  target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
  )

  add_test(NAME ${PROJECT_NAME} COMMAND run)
endif()
