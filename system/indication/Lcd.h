/* 
* Lcd.h
*
* Created: 14.02.2023 16:20:09
* Author: Gumerovmn
*/

#include "config.h"

#ifdef CONF_LCD

#ifndef __LCD_H__
#define __LCD_H__

#include "../path/path.h"

class Lcd
{
// переменные
public:
	char*		screen;				// массив копия экрана
	uint32_t	scrFlash[2];		// массив битовая копия позиций экрана для моргания
	volatile bool flicker;			// вывод с миганием
	uint8_t		pos;
protected:
	uint8_t		c_strok;
	uint8_t		c_stolbcov;
	uint8_t		maxPos;
	uint8_t		indxRefresh;
	uint16_t	scrFlashCount;		// счетчик тиков для моргания
	bool		scrFlashStat;		// кол-во тиков для
	uint32_t*	scrFlashPtr;
	const uint16_t scrFlashMaxMsec = 1000;
// функции
protected:
// 	virtual ~Lcd();
	virtual FILE* Init(void *obj, uint8_t stolbcov)=0;
	void NormalizePos(uint8_t *poz);
	char Tetra(uint8_t dat);
public:
	// загрузка символа
	void PutChar(uint8_t *pos, char sim);
	void PutChar(uint8_t pos, char sim);
	void FlashFn(uint8_t *pos, volatile bool *onOff);
	void Clear();
	// --
	void String(uint8_t *pos, const char *stroka);
	void String(uint8_t  pos, const char *stroka)			{ String(&pos, stroka); }
	void String(const char *stroka)							{ String(&pos, stroka); }
	// --
	void String_P(uint8_t *pos, const char *stroka);
	void String_P(uint8_t  pos, const char *stroka)			{ String_P(&pos, stroka); }
	void String_P(const char *stroka)						{ String_P(&pos, stroka); }
	// --
	void DigitZ(uint8_t *pos, uint8_t len, uint32_t digit);
	void DigitZ(uint8_t  pos, uint8_t len, uint32_t digit)	{ DigitZ(&pos, len, digit); }
	// --
	void Digit(uint8_t *pos, uint8_t len, uint32_t digit);
	void Digit(uint8_t  pos, uint8_t len, uint32_t digit)	{ Digit(&pos, len, digit); }
	// --
	void DigitM(uint8_t pos, uint8_t len, int32_t digit);
	// --
	void Hex(uint8_t *pos, uint8_t dig);
	void Hex(uint8_t  pos, uint8_t dig)						{ Hex(&pos, dig); }
	// --
	void ShowT(uint8_t *pos, uint8_t c, uint8_t d, float digit);
	void ShowT(uint8_t  pos, uint8_t c, uint8_t d, float digit)		{ ShowT(pos, c, d, digit); }
	void ShowT1(uint8_t *pos, uint8_t c, uint8_t d, float digit);
	void ShowT1(uint8_t  pos, uint8_t c, uint8_t d, float digit)	{ ShowT1(pos, c, d, digit); }
	// --
	uint8_t SetPosition(uint8_t x, uint8_t y);
	uint8_t GetStolbcov();
	// --
	virtual void Interrupt();
}; //Lcd

#endif //__LCD_H__

#endif // CONF_LCD
