#!/bin/bash
# Install script for Victron Solar Monitor
# By Nicholas McCatherine, for Undergrad Research at the Ohio State University
# December 8th, 2024

### Make sure to set execution permissions using "sudo chmod +x install.sh"

W='\033[1;37m' # White
R='\033[1;31m' # Red
G='\033[1;32m' # Green
NC='\033[0m' # No Color

while true; do
    read -r -p "Do you want to install all dependencies and compile the Solar Monitor program? (Y/N): " answer
    case $answer in
        [Yy]* ) break;;
        [Nn]* ) exit;;
        * ) echo "Please enter Y or N.";;
    esac
done

# Get the invoking user's home directory 12/9/2024
USER_HOME=$(getent passwd "$SUDO_USER" | cut -d: -f6)

if [ -z "$USER_HOME" ]; then
    echo -e "${R}Error: Could not determine the home directory for $SUDO_USER.${NC}"
    exit 1
fi

DESKTOP_DIR="$USER_HOME/Desktop"
OUTPUT_DIR="$USER_HOME/Desktop/SolarMonitorOutput"

BUILD_DIR="build"
INSTALL_DIR="/usr/local/bin/solarMonitor"
SERVICE_DIR="/etc/systemd/system"
SERVICE_FILE="solarMonitor.service"
CONFIG_FILE="config.txt"
OUTPUT_DIR="$USER_HOME/Desktop/SolarMonitorOutput"

# Install dependencies, openSSL included with apt-get update/upgrade
echo -e "${W}Installing dependencies... ${NC}"
sudo apt-get update
sudo apt-get upgrade -y
# CMake
sudo apt install -y cmake
cmake --version
# Add dependencies for OpenSSL - Note: dev-branch not inlcuded on Raspberry Pi distributable
sudo apt install -y openssl
sudo apt install -y libssl-dev
# Include BlueZ dependencies - will be removed at later date
sudo apt install -y bluez libbluetooth-dev
# dependencies for the python monitor program
sudo apt install python3-matplotlib python3-pandas
# SimpleBLE
sudo apt install libdbus-1-dev
if [ ! -d "SimpleBLE" ]; then
    git clone https://github.com/OpenBluetoothToolbox/SimpleBLE.git
else
    echo -e "${W}SimpleBLE repository already exists.${NC}"
fi
cd SimpleBLE
sudo cmake -S ./simpleble -B build_simpleble -DBUILD_SHARED_LIBS=TRUE
sudo cmake --build build_simpleble
sudo cmake --install build_simpleble
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
cd ..

# Create the build directory
echo -e "${W}Creating build directory..."
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR" || exit 1

echo -e "${W}Running cmake..."
# run Cmake
sudo cmake ..

# Build the project
sudo cmake --build . --target install --

cd ..

# Create the install directory if it doesn't exist - it should after cmake install
if [ ! -d "$INSTALL_DIR" ]; then
    sudo mkdir -p $INSTALL_DIR
fi

echo -e "${G}"
read -r -p "What is the device MAC address (ie: d19a0beb9982)? " MAC
read -r -p "What is the device encryption key (ie: 3b3baa4986a89b47e4b709ab117a4a2d)? " KEY
read -r -p "What is the desired scan interval? (Seconds) " SCAN_INT
read -r -p "What is the desired output filename? (Plain text)" FILE_OUT
read -r -p "How many days do you want the file to cover? " NEW_FILE_INT
echo -r -e "${NC}"

# create config.txt
if [ ! -f "$INSTALL_DIR/$CONFIG_FILE" ]; then
    echo -e "${W}Creating config.txt..."
    sudo cat <<EOL > "$INSTALL_DIR/$CONFIG_FILE"
device_mac=$MAC
key=$KEY
scan_interval=$SCAN_INT
output_file="$OUTPUT_DIR/$FILE_OUT"
new_file_interval=$NEW_FILE_INT
EOL
fi

# create a desktop shortcut to config.txt directory and move the systemctl restart script there.
DESKTOP_DIR="$USER_HOME/Desktop"
SHORTCUT_FILE="$DESKTOP_DIR/solarMonitorConfig.desktop"
sudo cat <<EOL > "$SHORTCUT_FILE"
[Desktop Entry]
Version=1.0
Type=Application
Name=Solar Monitor Config
Comment=Opens the config.txt file in nano and restart the Solar Monitor service
Exec=bash -c "sudo nano $INSTALL_DIR/$CONFIG_FILE; sudo systemctl stop solarMonitor; sudo systemctl restart solarMonitor"
Icon=text-x-generic
Terminal=true
Categories=Utility;
EOL

# set permissions for config file
chmod +x "$DESKTOP_DIR/solarMonitorConfig.desktop"

# check for the output directory and create it if it's not there
if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir -p "$OUTPUT_DIR"
fi

# Create systemd service file
if [ ! -f "$SERVICE_DIR/$SERVICE_FILE" ]; then
    echo -e "${W}Creating solarMonitor.service..."
    cat <<EOL > "$SERVICE_DIR/$SERVICE_FILE"
[Unit]
Description=Victron Solar Monitor Service
After=network.target

[Service]
ExecStart=/usr/local/bin/solarMonitor/solarMonitor
WorkingDirectory=/usr/local/bin/solarMonitor
Environment="LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH"
Restart=always
RestartSec=5
User=root
Environment=MY_VAR=my_value

[Install]
WantedBy=multi-user.target
EOL
fi

# copy the visualization python script to /usr/local/bin
sudo cp ./display/solar_monitor.py /usr/local/bin/solar_monitor.py
sudo chmod +x /usr/local/bin/solar_monitor.py

# Reload systemctl daemon
echo -e "${W}Setting up systemd service..."
sudo systemctl daemon-reload
sudo systemctl enable solarMonitor.service
sudo systemctl restart solarMonitor.service

echo -e "${G}Installation complete, Solar Monitoring service is now running!"
echo -e "Review the log above for errors. This script does not handle errors. ${R}"
read -p "Press any key and enter to continue." x
