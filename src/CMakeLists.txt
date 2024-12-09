cmake_minimum_required(VERSION 3.21)
project(solarMonitor VERSION 0.1.0b LANGUAGES C CXX)

# Set the standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the source files
set(SOURCES
    src/main.cpp
    src/BLE.cpp
    include/BLE.h
    src/AES.cpp
    include/AES.h
    src/bleParse.cpp
    include/bleParse.h
    src/datFunc.cpp
    include/datFunc.h
    src/mfgRegisterDat.cpp
    include/mfgRegisterDat.h
    include/common.h
)

# Include directories
find_package(OpenSSL REQUIRED)
find_package(simpleble REQUIRED)
target_link_directories(${PROJECT_NAME} PRIVATE /usr/local/include/simpleble /usr/local/lib)
target_link_libraries(${PROJECT_NAME} simpleble::simpleble)
include_directories(${simpleble_INCLUDE_DIRS})
set(BUILD_SHARED_LIBS ON)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Libraries
target_link_libraries(${PROJECT_NAME}
    simpleble
    pthread
    bluetooth
    dbus-1
    ssl
    crypto
)

# Installation instructions
set(INSTALL_DIR /usr/local/bin/solarMonitor)
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_DIR})