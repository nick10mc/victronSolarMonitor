# Victron Solar Monitor
## Lightweight, simple Solar Monitor background program to be used with Victron SmartSolar products for Raspberry Pi.
*By Nick McCatherine, Ohio State University, School of Earth Sciences
December 9th, 2024.*

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
   
![Screenshot_20241209-154320](https://github.com/user-attachments/assets/5f6e548e-f66d-41e2-82e2-5f29ae55b828)


Once connected, click on the gear icon in the upper right corner.

![Screenshot_20241209-154334](https://github.com/user-attachments/assets/9af79ac5-d10c-4780-ad2e-6ae36eac4dc9)


Then, click on the three dot "hamburger" at the top right, and select "Product Info".

![Screenshot_20241209-154343](https://github.com/user-attachments/assets/307a6f60-18fb-43ea-9dee-46329ea9e250)


Scroll down to the bottom, and copy the address and encryption key. Paste these into the prompts.

![Screenshot_20241209-154350](https://github.com/user-attachments/assets/eb0d4489-5a7b-4c47-b335-2aafbc90f9a3)
![Screenshot_20241209-154403](https://github.com/user-attachments/assets/efd1c60f-6241-4fc8-9185-1770bf9c7d75)


6. Output files will be recorded to a folder on the desktop

## Usage
The config file can be changed by accessing the shortcut titled "solarMonitorConfig.desktop" on your user desktop. When you are finished editing the config file, press Ctrl+S to save, and then Ctrl+X to exit, it will restart the service automatically. Note: if you exit the terminal by clicking on the X button in the upper right corner, it will not restart the monitoring service.
* If at any point you need to debug or confirm that the program is operating, use one of the following commands:
```
sudo systemctl status watchdog --no-pager
```
or:
```
sudo journalctl -u solarMonitor -f
```
