/* 
* Lcd.cpp
*
* Created: 14.02.2023 16:20:09
* Author: Gumerovmn
*/


#include "Lcd.h"

#ifdef CONF_LCD

#include "../path/path.h"

void Lcd::PutChar(uint8_t pos, char sim)
{
	PutChar(&pos, sim);
}

void Lcd::FlashFn(uint8_t *pos, volatile bool *onOff)
{
	if (*onOff)
		scrFlash[(*pos)/c_stolbcov] |=  (((uint32_t)1) << ((*pos)%c_stolbcov));
	else
		scrFlash[(*pos)/c_stolbcov] &= ~(((uint32_t)1) << ((*pos)%c_stolbcov));
}

void Lcd::NormalizePos(uint8_t *poz)
{
	while(*poz >= maxPos)
	{
		*poz = *poz - maxPos;
	}
}

void Lcd::PutChar(uint8_t *pos, char sim)
{
	NormalizePos(pos);
	FlashFn(pos, &flicker);
	screen[*pos] = sim;
	(*pos)++;
}

void Lcd::Clear()
{
	for (uint8_t i = 0; i < maxPos; i++) { screen[i] = ' '; }
	scrFlash[0] = 0; scrFlash[1] = 0;
	pos = 0;
	flicker = false;
}

void Lcd::String(uint8_t *pos, const char *stroka)
{
	uint8_t max = maxPos;
	while ((*stroka) > 0)
	{
		NormalizePos(pos);
		FlashFn(pos, &flicker);
		screen[*pos] = (*stroka);
		(*pos)++;
		stroka++;
		if (--max == 0)	break;
	}
}
// --
void Lcd::String_P(uint8_t *pos, const char *stroka)
{
	uint8_t max = maxPos;
	char sim;
	while ((sim = pgm_read_byte(stroka)) > 0)
	{
		NormalizePos(pos);
		FlashFn(pos, &flicker);
		screen[*pos] = sim;
		(*pos)++;
		stroka++;
		if (--max == 0) break;
	}
}

void Lcd::DigitZ(uint8_t *pos, uint8_t len, uint32_t digit)
{
	*pos += len;
	NormalizePos(pos);
	int8_t poz = *pos - 1;
	char low;
	while (len-- > 0)
	{
		low = digit % 10;
		if (poz < 0) poz += maxPos;
		FlashFn((uint8_t *) &poz, &flicker);
		screen[poz--] = '0' + low;
		digit /= 10;
	}
}

void Lcd::Digit(uint8_t *pos, uint8_t len, uint32_t digit)
{
	*pos += len;
	NormalizePos(pos);
	int8_t poz = *pos - 1;
	char low = digit % 10;
	if (poz < 0) poz += maxPos;
	FlashFn((uint8_t *) & poz, &flicker);
	screen[poz--] = low + '0';
	digit /= 10;
	len--;
	while(len-- > 0)
	{
		low = digit % 10;
		if (poz < 0) poz += maxPos;
		FlashFn((uint8_t *) & poz, &flicker);
		if ( (low == 0) && (digit == 0) ) screen[poz] = ' ';
		else screen[poz] = low + '0';
		poz--;
		digit /= 10;
	}
}

void Lcd::DigitM(uint8_t pos, uint8_t len, int32_t digit)
{
	if (digit < 0)
	{
		PutChar(pos++, '-');
		NormalizePos(&pos);
		DigitZ(&pos, len - 1, (uint32_t)(-digit));
	}
	else DigitZ(&pos, len, digit);
}

char Lcd::Tetra(uint8_t dat)
{
	dat &= 0x0f;
	if (dat > 9) dat += 7;
	dat += '0';
	return dat;
}

void Lcd::Hex(uint8_t *pos, uint8_t dig)
{
	PutChar(*pos, Tetra( (* ((volatile BYTE_TETRA*)(&dig))).High) );
	(*pos)++;
	PutChar(*pos, Tetra( (* ((volatile BYTE_TETRA*)(&dig))).Low) );
	(*pos)++;
	NormalizePos(pos);
}

void Lcd::ShowT(uint8_t *pos, uint8_t c, uint8_t d, float digit)
{
	if (digit > 0)
	{
		PutChar(pos, '+');
		c--;
		Digit(pos, c, (uint16_t)digit);
	}
	if (digit < 0)
	{
		PutChar(pos, '-');
		c--;
		digit = -digit;
		Digit(pos, c, (uint16_t)digit);
	}
	if (digit == 0) Digit(pos, c, 0);
	if (d > 0)
	{
		PutChar(pos, '.');
		uint16_t k = 1;
		for (uint8_t n = 0; n < d; n++) k *= 10;
		uint16_t dr = (digit - ((uint16_t) digit)) * k;
		DigitZ(pos, d, dr);
	}
}

void Lcd::ShowT1(uint8_t *pos, uint8_t c, uint8_t d, float digit)
{
	if (digit > 0)
	{
		Digit(pos, c, (uint16_t)digit);
	}
	if (digit < 0)
	{
		PutChar(pos, '-');
		c--;
		digit = -digit;
		Digit(pos, c, (uint16_t)digit);
	}
	if (digit == 0) Digit(pos, c, 0);
	if (d > 0)
	{
		PutChar(pos, '.');
		uint16_t k = 1;
		for (uint8_t n = 0; n < d; n++) k *= 10;
		uint16_t dr = (digit - ((uint16_t) digit)) * k;
		DigitZ(pos, d, dr);
	}
}

uint8_t Lcd::SetPosition(uint8_t x, uint8_t y)
{
	uint8_t pos = (y * c_stolbcov) + x;
	NormalizePos(&pos);
	return pos;
}

uint8_t Lcd::GetStolbcov()
{
	return c_stolbcov;
}

#endif // CONF_LCD