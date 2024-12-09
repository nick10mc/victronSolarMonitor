cmake_minimum_required(VERSION 3.21)
project(solarMonitor VERSION 0.1.0 LANGUAGES C CXX)

# Set the standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the source files
set(SOURCES
    src/main.cpp
    src/BLE.cpp
    includes/BLE.h
    src/AES.cpp
    includes/AES.h
    src/bleParse.cpp
    includes/bleParse.h
    src/datFunc.cpp
    includes/datFunc.h
    src/mfgRegisterDat.cpp
    includes/mfgRegisterDat.h
    includes/common.h
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
include_directories(${simpleble_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/includes)
find_package(OpenSSL REQUIRED)
find_package(simpleble REQUIRED)
set(BUILD_SHARED_LIBS ON)

# Link Libraries
target_link_libraries(${PROJECT_NAME}
    simpleble::simpleble
    pthread
    bluetooth
    dbus-1
    ssl
    crypto
)

target_link_directories(${PROJECT_NAME} PRIVATE /usr/local/include/simpleble /usr/local/lib)

# Installation instructions
set(INSTALL_DIR /usr/local/bin/solarMonitor)
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_DIR})