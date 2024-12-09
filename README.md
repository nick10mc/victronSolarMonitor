# victronSolMon
Lightweight, simple Solar Monitor background program &amp; GUI for Ubuntu/Raspberry Pi

Compilation:
```
g++ -std=c++23 -o solarMonitor main.cpp BLE.cpp AES.cpp bleParse.cpp datFunc.cpp mfgRegisterDat.cpp -I/usr/local/include/simpleble -L/usr/local/lib -lsimpleble -lpthread -lbluetooth -ldbus-1 -lssl -lcrypto
```

