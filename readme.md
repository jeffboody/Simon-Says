About
=====

Simon Says is a memory game. Start the game by pressing one of the
four buttons. When a button lights up, press the button, repeating
the sequence. The sequence will get longer and longer. The game is
won after 13 rounds.

The Simon Says Soldering kit is available from Sparkfun.

* <a href="https://www.sparkfun.com/products/10547">Simon Says - Through Hole Soldering Kit</a>
* <a href="https://www.sparkfun.com/products/10935">Simon Says - Surface Mount Soldering Kit</a>

License
=======

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
