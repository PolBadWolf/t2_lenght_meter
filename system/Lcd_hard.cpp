/* 
* Lcd_hard.cpp
*
* Created: 14.02.2023 16:24:00
* Author: Gumerovmn
*/


#include "Lcd_hard.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
#include <stddef.h>
#include <stdio.h>

#ifdef CONF_LCD

#if  defined (__AVR_ATmega128__)
typedef struct// BYTE_LCD_DATAs
{
	unsigned data:4;
} BYTE_LCD_DATA;
#define LCD_D_DDR	(*((volatile BYTE_LCD_DATA*)(&DDRA))).data
#define LCD_D		(*((volatile BYTE_LCD_DATA*)(&PORTA))).data
//#define LCD_D_PIN	(*((volatile BYTE_LCD_DATA*)(&PINA))

#define LCD_EI_DDR		__port(DDRA).bit4
#define LCD_EI			__port(PORTA).bit4

#define LCD_RS_DDR		__port(DDRA).bit5
#define LCD_RS			__port(PORTA).bit5
#else
//--
#if  defined (__AVR_ATmega16__)
typedef struct// BYTE_LCD_DATAs
{
	unsigned data:4;
} BYTE_LCD_DATA;
#define LCD_D_DDR	(*((volatile BYTE_LCD_DATA*)(&DDRB))).data
#define LCD_D		(*((volatile BYTE_LCD_DATA*)(&PORTB))).data
//#define LCD_D_PIN	(*((volatile BYTE_LCD_DATA*)(&PINA))

#define LCD_EI_DDR		__port(DDRB).bit5
#define LCD_EI			__port(PORTB).bit5

#define LCD_RS_DDR		__port(DDRB).bit4
#define LCD_RS			__port(PORTB).bit4
#else
#error No config for LCD_HARD
#endif
//--
#endif
//--
const char tabDecoder_local[] PROGMEM  =
{
	200,	201,	202,	203,	215,	216,	217,	218,	219,	220,	237,	253,	238,	239,	240,	241,
	242,	244,	204,	205,	206,	207,	208,	209,	210,	211,	212,	213,	214,	221,	222,	223,
	231,	232,	233,	234,	235,	236,	243,	245,	162,	246,	247,	248,	249,	250,	251,	252,
	254,	255,	32,		32,		32,		32,		32,		32,		181,	32,		32,		32,		32,		32,		32,		32,
	65,		160,	66,		161,	224,	69,		163,	164,	165,	166,	75,		167,	77,		72,		79,		168,
	80,		67,		84,		169,	170,	88,		225,	171,	172,	226,	173,	174,	98,		175,	176,	177,
	97,		178,	179,	180,	227,	101,	182,	183,	184,	185,	186,	187,	188,	189,	111,	190,
	112,	99,		191,	121,	228,	120,	229,	192,	193,	230,	194,	195,	196,	197,	198,	199
};
const char* Lcd_hard::tabDecoder = (char*)tabDecoder_local;

// default constructor
Lcd_hard *thisObj = nullptr;
Lcd_hard::Lcd_hard()
{
	c_strok = 2;
	c_stolbcov = 16;
	maxPos = c_strok * c_stolbcov;
	scrFlashStat = false;
	indxRefresh = 0;
} //Lcd_hard

// default destructor
Lcd_hard::~Lcd_hard()
{
} //~Lcd_hard

int putCharX(char c, FILE *stream)
{
	uint8_t *p;
	p = &(thisObj->pos);
	thisObj->PutChar(p, c);
	return 1;
}
FILE* Lcd_hard::Init(void *obj, uint8_t stolbcov)
{
	// настройка портов
	LCD_D_DDR = 0x0f;
	LCD_D = 0x0f;
	LCD_EI_DDR = 1;
	LCD_EI = 1;
	LCD_RS_DDR = 1;
	LCD_RS = 1;
	__delay_ms(20);
	//
	c_stolbcov = stolbcov;
	maxPos = c_strok * c_stolbcov;
	//
	screen = new char[maxPos];
	{
		uint8_t lenF = 2; //sizeof(scrFlash) / sizeof(scrFlash[0]);
		for (uint8_t i = 0; i < lenF; i++)
		{
			scrFlash[i] = 'A';
		}
	}
	pos = 0;
	scrFlashCount = scrFlashMaxMsec;
	// настройка индикатора
	SendCommand(0x28);					// 4-ех разрядная шина, 2 строки
	__delay_ms(2);
	SendCommand(0x28);					// 4-ех разрядная шина, 2 строки
	__delay_ms(2);
	SendCommand(0x0c);					// дисплей, мерцание включено
	SendCommand(0x01);					// очистка дисплея
	__delay_ms(2);
	SendCommand(0x06);					// курсор движется вправо, сдвига нет
	SendCommand(0x02);					// 
	__delay_ms(2);
	SendCommand(0x80);					//
	__delay_ms(2);
	// загрузка символов
	//
	thisObj = (Lcd_hard *)obj;
	return fdevopen(putCharX, NULL);
}


void Lcd_hard::SendCommand(uint8_t data)
{
	CRITICAL_SECTION
	{
		LCD_RS = 0;
		__delay_us(50);
		Send(data);
	}
}
void Lcd_hard::SendData(uint8_t data)
{
	CRITICAL_SECTION
	{
		LCD_RS = 1;
		__delay_us(50);
		Send(data);
	}
}
void Lcd_hard::Send(uint8_t data)
{
	LCD_D = ((data >> 4) & 0x0f);
	LCD_EI = 1;
	__builtin_avr_delay_cycles(10);
	LCD_EI = 0;
	__builtin_avr_delay_cycles(10);
	LCD_D = (data & 0x0f);
	LCD_EI = 1;
	__builtin_avr_delay_cycles(10);
	LCD_EI = 0;
	__builtin_avr_delay_cycles(10);
}

void Lcd_hard::Interrupt()
{
	// моргание
	scrFlashCount--;
	if (scrFlashCount == 0)
	{
		scrFlashCount = scrFlashMaxMsec;
		scrFlashStat = !scrFlashStat;
	}
	
	indxRefresh++;
	if (indxRefresh >= maxPos)
	{
		indxRefresh = 0;
		scrFlashPtr = &scrFlash[0];
		SendCommand(0x80);
	}
	if (indxRefresh == c_stolbcov)
	{
		scrFlashPtr = &scrFlash[1];
		SendCommand(0xc0);
	}
	__delay_us(50);
	
	uint8_t tmp = screen[indxRefresh];
	if (tmp >= 128)
		tmp = pgm_read_byte(& tabDecoder[tmp - 128]);
	
	{
		if ( ((*scrFlashPtr) & (((uint32_t)1)<<(indxRefresh % c_stolbcov))) && scrFlashStat)
//		if ( ( scrFlash[indxRefresh / c_stolbcov] & ( 1<< (indxRefresh % c_stolbcov)) ) && scrFlashStat )
		{
 			if (tmp >= 8 && tmp <= ' ') tmp = 255;
 			else
			tmp = ' ';
		}
	}
	SendData(tmp);
}

// static const char* Lcd_hard::tabDecoder = 
// {
// 	0, 1 
// };

//int* Lcd_hard::a = new int {1, 2};


#endif // CONF_LCD
