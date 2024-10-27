# Set minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Define project name
project(DeribitOrderManagementSystem)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find CURL package
find_package(CURL REQUIRED)

# Find or fetch nlohmann_json
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
    FetchContent_MakeAvailable(json)
endif()

# Find Boost
find_package(Boost REQUIRED system)

# Manually set OpenSSL paths for macOS with Homebrew
set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@3")
set(OPENSSL_INCLUDE_DIR "/opt/homebrew/opt/openssl@3/include")
set(OPENSSL_CRYPTO_LIBRARY "/opt/homebrew/opt/openssl@3/lib/libcrypto.dylib")
set(OPENSSL_SSL_LIBRARY "/opt/homebrew/opt/openssl@3/lib/libssl.dylib")

# Find OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Include WebSocket++ headers (header-only)
include_directories(${CMAKE_SOURCE_DIR}/websocketpp)

# Add include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${CURL_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

# Add all source files
add_executable(DeribitOrderManagementSystem 
    src/main.cpp
    src/DeribitAuth.cpp
)

# Link libraries
target_link_libraries(DeribitOrderManagementSystem PRIVATE
    ${CURL_LIBRARIES}
    nlohmann_json::nlohmann_json
    Boost::system
    OpenSSL::SSL
    OpenSSL::Crypto
)