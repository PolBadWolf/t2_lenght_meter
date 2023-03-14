/* 
* Serial.h
*
* Created: 14.03.2023 11:36:44
* Author: Gumerovmn
*/


#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "path/path.h"

class Serial
{
//functions
public:
// 	virtual ~Serial();
 	virtual bool readByte(uint8_t *dat);
	virtual void sendByte(uint8_t dat);
	//
	void string(const char *stroka);
	void string_P(const char *stroka);
	//
	void digit(uint8_t len, int32_t digit);
	void digitZ(uint8_t len, int32_t digit);
protected:

}; //Serial

#endif //__SERIAL_H__
