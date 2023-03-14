/* 
* RS_232.cpp
*
* Created: 14.03.2023 11:34:15
* Author: Gumerovmn
*/


#include "RS_232.h"

RS_232	*RS_232::obj;

// default constructor
RS_232::RS_232()
{
	obj = this;
} //RS_232

// default destructor
RS_232::~RS_232()
{
	delete[]	buffInp;
	delete[]	buffOut;
} //~RS_232

Serial* RS_232::init(rsBaud baud, rsParity parity, rsBit bit, uint8_t sizeBuffInp, uint8_t sizeBuffOut)
{
	RS_232 *rs = new RS_232();
 	rs->buffInp = new RingBuffer(new uint8_t[sizeBuffInp], sizeBuffInp);
	rs->buffOut = new RingBuffer(new uint8_t[sizeBuffOut], sizeBuffOut);
	uint16_t ubrr = (F_CPU / 16 / baud) - 1;
	UBRRH = (ubrr >> 8) & 0xff;
	UBRRL = ubrr & 0xff;
	//
	uint8_t	tmp = 0;
 	UCSRA = tmp;
// 	
	tmp = 0;
	tmp |= 1 << RXEN;	// разрешение на прием
	tmp |= 1 << TXEN;	// разрешение на передачу
	tmp |= 1 << RXCIE;	// прерывание на прием
	UCSRB = tmp;
// 	UCSRC
	tmp = 0;
	tmp |= 1 << URSEL;
	switch (parity)
	{
		case EVEN:
			tmp |= 1 << UPM1;
			break;
		case ODD:
			tmp |= 1 << UPM1;
			tmp |= 1 << UPM0;
			break;
		default:
			break;
	}
	tmp |= 1 << USBS;		// 2 стоп бита
	switch (bit)
	{
		case bit5:
			break;
		case bit6:
			tmp |= 1 << UCSZ0;
			break;
		case bit7:
			tmp |= 1 << UCSZ1;
			break;
		case bit8:
			tmp |= 1 << UCSZ0;
			tmp |= 1 << UCSZ1;
			break;
	}
	UCSRC = tmp;
	
 	return	rs;
}

void RS_232::sendByte(uint8_t dat)
{
	buffOut->push(dat);
	UCSRB_UDRIE = 1;	// прерывание при отсутствие данных для отправки
}

ISR(USART_UDRE_vect)
{
	RS_232::obj->interruptSend();
}

void RS_232::interruptSend()
{
	uint8_t dat;
	if ( buffOut->pop(&dat) )
	{
		UCSRB_UDRIE = 0;	// отключение прерывания при отсутствие данных для отправки
		return;
	}
	UDR = dat;
}

ISR(USART_RXC_vect)
{
	RS_232::obj->interruptAccept();
}

void RS_232::interruptAccept()
{
	uint8_t dat = UDR;
	buffInp->push(dat);
}

bool RS_232::readByte(uint8_t *dat)
{
	return buffInp->pop(dat);
}
