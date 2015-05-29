/**
  ******************************************************************************
  * @file   Player.h
  * @author  A.E Tec
  * @version V1.0
  * @date    23/4/15
  * @brief   This file provides all the Uctronics MP3 module serial functions.
  ******************************************************************************
    @note
         This driver is for Uctronics MP3 module (http://www.ebay.com/itm/281387900991)
  ******************************************************************************
  * @section  HISTORY
  
    V1.0    Initial version.

  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, ELECHOUSE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 ELECHOUSE</center></h2>
  ******************************************************************************
  */

#ifndef Player_h
#define Player_h

#include "Arduino.h"
#include "SoftwareSerial.h"

#define MAX_VOLUME		(0x1E)
#define READ_CMD_SIZE	(10)
#define READ_MILLIS		(1000)
#define WAITING_SECONDS	(1000)

typedef enum {
	NORMAL		= 0,
	ROCK		= 1,
	POP			= 2, 
	JAZZ		= 3,
	CLASSICAL	= 4,
} eq_type_t;
		
class Player : public SoftwareSerial {
	public:
		Player(uint8_t receivePin, uint8_t transmitPin);

		void play(void);
		void pause(void);
		void stop(void);
		void next(void);
		void prev(void); 
		void volumeUp(void);
		void volumeDown(void);
		
		void setVolume(uint8_t volume);
		void setEqualizer(eq_type_t eq_type);	 
		
		void reset(void);
		void setPower(bool is_on);
		
		void playTrack(uint16_t track_index);
		void playTrack(uint8_t folder_index, uint8_t track_index);
		
		void repeatTrack(uint16_t track_index);
		void repeatTrack(uint8_t folder_index, uint8_t track_index);
		
		void repeatTracks(void);
		void repeatShuffledTracks(void);
		
		void repeatingTrack(bool is_repeated = true);
		void repeatFolder(uint8_t folder_index);
		
		int8_t waitForTrack(uint16_t timeout = WAITING_SECONDS);
		int8_t waitForTracks(uint16_t tracks_count, uint16_t timeout = WAITING_SECONDS);
		
		bool isPlaying(void);
		uint8_t getVolume(void);
		eq_type_t getEqualizer(void);

		uint16_t getTracksCount();
		uint8_t getFoldersCount();
		uint8_t getTrackCount(uint8_t folder_index);

	private:
		void writeCommandId(uint8_t id);
		void writeCommand(uint8_t id, uint8_t a, uint8_t b, uint8_t c);
		int8_t readCommand(uint8_t * id, uint8_t * a, uint8_t * b, uint8_t * c, uint32_t timeout = READ_MILLIS);
		uint16_t readToBuffer(uint8_t * buff, uint16_t len, uint32_t timeout);
};

#endif
