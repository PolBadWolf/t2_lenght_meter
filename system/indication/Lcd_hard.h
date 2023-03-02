/* 
* Lcd_hard.h
*
* Created: 14.02.2023 16:24:00
* Author: Gumerovmn
*/

#include "config.h"

#ifdef CONF_LCD

#ifndef __LCD_HARD_H__
#define __LCD_HARD_H__
#include "Lcd.h"


class Lcd_hard : public Lcd
{
//variables
public:
	// указатель записи
	uint8_t		pos;
protected:
private:
	static const char* tabDecoder;

//functions
public:
	Lcd_hard();
	~Lcd_hard();
	FILE* Init(void *obj, uint8_t stolbcov);
protected:
	void SendCommand(uint8_t data);
	void SendData(uint8_t data);
	void Send(uint8_t data);
	void Interrupt();
private:
	Lcd_hard( const Lcd_hard &c );
	Lcd_hard& operator=( const Lcd_hard &c );
}; //Lcd_hard

#endif //__LCD_HARD_H__

#endif // CONF_LCD
