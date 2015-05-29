/*********************************** GENERAL *********************************\
| File:		mp3Player.ino
| Author:	A.E Tec
| Date:		23/4/15
\*****************************************************************************/
/************************************ USES ***********************************\
| This sketch demonstrates the use of Uctronics MP3 module lib.
|
| The SD card must have the files in the currect format (001.mp3 - 255.mp3) and folders (01 - 99).
| So copy the content of the SD folder to your SD card root.
|
| After burning the sketch, open serial window at 9600 boud- to see the debug log...
\*****************************************************************************/
/*********************************** CIRCUIT *********************************\
| MP3 Player module connections:
|	* VCC	(1)		to VCC (5v)
|	* GND	(7,10)	to GND (0v)
|	* RX	(2)		to ARDUINO_TX_PIN (3) VIA 1K-10K resistor!!! (else, a static noise will be heared...)
|	* TX	(3)		to ARDUINO_RX_PIN (2)
|	* SPK1	(6)		to Speaker
|	* SPK2	(8)		to Speaker
\*****************************************************************************/
#include "SoftwareSerial.h"
#include "Player.h"

/* player object */
Player player(2, 3);

/* main */
void setup() {
	/* play track 1 with max volume (31) - 8 and wait for it to stop */
	player.setVolume(MAX_VOLUME-8);
	player.playTrack(1);
	player.waitForTrack();
	
	/* play track 1 from folder 2 with Volume=20 Equalizer=CLASSICAL and wait for it to stop */
	player.setVolume(20);
	player.setEqualizer(CLASSICAL);
	player.playTrack(2, 1);
	player.waitForTrack();
	
	/* play all tracks in random with Volume=30 Equalizer=NORMAL */
	player.setVolume(30);
	player.setEqualizer(NORMAL);
	player.repeatShuffledTracks();
}

void loop() {
	
}
