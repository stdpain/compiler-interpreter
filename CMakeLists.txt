cmake_minimum_required(VERSION 3.12.0)

project(compiler-interpreter)

include(FetchContent)

function(add_thirdparty PROJECT_NAME GIT_REPOSITORY GIT_TAG)
    FetchContent_Declare(
        ${PROJECT_NAME}
        GIT_REPOSITORY ${GIT_REPOSITORY}
        GIT_TAG ${GIT_TAG}
    )

    FetchContent_MakeAvailable(${PROJECT_NAME})
endfunction()
add_thirdparty(gflags https://github.com/gflags/gflags.git master)
add_thirdparty(glog https://github.com/google/glog.git master)
add_thirdparty(gtest https://github.com/google/googletest.git main)

message(STATUS "GLOG SOURCE ${glog_SOURCE_DIR} ${glog_BINARY_DIR}")
include_directories(${glog_BINARY_DIR})
include_directories(${glog_SOURCE_DIR}/src)

add_subdirectory(interpreter)
