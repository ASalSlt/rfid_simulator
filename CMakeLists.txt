cmake_minimum_required(VERSION 3.20)

project(
    rfid_simulator
    VERSION 1.0.0
    LANGUAGES C CXX)

# ============================================================================
# C++ Standard Configuration
# ============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Include Custom Modules
# ============================================================================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(paho-mqtt)

# ============================================================================
# Source Code
# ============================================================================
file(GLOB SOURCES "src/*.cpp")

# ============================================================================
# Main Executable
# ============================================================================
add_executable(rfid_simulator
    app/main.cpp
    ${SOURCES})

target_include_directories(rfid_simulator PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/external/paho.mqtt.cpp/include"
)

target_link_libraries(rfid_simulator
    PRIVATE paho-mqttpp3
    PRIVATE pthread
)

target_link_options(rfid_simulator PRIVATE -static-libstdc++ -static-libgcc)