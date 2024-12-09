#!/bin/bash
# Install script for Victron Solar Monitor
# By Nicholas McCatherine, for Undergrad Research at the Ohio State University
# December 8th, 2024

### Make sure to set execution permissions using "sudo chmod +x install.sh"

W="\033[1;37m" # White
R="\033[1;31m" # Red
G="\033[1;32m" # Green

BUILD_DIR="build"
INSTALL_DIR="/usr/local/bin/solarMonitor"
SERVICE_DIR="/etc/systemd/system"
SERVICE_FILE="solarMonitor.service"
CONFIG_FILE="config.txt"
OUTPUT_DIR="$HOME/Desktop/SolarMonitorOutput"

# Install dependencies, openSSL included with apt-get update/upgrade
echo "${W}Installing dependencies..."
sudo apt-get update
sudo apt-get upgrade
# SimpleBLE
sudo apt install libdbus-1-dev
git clone https://github.com/OpenBluetoothToolbox/SimpleBLE.git
cd SimpleBLE
cmake -S . -B build_simpleble -DBUILD_SHARED_LIBS=TRUE
cmake --build build_simpleble -j$(nproc)
cmake --install build_simpleble
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# Create the build directory
echo "${W}Creating build directory..."
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR" || exit 1

echo "${W}Running cmake..."
# run Cmake
cmake ../src

# Build the project
cmake --build . --target install -- -j$(nproc)

cd ..

# Create the install directory if it doesn't exist - it should after cmake install
if [ ! -d "$INSTALL_DIR" ]; then
    mkdir -p $INSTALL_DIR
fi

read -p "${G}What is the device MAC address? ${W}" MAC
read -p "${G}What is the device encryption key? ${W}" KEY
read -p "${G}What is the desired scan interval? ${W}" SCAN_INT
read -p "${G}What is the desired output filename? ${W}" FILE_OUT
read -p "${G}How many days do you want the file to cover? ${W}" NEW_FILE_INT

# create config.txt
if [ ! -f "$CONFIG_FILE" ]; then
    echo "${W}Creating config.txt..."
    cat <<EOL > "$CONFIG_FILE"
    device_mac=$MAC
    key=$KEY
    scan_interval=$SCAN_INT
    output_file="$OUTPUT_DIR/$FILE_OUT"
    new_file_interval=$NEW_FILE_INT
EOL
fi

# check for the output directory and create it if it's not there
if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir -p "$OUTPUT_DIR"
fi

# Create systemd service file
if [ ! -f "$SERVICE_DIR/$SERVICE_FILE" ]; then
    echo "${W}Creating solarMonitor.service..."
    cat <<EOL > "$SERVICE_DIR/$SERVICE_FILE"
    [Unit]
    Description=Victron Solar Monitor Service
    After=network.target

    [Service]
    ExecStart=/usr/local/bin/VictronSolarMon/solarMonitor
    WorkingDirectory=/usr/local/bin/VictronSolarMon
    Restart=always
    RestartSec=5
    User=root
    Environment=MY_VAR=my_value

    [Install]
    WantedBy=multi-user.target
EOL
fi

# Reload systemctl daemon
echo "${W}Setting up systemd service..."
sudo systemctl daemon-reload
sudo systemctl enable solarMonitor.service
sudo systemctl restart solarMonitor.service

echo "${G}Installation complete, Solar Monitoring service is now running!"
