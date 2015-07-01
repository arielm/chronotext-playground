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
  set_source_files_properties("resources/test.txt" PROPERTIES MACOSX_PACKAGE_LOCATION ${PROJECT_NAME}.app)

elseif (PLATFORM MATCHES android)
  install(CODE "execute_process(
    COMMAND adb push \"${CMAKE_CURRENT_LIST_DIR}/resources/.\" /data/local/tmp/)"
  )

elseif (PLATFORM MATCHES osx|mxe)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND rm -f resources && ln -s \"${CMAKE_CURRENT_LIST_DIR}/resources\"
  )
endif()

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
)

enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND run)
