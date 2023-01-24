cmake_minimum_required(VERSION 3.20)

project(ToLogOrNotToLog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1")
endif()

add_executable(${PROJECT_NAME} main.cpp unix_domain_socket_sink.h unix_domain_socket_client.h)

target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
