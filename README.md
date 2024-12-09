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
3. Run the installation script, which will download all neccessary dependencies. This will also compile simpleBLE and link it to the main executable. The script will compile the program and install it in the proper directory along with config.txt. The script will then add the system service and a shortcut to configure config.txt.
   ```
   sudo ./install.sh
   ```
4. When prompted, enter your desired confirmation data. To access the MAC address and encryption key for your Victron SmartSolar charger, you must download VictronConnect to your phone (https://www.victronenergy.com/victronconnectapp/victronconnect/downloads), open up the app, and connect to the controller. Your pin code should be listed in the documentation that was included with the controller. If you do not have a code, try "000000". Failing that, the controller can be manually reset, the procedure for which is in Victron's included literature.
![Screenshot_20241209-154320](https://github.com/user-attachments/assets/77390e69-f371-4ecf-bfc4-1e6f6d0bd265)

Once connected, click on the gear icon in the upper right corner.
![Screenshot_20241209-154334](https://github.com/user-attachments/assets/4b185677-5c88-406f-9147-e9a691ca803c)

Then, click on the three dot "hamburger" at the top right, and select "Product Info".
![Screenshot_20241209-154343](https://github.com/user-attachments/assets/6914a6ca-32f2-4ff3-8fa4-57eb7f74ea60)

Scroll down to the bottom, and copy the address and encryption key. Paste these into the prompts.
![Screenshot_20241209-154350](https://github.com/user-attachments/assets/ca67193f-242e-4593-8d4d-466371949595)
![Screenshot_20241209-154403](https://github.com/user-attachments/assets/736037d4-b4e9-4a52-94fd-ca49c8851b50)

6. Output files will be recorded to a folder on the desktop
