cmake_minimum_required(VERSION 3.24)

project(GateServer)

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

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)

find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

aux_source_directory(
    ${PROJECT_SOURCE_DIR}/src
    SRC_FILES
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/config.ini
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config/config.ini
    COPYONLY
)

add_executable(
    ${PROJECT_NAME}
    ${SRC_FILES}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE

    # gRPC和Protobuf
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
    
    # third party
    ${JSONCPP_LIBRARIES}
)
