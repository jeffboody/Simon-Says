About
=====

This project is a fork of the Sparkfun Simon Says Soldering Kit. It
contains modifications to the firmware to pass game state over
Bluetooth serial so that the Simon Says device can be used as a
controller for an Android app. It is my hope that this app will
provide a good example for others who wish to combine Android
and Arduino.

Simon Says is a memory game. Start the game by pressing one of the
four buttons. When a button lights up, press the button, repeating
the sequence. The sequence will get longer and longer. The game is
won after 13 rounds.

The Simon Says Soldering kit is available from Sparkfun (the git
repository only seems to support the PTH version at this time).

* <a href="https://www.sparkfun.com/products/10547">Simon Says - Through-Hole Soldering Kit</a>
* <a href="https://www.sparkfun.com/products/11202">Learn to Solder - Simon Says</a>

The Simon Says (SS) device can control the SS Android app when
connected over Bluetooth. This SS app does not perform any game
logic and simply shows the game state as reported by the SS
device. When the SS device is in attract mode (lights blinking
CCW) then the SS app can connect and the game restarts.

To modify an existing SS device to support the SS app you simply
need to flash the new firmware, connect a Bluetooth device and
launch the SS app. Be sure that the baud rate in the firmware
(default is 57600) matches the baud rate of the Bluetooth device.

Connect the Bluetooth device to the SS device as follows:

1. Bluetooth VCC to SS 5V (through hole) or SS 3.3V (surface mount)
2. Bluetooth GND to SS GND
3. Bluetooth Tx to SS Rx (disconnect when uploading sketches)
4. Bluetooth Rx to SS Tx
5. Bluetooth Cts to Bluetooth Rts (if applicable)

The Bluesmirf Bluetooth module is available from Sparkfun.

* <a href="https://www.sparkfun.com/products/10268">Bluesmirf Gold</a>
* <a href="https://www.sparkfun.com/products/10269">Bluesmirf Silver</a>

Simon Says Firmware
===================

Installing Arduino Dev Kit
--------------------------

	# install Arduino
	sudo apt-get install arduino

	# install ino command line toolkit (optional)
	# https://github.com/amperka/ino
	sudo apt-get install python-setuptools
	sudo apt-get install python-configobj
	sudo apt-get install python-jinja2
	sudo apt-get install python-serial
	cd $SRC
	git clone git://github.com/amperka/ino.git
	cd ino
	sudo make install

Build and upload with ino (optional)
------------------------------------

	cd Firmware/ino
	ino build -m atmega328
	ino upload -m atmega328

Simon Says App
==============

Source Code
-----------

The SS app is a fork of the Sparkfun Simon Kit. The SS app
uses several git submodules that need to be initialized before the
app can be built.

	git clone git@github.com:jeffboody/Simon-Says.git
	cd Simon-Says
	git submodule update --init

Building, Installing and Uninstalling
-------------------------------------

1. Install the Android(TM) SDK and NDK available from http://developer.android.com/
2. Change to App directory

	cd App

3. Initialize environment variables

	<edit profile>
	source profile

4. Build project

	./build-native.sh
	./build-java.sh

5. Install apk

	./install.sh

6. Uninstall apk

	./uninstall.sh

Firmware License
================

	Started: 6-19-2007
	Spark Fun Electronics
	Nathan Seidle

	Simon Says is a memory game. Start the game by pressing one of the
	four buttons. When a button lights up, press the button, repeating
	the sequence. The sequence will get longer and longer. The game is
	won after 13 rounds.

	This code is public domain but you buy me a beer if you use this and
	we meet someday (Beerware license).

	Simon Says game originally written in C for the PIC16F88.
	Ported for the ATmega168, then ATmega328, then Arduino 1.0.
	Fixes and cleanup by Joshua Neal <joshua[at]trochotron.com>

	Generates random sequence, plays music, and displays button lights.

	Simon tones from Wikipedia
	- A (red, upper left) - 440Hz - 2.272ms - 1.136ms pulse
	- a (green, upper right, an octave higher than A) - 880Hz - 1.136ms,
	0.568ms pulse
	- D (blue, lower left, a perfect fourth higher than the upper left)
	587.33Hz - 1.702ms - 0.851ms pulse
	- G (yellow, lower right, a perfect fourth higher than the lower left) -
	784Hz - 1.276ms - 0.638ms pulse

	The tones are close, but probably off a bit, but they sound all right.

	The old version of SparkFun simon used an ATmega8. An ATmega8 ships
	with a default internal 1MHz oscillator.  You will need to set the
	internal fuses to operate at the correct external 16MHz oscillator.

	Original Fuses:
	avrdude -p atmega8 -P lpt1 -c stk200 -U lfuse:w:0xE1:m -U hfuse:w:0xD9:m

	Command to set to fuses to use external 16MHz:
	avrdude -p atmega8 -P lpt1 -c stk200 -U lfuse:w:0xEE:m -U hfuse:w:0xC9:m

	The current version of Simon uses the ATmega328. The external osciallator
	was removed to reduce component count.  This version of simon relies on the
	internal default 1MHz osciallator. Do not set the external fuses.

App License
===========

	Copyright (c) 2013 Jeff Boody

	Permission is hereby granted, free of charge, to any person obtaining a
	copy of this software and associated documentation files (the "Software"),
	to deal in the Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish, distribute, sublicense,
	and/or sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
