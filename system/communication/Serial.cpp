/* 
* Serial.cpp
*
* Created: 14.03.2023 11:36:44
* Author: Gumerovmn
*/


#include "Serial.h"

void Serial::string(const char *stroka)
{
	char sim;
	while (true)
	{
		sim = *stroka;
		if (sim == 0)	break;
		sendByte(sim);
		stroka++;
	}
}

void Serial::string_P(const char *stroka)
{
	char sim;
	while (true)
	{
		sim = pgm_read_byte(stroka);
		if (sim == 0)	break;
		sendByte(sim);
		stroka++;
	}
}

void Serial::digit(uint8_t len, int32_t digit)
{
	if (digit < 0)
	{
		digitZ(len, digit);
		return;
	}
	char stroka[len + 1];
	stroka[len] = 0;
	uint8_t low;
	// обязательная цифра
	len--;
	low = digit % 10;
	digit /= 10;
	stroka[len] = '0' + low;
	//
	while (len > 0)
	{
		len--;
		if (digit == 0)
		{
			stroka[len] = ' ';
			continue;
		}
		low = digit % 10;
		digit /= 10;
		stroka[len] = '0' + low;
	}
	string(stroka);
}

void Serial::digitZ(uint8_t len, int32_t digit)
{
	char stroka_n[len + 1];
	char *stoka	= stroka_n + len;
	*stoka = 0;
	uint8_t low;
	//
	if (digit < 0)
	{
		stroka_n[0] = '-';
		len--;
		digit = -digit;
	}
	//
	while (len > 0)
	{
		len--;
		stoka--;
		low = digit % 10;
		digit /= 10;
		*stoka = '0' + low;
	}
	string(stroka_n);
}
