/* 
* RS_232.cpp
*
* Created: 14.03.2023 11:34:15
* Author: Gumerovmn
*/


#include "RS_232.h"

#ifdef CONF_RS232

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

// =========================================================================
#if defined (__AVR_ATmega16__)
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
#endif	// defined (__AVR_ATmega16__)
// =========================================================================
#if defined (__AVR_ATmega128__)
Serial* RS_232::init(rsBaud baud, rsParity parity, rsBit bit, uint8_t sizeBuffInp, uint8_t sizeBuffOut)
{
	RS_232 *rs = new RS_232();
	rs->buffInp = new RingBuffer(new uint8_t[sizeBuffInp], sizeBuffInp);
	rs->buffOut = new RingBuffer(new uint8_t[sizeBuffOut], sizeBuffOut);
	uint16_t ubrr = (F_CPU / 16 / baud) - 1;
	UBRR0H = (ubrr >> 8) & 0xff;
	UBRR0L = ubrr & 0xff;
	//
	uint8_t	tmp = 0;
	UCSR0A = tmp;
	//
	tmp = 0;
	tmp |= 1 << RXEN0;	// разрешение на прием
	tmp |= 1 << TXEN0;	// разрешение на передачу
	tmp |= 1 << RXCIE0;	// прерывание на прием
	UCSR0B = tmp;
	// 	UCSRC
	tmp = 0;
	switch (parity)
	{
		case EVEN:
		tmp |= 1 << UPM01;
		break;
		case ODD:
		tmp |= 1 << UPM01;
		tmp |= 1 << UPM00;
		break;
		default:
		break;
	}
	tmp |= 1 << USBS0;		// 2 стоп бита
	switch (bit)
	{
		case bit5:
		break;
		case bit6:
		tmp |= 1 << UCSZ00;
		break;
		case bit7:
		tmp |= 1 << UCSZ01;
		break;
		case bit8:
		tmp |= 1 << UCSZ00;
		tmp |= 1 << UCSZ01;
		break;
	}
	UCSR0C = tmp;
	
	return	rs;
}

void RS_232::sendByte(uint8_t dat)
{
	buffOut->push(dat);
	UCSR0B_UDRIE0 = 1;	// прерывание при отсутствие данных для отправки
}

ISR(USART0_UDRE_vect)
{
	RS_232::obj->interruptSend();
}

void RS_232::interruptSend()
{
	uint8_t dat;
	if ( buffOut->pop(&dat) )
	{
		UCSR0B_UDRIE0 = 0;	// отключение прерывания при отсутствие данных для отправки
		return;
	}
	UDR0 = dat;
}

ISR(USART0_RX_vect)
{
	RS_232::obj->interruptAccept();
}

void RS_232::interruptAccept()
{
	uint8_t dat = UDR0;
	buffInp->push(dat);
}

bool RS_232::readByte(uint8_t *dat)
{
	return buffInp->pop(dat);
}
#endif	// defined (__AVR_ATmega128__)
// =========================================================================

#endif // CONF_RS232
