/* 
* RS_232.h
*
* Created: 14.03.2023 11:34:16
* Author: Gumerovmn
*/


#ifndef __RS_232_H__
#define __RS_232_H__

#include "Serial.h"
#include "RingBuffer.h"


class RS_232 : public Serial
{
//variables
public:
	static	RS_232	*obj;
protected:
	RingBuffer	*buffInp;
	RingBuffer	*buffOut;
private:

//functions
public:
	RS_232();
	~RS_232();
 	static Serial* init(rsBaud baud, rsParity parity, rsBit bit, uint8_t sizeBuffInp, uint8_t sizeBuffOut);
	void interruptSend();
	void interruptAccept();
	void sendByte(uint8_t dat);
	bool readByte(uint8_t *dat);
protected:
private:
	RS_232( const RS_232 &c );
	RS_232& operator=( const RS_232 &c );

}; //RS_232

#endif //__RS_232_H__
