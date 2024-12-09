# victronSolMon
Lightweight, simple Solar Monitor background program &amp; GUI for Ubuntu/Raspberry Pi

Usage:
1. Download the repository by typing the following into a bash terminal:
   ```
   git clone https://github.com/nick10mc/victronSolarMonitor
   ```
2. Change directories, and set the permissions of the install.sh script to allow execution.
   ```
   cd victronSolarMonitor
   sudo chmod +x insall.sh
   ```
3. Run the installation script, which will download all neccessary dependencies. This will also compile simpleBLE and link it to the main executable. The script will compile the program and install it in the proper directory along with config.txt. The script will then add the system service and a shortcut to configure config.txt. Follow the on screen prompts, and enter the config data in the formats displayed.
   ```
   ./install.sh
   ```
4. Output files will be recorded to a folder on the desktop
