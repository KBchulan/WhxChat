cmake_minimum_required(VERSION 3.24)
project(ChatServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O3)
        add_compile_options(-march=native)
    endif()
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(SPDLOG REQUIRED spdlog)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

aux_source_directory(
    ${PROJECT_SOURCE_DIR}/src
    SRC_FILES
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    ${SPDLOG_LIBRARIES} 
    ${JSONCPP_LIBRARIES}
)
