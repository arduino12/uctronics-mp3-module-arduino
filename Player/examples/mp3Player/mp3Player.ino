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

/* pin definitions */
#define ARDUINO_RX_PIN (2)
#define ARDUINO_TX_PIN (3)

/* debug log */
#define DEBUG
#ifdef DEBUG
	#define DEBUG_BAUDRATE		(9600)
	#define DEBUG_PRINT(...)	Serial.print(__VA_ARGS__)
	#define DEBUG_PRINTLN(...)	Serial.println(__VA_ARGS__)
#else
	#define DEBUG_PRINT(...)
	#define DEBUG_PRINTLN(...)
#endif

/* player object */
Player player(ARDUINO_RX_PIN, ARDUINO_TX_PIN);

/* main */
void setup() {
#ifdef DEBUG
	Serial.begin(DEBUG_BAUDRATE);
#endif

	/* reset module*/
	DEBUG_PRINT(F("reset() -> "));
	player.reset();
	DEBUG_PRINTLN(F("finish"));
	
	/* repeat track 3 and stop it afte 4 times */
	DEBUG_PRINT(F("getTracksCount() -> "));
	DEBUG_PRINTLN(player.getTracksCount(), DEC);
	DEBUG_PRINT(F("getFoldersCount() -> "));
	DEBUG_PRINTLN(player.getFoldersCount(), DEC);	
	
	/* play track 1 and wait for it to stop */
	DEBUG_PRINT(F("playTrack(1) -> "));
	player.playTrack(1);
	player.waitForTrack();
	DEBUG_PRINTLN(F("waitForTrack()"));
	
	/* play track 2 and wait for it to stop */
	DEBUG_PRINT(F("playTrack(2) -> "));
	player.playTrack(2);
	DEBUG_PRINT(F("isPlaying() -> "));
	DEBUG_PRINT(player.isPlaying(), DEC);
	DEBUG_PRINT(F(" -> "));
	player.waitForTrack();
	DEBUG_PRINT(F("waitForTrack() -> "));
	DEBUG_PRINT(F("isPlaying() -> "));
	DEBUG_PRINTLN(player.isPlaying(), DEC);
	
	/* set volume to 20 (0 to MAX_VOLUME = 31) */
	DEBUG_PRINT(F("setVolume(20) -> "));
	player.setVolume(20);
	DEBUG_PRINT(F("getVolume() -> "));
	DEBUG_PRINTLN(player.getVolume(), DEC);
	
	/* repeat track 3 and stop it afte 5 times */
	DEBUG_PRINT(F("repeatTrack(3) -> "));
	player.repeatTrack(3);
	player.waitForTracks(5);
	DEBUG_PRINT(F("waitForTracks(5) -> "));
	player.repeatingTrack(false);
	player.stop();
	DEBUG_PRINTLN(F("repeatingTrack(false)"));
	
	/* play all tracks of folder 1 once */
	playFolder(1);
	
	/* play 6 rendom tracks max 4 secons each */
	DEBUG_PRINT(F("repeatShuffledTracks() -> "));
	player.repeatShuffledTracks();
	player.waitForTracks(6, 4);
	DEBUG_PRINT(F("waitForTracks(6) -> "));
	player.repeatingTrack(false);
	player.stop();
	DEBUG_PRINTLN(F("repeatingTrack(false)"));
	
	/* play track 1 in folder 2 and change equalizer and volume */
	DEBUG_PRINT(F("playTrack(2, 1) -> "));
	player.playTrack(2, 1);
	delay(3000);
	player.setEqualizer(ROCK);
	DEBUG_PRINT(F("setEqualizer [ROCK -> "));
	delay(3000);
	player.setEqualizer(POP);
	DEBUG_PRINT(F("POP -> "));
	delay(3000);
	player.setEqualizer(JAZZ);
	DEBUG_PRINT(F("JAZZ -> "));
	delay(3000);
	player.setEqualizer(CLASSICAL);
	DEBUG_PRINT(F("CLASSICAL -> "));
	delay(3000);
	player.setEqualizer(NORMAL);
	DEBUG_PRINTLN(F("NORMAL]"));
	delay(3000);
	player.pause();
	DEBUG_PRINT(F("pause() -> "));
	delay(1000);
	player.play();
	DEBUG_PRINT(F("play() -> "));
	delay(5000);
	player.stop();
	DEBUG_PRINT(F("stop() -> "));
	delay(1000);
	player.next();
	DEBUG_PRINTLN(F("next()"));
	DEBUG_PRINT(F("fade out ["));
	player.setVolume(MAX_VOLUME);
	delay(1000);
	for (uint8_t i = MAX_VOLUME; i > 0; i--) {
		delay(300);
		player.setVolume(i - 1);
		DEBUG_PRINT('|');
	}
	DEBUG_PRINT(F("] -> "));
	player.stop();
	DEBUG_PRINTLN(F("stop()"));

	/* repeat all tracks */
	DEBUG_PRINT(F("repeatTracks() ... :"));
	player.setVolume(MAX_VOLUME - 8);
	player.repeatTracks();
}

void playFolder(uint8_t folder_index) {
	uint8_t trackCount = player.getTrackCount(folder_index);
	DEBUG_PRINT(F("repeatFolder("));
	DEBUG_PRINT(folder_index, DEC);
	DEBUG_PRINT(F(") -> "));
	player.repeatFolder(folder_index);
	player.waitForTracks(trackCount);
	DEBUG_PRINT(F("waitForTracks("));
	DEBUG_PRINT(trackCount, DEC);
	DEBUG_PRINT(F(") -> "));
	player.repeatingTrack(false);
	DEBUG_PRINTLN(F("repeatingTrack(false)"));
}

void loop() {
	if (0 == player.waitForTrack(1)) DEBUG_PRINT(F("]\nwaitForTrack ["));
	DEBUG_PRINT('|');
}
