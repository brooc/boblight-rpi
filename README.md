boblight-rpi
============

Boblight solution for Raspberry Pi using DispmanX

How To Use
==========
```
Upload the file boblight-dispmanx located in src to your RPi.

On the RPi
**********
Check your firmware version with 'vcgencmd version'
If your firmaware version is older than Jul 2 2013 update your firmware.

./boblight-dispmanx -p 5 &
```

How To Compile
==============

Step 1
======
```
This project compiles against the Raspberry Pi firmware libraries.
The first thing you need, is to get the sources from https://github.com/raspberrypi/firmware
and compile them
```

Step 2
======
```
Compile the code using these commands:
./configure --without-portaudio --without-x11 --without-libusb
or this command if cross-compiling
./configure --without-portaudio --without-x11 --without-libusb --host=arm-<cross-compiler>

cd src

make SDKSTAGE=<path/to/firmware-directory>
```
