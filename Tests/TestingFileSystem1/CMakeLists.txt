cmake_minimum_required (VERSION 3.2.2)

project(TestingFileSystem1)

find_package(Boost 1.58.0 REQUIRED COMPONENTS system filesystem)

include_directories(
  ../common
  ${Boost_INCLUDE_DIRS}
)

file(GLOB RESOURCES_FILES res/*)

add_executable(${PROJECT_NAME}
  src/main.cpp
  ${RESOURCES_FILES}
)

if (PLATFORM MATCHES emscripten)
  add_definitions(-DCHR_FS_JS_NODE)
  em_link_pre_js(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/pre.js")

elseif (PLATFORM MATCHES ios)
  add_definitions(-DCHR_FS_BUNDLE)
  set_source_files_properties(${RESOURCES_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION ${PROJECT_NAME}.app/res)

elseif (PLATFORM MATCHES android)
  install(CODE "execute_process(
    COMMAND adb push \"${CMAKE_CURRENT_LIST_DIR}/res/.\" /data/local/tmp/)"
  )

elseif (PLATFORM MATCHES osx|mxe)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND rm -f res
    COMMAND ln -s \"${CMAKE_CURRENT_LIST_DIR}/res\"
  )
endif()

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

enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND run)
