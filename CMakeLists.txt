cmake_minimum_required(VERSION 3.10)
project(DeribitOrderManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json QUIET)
find_package(Boost REQUIRED COMPONENTS system)

if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
    )
    FetchContent_MakeAvailable(json)
endif()

find_path(WEBSOCKETPP_INCLUDE_DIR websocketpp/server.hpp
    HINTS ${CMAKE_SOURCE_DIR}/external/websocketpp
    /usr/local/include
    /usr/include
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${CURL_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${WEBSOCKETPP_INCLUDE_DIR}
)

add_executable(DeribitOrderManagementSystem 
    src/main.cpp
    src/HttpClient.cpp
    src/DeribitAuth.cpp
    src/PlaceOrder.cpp
    src/CancelOrder.cpp
    src/ModifyOrder.cpp
    src/GetOrderBook.cpp
    src/GetPosition.cpp
    src/WebSocketServer.cpp
    src/Logger.cpp
    src/DeribitTrading.cpp
)

target_link_libraries(DeribitOrderManagementSystem PRIVATE
    ${CURL_LIBRARIES}
    nlohmann_json::nlohmann_json
    ${Boost_LIBRARIES}
)