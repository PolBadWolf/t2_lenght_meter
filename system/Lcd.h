/* 
* Lcd.h
*
* Created: 04.02.2017 12:04:57
* Author: User
*/


#ifndef __LCD_H__
#define __LCD_H__

#ifndef __PATH_H_
#include "path.h"
#endif

namespace ns_lcd
{
	// var
	// копия экрана
	extern char*	screen;
	// указатель записи
	extern unsigned char pos;
	// с миганием
	extern volatile bool flicker;
	uint8_t Stolbcov();
	// ---------------------------------------
	// Fn
	FILE* Init(uint8_t stolbcov);
	void FlashFn(uint8_t* pos, volatile bool* onOff);
	void Interrupt();
	void Clear();
	void PutChar(uint8_t *pos, char sim);
	void PutChar(uint8_t pos, char sim);
	uint8_t SetPosition(uint8_t x, uint8_t y);
	void String(uint8_t *pos, const char *stroka);
	void String(uint8_t pos, const char *stroka);
	void String(const char *stroka);
	void String_P(uint8_t *pos, const char *stroka);
	void String_P(uint8_t pos, const char *stroka);
	void String_P(const char *stroka);
	void DigitZ(uint8_t *pos, uint8_t len, uint32_t digit);
	void DigitZ(uint8_t pos, uint8_t len, uint32_t digit);
	void DigitZ_16(uint8_t pos, uint8_t len, char *buf, uint16_t digit);
	void Digit(uint8_t *pos, uint8_t len, uint32_t digit);
	void Digit(uint8_t pos, uint8_t len, uint32_t digit);
	void DigitM(uint8_t pos, uint8_t len, int32_t digit);
	char Tetra(uint8_t dat);
	void Hex(uint8_t* pos, uint8_t dig);
	void Hex(uint8_t pos, uint8_t dig);
	void ShowT(uint8_t *pos, uint8_t c, uint8_t d, float digit);
	void ShowT(uint8_t pos, uint8_t c, uint8_t d, float digit);
	void ShowT1(uint8_t *pos, uint8_t c, uint8_t d, float digit);
	void ShowT1(uint8_t pos, uint8_t c, uint8_t d, float digit);
}


#endif //__LCD_H__
