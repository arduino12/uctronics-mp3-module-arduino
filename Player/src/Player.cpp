/**
  ******************************************************************************
  * @file   Player.cpp
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

#include "Player.h"

Player::Player(uint8_t receivePin, uint8_t transmitPin) : SoftwareSerial(receivePin, transmitPin) {
	SoftwareSerial::begin(9600);
}

void Player::play(void) {
	writeCommandId(0x0D);
}

void Player::pause(void) {
	writeCommandId(0x0E);
}

void Player::stop(void) {
	writeCommandId(0x16);
}

void Player::next(void) {
	writeCommandId(0x01);
}

void Player::prev(void) {
	writeCommandId(0x02);
}

void Player::volumeUp(void) {
	writeCommandId(0x04);
}

void Player::volumeDown(void) {
	writeCommandId(0x05);
}

void Player::setVolume(uint8_t volume) {
	if (volume > MAX_VOLUME) volume = MAX_VOLUME;
	writeCommand(0x06, 0, 0, volume);
}

void Player::setEqualizer(eq_type_t eq_type) {
	writeCommand(0x07, 0, 0, eq_type);
}

void Player::reset(void) {
	writeCommandId(0x0C);
	delay(1000);
}

void Player::setPower(bool is_on) {
	writeCommandId(is_on ? 0x0B : 0x0A);
	delay(1000);
}

void Player::playTrack(uint16_t track_index) {
	writeCommand(0x03, 0, track_index >> 8, track_index);
}

void Player::playTrack(uint8_t folder_index, uint8_t track_index) {
	writeCommand(0x0F, 0, folder_index, track_index);
}

void Player::repeatTrack(uint16_t track_index) {
	writeCommand(0x08, 0, track_index >> 8, track_index);
}

void Player::repeatTrack(uint8_t folder_index, uint8_t track_index) {
	playTrack(folder_index, track_index);
	repeatingTrack();
}

void Player::repeatTracks(void) {
	writeCommand(0x11, 0, 0, 1);
}

void Player::repeatShuffledTracks(void) {
	writeCommandId(0x18);
}

void Player::repeatingTrack(bool is_repeated) {
	writeCommand(0x19, 0, 0, is_repeated ? 0 : 1);
	delay(80);
}

void Player::repeatFolder(uint8_t folder_index) {
	writeCommand(0x17, 0, 0, folder_index);
}

int8_t Player::waitForTrack(uint16_t timeout) {
	uint8_t id = 0, data = 0;
	return readCommand(&id, NULL, NULL, &data, (uint32_t)timeout * 1000);
}

int8_t Player::waitForTracks(uint16_t tracks_count, uint16_t timeout) {
	int8_t return_value = 0;
	while(0 < tracks_count) {
		return_value = waitForTrack(timeout);
		if (return_value) goto l_cleanup;
		tracks_count--;
	}
	
l_cleanup:
	return return_value;
}

bool Player::isPlaying(void) {
	uint8_t data = 0;
	delay(80);
	writeCommandId(0x42);
	readCommand(NULL, NULL, NULL, &data);
	return 1 == data;
}

uint8_t Player::getVolume(void) {
	uint8_t data = 0;
	writeCommandId(0x43);
	readCommand(NULL, NULL, NULL, &data);
	return data;
}

eq_type_t Player::getEqualizer(void) {
	uint8_t data = 0;
	writeCommandId(0x44);
	readCommand(NULL, NULL, NULL, &data);
	return (eq_type_t)data;
}

uint16_t Player::getTracksCount(void) {
	uint8_t b = 0, c= 0;
	writeCommandId(0x48);
	readCommand(NULL, NULL, &b, &c);
	return (((uint16_t)b) << 8) | c;
}

uint8_t Player::getFoldersCount(void) {
	uint8_t data = 0;
	writeCommandId(0x4f);
	readCommand(NULL, NULL, NULL, &data);
	return data;
}

uint8_t Player::getTrackCount(uint8_t folder_index) {
	uint8_t data = 0;
	writeCommand(0x4E, 0, 0, folder_index);
	readCommand(NULL, NULL, NULL, &data);
	return data;
}

void Player::writeCommandId(uint8_t id) {
	writeCommand(id, 0, 0, 0);
}

void Player::writeCommand(uint8_t id, uint8_t a, uint8_t b, uint8_t c) {
	//uint8_t command = {0x7E, 0xFF, 0x06, id, a, b, c, 0xEF};
	while(available()) flush();
	write(0x7E);
	write(0xFF);
	write(0x06);
	write(id);
	write(a);
	write(b);
	write(c);
	write(0xEF);
	delay(50);
}

int8_t Player::readCommand(uint8_t * id, uint8_t * a, uint8_t * b, uint8_t * c, uint32_t timeout) {
	uint8_t buff[READ_CMD_SIZE] = {0};
	if (READ_CMD_SIZE != readToBuffer(buff, READ_CMD_SIZE, timeout)) return -1;
	if ((buff[0] != 0x7E) ||
		(buff[1] != 0xFF) ||
		(buff[2] != 0x06) ||
		(buff[7] != 0xFE) ||
		(buff[9] != 0xEF)) return -2;
	if (NULL != id) *id = buff[3];
	if (NULL != a)  *a  = buff[4];
	if (NULL != b)  *b  = buff[5];
	if (NULL != c)  *c  = buff[6];
	return 0;
}

uint16_t Player::readToBuffer(uint8_t * buff, uint16_t len, uint32_t timeout) {
	int read_bytes = 0;
	uint32_t start_millis = millis();
	
	while ((millis() - start_millis) < timeout) {
		if (read_bytes >= len) goto l_cleanup;
		if (available()) buff[read_bytes++] = read();
		delay(1);
	}
	
l_cleanup:
	return read_bytes;
}
