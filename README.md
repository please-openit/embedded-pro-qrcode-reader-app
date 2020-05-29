# Embedded QRCode app reader

## Requirements

- A QR code reader
- a relay board to drive on USB or serial
- python
- pyinstaller
- innosetup (windows)

## Build

pyinstaller --onefile theaccess.io_client.py

## Installation

### Windows
Build windows setup file with inno setup compiler.

Just run setup.exe

It will install the app, edit the config file and put it as a startup script.

### Linux

Copy theaccess.io_client binary to /opt, given execution rights with ```chmod +x /opt/theaccess.io_client```.

Do not forget to create also the configuration file /opt/theaccess.io_config.ini based on the template.

#### Disable login

##### Raspberry pi
On raspberry pi, use ```raspi-config ``` select **Boot Options** then **Console Autologin Text console, automatically logged in as 'pi' user**

##### Other
edit inittab file : ```nano /etc/inittab```

Replace tty1 launch line : ```1:2345:respawn:/sbin/getty --noclear 38400 tty1```

By a new one, containing the user you want to login : ```1:2345:respawn:/bin/login -f USER tty1 /dev/tty1 2>&1```

#### Launch on startup

Edit Pi bashrc script : ```nano /home/pi/.bashrc```

By the end add : ```/opt/theaccess.io_client```

## Configuration

For Windows, the theaccess.io_config.ini file is in the same directory where you installed the app.

On Linux systems, configuration file is located at /opt/theaccess.io_config.ini