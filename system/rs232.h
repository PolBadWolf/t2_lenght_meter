/* 
* rs232.h
*
* Created: 31.10.2016 10:07:37
* Author: GumerovMN
*/
#ifndef __PATH_H_
#include "path.h"
#endif


#ifndef __RS232_H__
#define __RS232_H__


namespace ns_rs232
{
	FILE* Init(rsBaud baud, rsParity parity, rsBit bit, rsBufferSize rxSize, rsBufferSize txSize);
	uint8_t ReadByte(uint8_t *byte);
	uint8_t ReadByte(char *byte);
	uint8_t SendByte(uint8_t *byte);
	uint8_t SendByte(uint8_t  byte);
	void String(const char *stroka);
	void String_P(const char *stroka);
	void Digit (uint8_t len, int32_t digit);
	void DigitZ(uint8_t len, int32_t digit);
	void Hex(uint8_t dig);
	// ------------
	void SendTetraByte(uint32_t dat);
	void SendDoubleByte(uint16_t dat);
	void SendDoubleByte(int16_t dat);
	void CrcInit();
	uint8_t CrcRead();
	//
	extern uint8_t rxBufMask;
	extern uint8_t txBufMask;
} //rs232

#endif //__RS232_H__
