boblight-rpi
============

Boblight solution for Raspberry Pi using OpenMax

How To Use
==========
```
Upload the file boblight-openmax located in src to your RPi.
On the RPi:
./boblight-openmax -p 5 &
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

make SDKSTAGE=<path/to/firmware>
```
