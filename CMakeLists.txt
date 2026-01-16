cmake_minimum_required(VERSION 3.16)

project(WebSocketChat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/source/libs/websocketpp
    ${CMAKE_SOURCE_DIR}/source/libs/json
)

add_executable(server source/server/server.cpp)
add_executable(client source/client/client.cpp)

target_link_libraries(server pthread)
target_link_libraries(client pthread)
