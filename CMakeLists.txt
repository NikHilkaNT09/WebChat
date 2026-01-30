cmake_minimum_required(VERSION 3.16)
project(WebSocketChat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# ---------- Server ----------
add_executable(server source/server/server.cpp)
target_include_directories(server PRIVATE
    ${CMAKE_SOURCE_DIR}/source/libs/websocketpp
    ${CMAKE_SOURCE_DIR}/source/libs/json
)
target_link_libraries(server PRIVATE pthread)

# ---------- Client ----------
# add_executable(client source/client/client.cpp)
# target_include_directories(client PRIVATE
#     ${CMAKE_SOURCE_DIR}/source/libs/websocketpp
#     ${CMAKE_SOURCE_DIR}/source/libs/json
# )
# target_link_libraries(client PRIVATE pthread)

# ---------- Receiver ----------
# add_executable(receiver source/receiverClient/receiverClient.cpp)
# target_include_directories(receiver PRIVATE
#     ${CMAKE_SOURCE_DIR}/source/libs/websocketpp
#     ${CMAKE_SOURCE_DIR}/source/libs/json
# )
# target_link_libraries(receiver PRIVATE pthread)

# ---------- GUI CLIENT ----------
add_executable(Client 
    source/chatUi/chatApp.cpp
    source/chatUi/chatUi.h
)
target_include_directories(Client PRIVATE
    ${CMAKE_SOURCE_DIR}/source/libs/websocketpp
    ${CMAKE_SOURCE_DIR}/source/libs/json
)
target_link_libraries(Client
    PRIVATE
    ${wxWidgets_LIBRARIES}
    pthread
)
