cmake_minimum_required(VERSION 3.10)
project(DeribitOrderManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json QUIET)

if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
    )
    FetchContent_MakeAvailable(json)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${CURL_INCLUDE_DIRS}
)

add_executable(DeribitOrderManagementSystem 
    src/main.cpp
    src/HttpClient.cpp
    src/DeribitAuth.cpp
    src/PlaceOrder.cpp
    src/CancelOrder.cpp
)

target_link_libraries(DeribitOrderManagementSystem PRIVATE
    ${CURL_LIBRARIES}
    nlohmann_json::nlohmann_json
)