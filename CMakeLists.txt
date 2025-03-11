cmake_minimum_required(VERSION 3.16)
project(cgcpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(SDL3_PACKAGE_PATH "${CMAKE_SOURCE_DIR}/packages/SDL3-3.2.8")
set(SDL3_BIN_PATH "${SDL3_PACKAGE_PATH}/lib/x64/SDL3.dll")
set(SDL3_ttf_PACKAGE_PATH "${CMAKE_SOURCE_DIR}/packages/SDL3_ttf-3.2.0")
set(SDL3_ttf_BIN_PATH "${SDL3_ttf_PACKAGE_PATH}/lib/x64/SDL3_ttf.dll")

set(CMAKE_PREFIX_PATH "${SDL3_PACKAGE_PATH};${SDL3_ttf_PACKAGE_PATH}")
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

file(GLOB SOURCES "src/*.cpp" "src/common/*.cpp")
add_executable(cgcpp ${SOURCES})

target_link_libraries(cgcpp PRIVATE
    SDL3_ttf::SDL3_ttf
    SDL3::SDL3
)

add_custom_command(
    TARGET cgcpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL3_BIN_PATH} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
add_custom_command(
    TARGET cgcpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL3_ttf_BIN_PATH} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)