
#ifndef __CRC_H__
#define __CRC_H__

#include "../path/path.h"

// подсчет кс в буфере
unsigned char crc8_buf(uint8_t *buf, uint16_t len);
// расчет одного байта
void crc8_byte(uint8_t *crc, uint8_t data);

#endif

