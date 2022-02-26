cmake_minimum_required(VERSION 3.18)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(
  CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_MODULE_PATH}
)
set(CMAKE_PREFIX_PATH /usr;/usr/local;/opt/local)
project(
  wp
  VERSION 1.0
  LANGUAGES C
)

enable_testing()
find_package(
  Check
  0.15.2
  REQUIRED
)

file(
  GLOB_RECURSE
  ${CMAKE_PROJECT_NAME}_sources
  ${CMAKE_SOURCE_DIR}/src/cmd/*c
  ${CMAKE_SOURCE_DIR}/src/cmd/*.h
)
file(
  GLOB_RECURSE
  ${CMAKE_PROJECT_NAME}_lib_sources
  ${CMAKE_SOURCE_DIR}/src/lib/*c
  ${CMAKE_SOURCE_DIR}/src/lib/*.h
)
file(GLOB ${CMAKE_PROJECT_NAME}_test_sources test/*.c test/*.h)

add_executable(${CMAKE_PROJECT_NAME} ${${CMAKE_PROJECT_NAME}_sources})
add_library(${CMAKE_PROJECT_NAME}lib ${${CMAKE_PROJECT_NAME}_lib_sources})
add_executable(${CMAKE_PROJECT_NAME}_tests ${${CMAKE_PROJECT_NAME}_test_sources})

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${CHECK_LIBRARIES} ${CMAKE_PROJECT_NAME}lib)
target_link_libraries(${CMAKE_PROJECT_NAME}_tests PUBLIC ${CHECK_LIBRARIES} ${CMAKE_PROJECT_NAME}lib)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/cmd)
target_include_directories(${CMAKE_PROJECT_NAME}lib PUBLIC ${CMAKE_SOURCE_DIR}/src/lib)
target_include_directories(
  ${CMAKE_PROJECT_NAME}_tests
  PUBLIC
  ${CHECK_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/src/lib
  ${CMAKE_SOURCE_DIR}/test
)

install(TARGETS ${CMAKE_PROJECT_NAME})


add_test(NAME all_tests COMMAND ${CMAKE_PROJECT_NAME}_tests)


if(MSVC)
  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PRIVATE /W4
    /WX
  )
else()
  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PRIVATE -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wno-unused-parameter
  )
endif()
