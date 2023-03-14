/* 
* rs232.cpp
*
* Created: 31.10.2016 10:07:37
* Author: GumerovMN
*/


#include "rs232.h"

#ifdef CONF_RS232
#include "crc.h"

namespace ns_rs232
{
	uint8_t rxBufMask;
	uint8_t txBufMask;
	// ����� ������
	volatile uint8_t*	RxBuf = NULL;
	volatile uint8_t	RxHead;
	volatile uint8_t	RxTail;
	// ����� ��������
	volatile uint8_t*	TxBuf = NULL;
	volatile uint8_t	TxHead;
	volatile uint8_t	TxTail;
	
	rsBit bit;
	//
	uint8_t	crcDat;
	void CrcSumm(uint8_t *Byte);

	int putChar(char c, FILE *stream)
	{
		// tabWin2Avr
		//if (c >= tabWin2AvrOffset)
		//c = pgm_read_byte(& tabWin2Avr[c - tabWin2AvrOffset]);
		SendByte(c);
		return 1;
	}

#if defined (__AVR_ATmega128__)
	FILE* Init(rsBaud baud, rsParity parity, rsBit bit, rsBufferSize rxSize, rsBufferSize txSize)
	{
		ns_rs232::bit = bit;
		// ��������� ��������
		//uint16_t ubbrr = (uint16_t)( ( ((uint32_t)F_CPU) / ( ((uint32_t)16) * ((uint32_t)baud) ) ) -1 );
		uint16_t ubbrr = (uint16_t)( ( ((uint32_t)F_CPU) / ( ((uint32_t)8) * ((uint32_t)baud) ) ) -1 );  // 2x
		UBRR0H = (uint8_t)(ubbrr >> 8);
		UBRR0L = (uint8_t)(ubbrr);
		UCSR0A_U2X = 1;
		//
		UCSR0B = 0;
		uint8_t tmp = 0;
		// �������� ��������
		tmp |= __port(parity).bit0;
		tmp |= __port(parity).bit1;
		// ��������� ��������
		tmp |= __port(bit).bit0 << UCSZ00;
		tmp |= __port(bit).bit1 << UCSZ01;
		tmp |= __port(bit).bit2 << UCSZ02;
		// ���� ����� 1: 0 << USBS0;
		// ���� ����� 2: 1 << USBS0;
		tmp |= 1 << USBS0;
		UCSR0C = tmp;// | (1 << URSEL);
		// enable UART reciver & transmitte, recive & transmitte interrupt
		UCSR0B_TXEN0 = 1;
		UCSR0B_RXEN0 = 1;
		UCSR0B_RXCIE0 = 1;
		// init buffers
		RxBuf = new uint8_t[rxSize];
		rxBufMask = rxSize - 1;
		TxBuf = new uint8_t[txSize];
		txBufMask = txSize - 1;

		return fdevopen(putChar, NULL);
	}
	// ==========================================
	ISR(USART0_RX_vect)
	{
		uint8_t tmpHead = (RxHead + 1) & rxBufMask;
		if (tmpHead != RxTail)
		{
			RxBuf[RxHead] = UDR0;
			RxHead = tmpHead;
		}
		else tmpHead = UDR0;
	}
	// ==========================================
	ISR(USART0_UDRE_vect)
	{
		if (TxHead == TxTail)
			UCSR0B_UDRIE0 = 0;
		else
		{
			UCSR0B_TXB80 = 1;
			UDR0 = TxBuf[TxTail];
			TxTail = (TxTail + 1) & txBufMask;
		}
	}
	// ==========================================
	uint8_t ReadByte(uint8_t *byte)
	{
		uint8_t tmpHead, tmpTail;
		CRITICAL_SECTION
		{
			tmpHead = RxHead;
			tmpTail = RxTail;
		}
		if (tmpHead == tmpTail)	return 0;
		*byte = RxBuf[RxTail];
		tmpTail = (tmpTail + 1) & rxBufMask;
		CRITICAL_SECTION
		{
			RxTail = tmpTail;
		}
		return 1;
	}
	// ==========================================
	uint8_t ReadByte(char *byte)
	{
		uint8_t tmpHead, tmpTail;
		CRITICAL_SECTION
		{
			tmpHead = RxHead;
			tmpTail = RxTail;
		}
		if (tmpHead == tmpTail)	return 0;
		*byte = RxBuf[RxTail];
		tmpTail = (tmpTail + 1) & rxBufMask;
		CRITICAL_SECTION
		{
			RxTail = tmpTail;
		}
		return 1;
	}
	// ==========================================
	uint8_t SendByte(uint8_t *byte)
	{
		uint8_t tmpTail;
		tmpTail = TxTail;
		uint8_t tmpHead = (TxHead + 1) & txBufMask;
		if (tmpHead == tmpTail)	return 0;
		TxBuf[TxHead] = *byte;
		UCSR0B_UDRIE0 = 1;
		CrcSumm(byte);
		CRITICAL_SECTION
		{ 
			TxHead = tmpHead;
		}
		return 1;
	}
	// ==========================================
	uint8_t SendByte(uint8_t byte)
	{
		uint8_t tmpTail;
		tmpTail = TxTail;
		uint8_t tmpHead = (TxHead + 1) & txBufMask;
		if (tmpHead == tmpTail)	return 0;
		TxBuf[TxHead] = byte;
		UCSR0B_UDRIE0 = 1;
		CrcSumm(&byte);
		CRITICAL_SECTION
		{
			TxHead = tmpHead;
		}
		return 1;
	}
#endif	// MEGA 128

#if defined (__AVR_ATmega16__)

FILE* Init(rsBaud baud, rsParity parity, rsBit bit, rsBufferSize rxSize, rsBufferSize txSize)
{
	ns_rs232::bit = bit;
	// ��������� ��������
	//uint16_t ubbrr = (uint16_t)( ( ((uint32_t)F_CPU) / ( ((uint32_t)16) * ((uint32_t)baud) ) ) -1 );
	uint16_t ubbrr = (uint16_t)( ( ((uint32_t)F_CPU) / ( ((uint32_t)8) * ((uint32_t)baud) ) ) -1 );  // 2x
	UBRRH = (uint8_t)(ubbrr >> 8);
	UBRRL = (uint8_t)(ubbrr);
	UCSRA_U2X = 1;
	//
	UCSRB = 0;
	// �������� ��������
	UCSRC_UPM0 = __port(parity).bit0;
	UCSRC_UPM1 = __port(parity).bit1;
	// ��������� ��������
	UCSRC_UCSZ0 = __port(bit).bit0;
	UCSRC_UCSZ1 = __port(bit).bit1;
	UCSRB_UCSZ2 = __port(bit).bit2;
	// ���� ����� 1: 0 << USBS0;
	// ���� ����� 2: 1 << USBS0;
	UCSRC_USBS = 1;
	UCSRC_URSEL = 1;
	// enable UART reciver & transmitte, recive & transmitte interrupt
	UCSRB_TXEN = 1;
	UCSRB_RXEN = 1;
	UCSRB_RXCIE = 1;
	UCSRB_UDRIE = 1;
	// init buffers
	RxBuf = new uint8_t[rxSize];
	rxBufMask = rxSize - 1;
	TxBuf = new uint8_t[txSize];
	txBufMask = txSize - 1;

	return fdevopen(putChar, NULL);
}
// ==========================================
ISR(USART_RXC_vect)
{
	uint8_t tmpHead = (RxHead + 1) & rxBufMask;
	if (tmpHead != RxTail)
	{
		RxBuf[RxHead] = UDR;
		RxHead = tmpHead;
	}
	else tmpHead = UDR;
}
// ==========================================
ISR(USART_UDRE_vect)
{
	if (TxHead == TxTail)
	{
		UCSRB_UDRIE = 0;
	}
	else
	{
		UDR = TxBuf[TxTail];
		TxTail = (TxTail + 1) & txBufMask;
// 		UCSRB_TXCIE = 1;
	}
}
// ==========================================
uint8_t ReadByte(uint8_t *byte)
{
	uint8_t tmpHead, tmpTail;
	CRITICAL_SECTION
	{
		tmpHead = RxHead;
		tmpTail = RxTail;
	}
	if (tmpHead == tmpTail)	return 0;
	*byte = RxBuf[RxTail];
	tmpTail = (tmpTail + 1) & rxBufMask;
	CRITICAL_SECTION
	{
		RxTail = tmpTail;
	}
	return 1;
}
// ==========================================
uint8_t ReadByte(char *byte)
{
	uint8_t tmpHead, tmpTail;
	CRITICAL_SECTION
	{
		tmpHead = RxHead;
		tmpTail = RxTail;
	}
	if (tmpHead == tmpTail)	return 0;
	*byte = RxBuf[RxTail];
	tmpTail = (tmpTail + 1) & rxBufMask;
	CRITICAL_SECTION
	{
		RxTail = tmpTail;
	}
	return 1;
}
// ==========================================
uint8_t SendByte(uint8_t *byte)
{
	uint8_t tmpTail;
	tmpTail = TxTail;
	uint8_t tmpHead = (TxHead + 1) & txBufMask;
	if (tmpHead == tmpTail)	return 0;
	TxBuf[TxHead] = *byte;
 	UCSRB_UDRIE = 1;
	CrcSumm(byte);
	CRITICAL_SECTION
	{
		TxHead = tmpHead;
	}
	return 1;
}
// ==========================================
uint8_t SendByte(uint8_t byte)
{
	uint8_t tmpTail;
	tmpTail = TxTail;
	uint8_t tmpHead = (TxHead + 1) & txBufMask;
	if (tmpHead == tmpTail)	return 0;
	TxBuf[TxHead] = byte;
  	UCSRB_UDRIE = 1;
	CrcSumm(&byte);
	CRITICAL_SECTION
	{
		TxHead = tmpHead;
	}
	return 1;
}

#undef UDR__enable

#endif	// MEGA 16

	// ==================================================
	void String(const char *stroka)
	{
		char sim;
		while (true)
		{
			sim = *stroka;
			if (sim == 0) break;
			SendByte(sim);
			stroka++;
		}
	}
	// ==================================================
	void String_P(const char *stroka)
	{
		char sim;
		while (true)
		{
			sim = pgm_read_byte(stroka);
			if (sim == 0) break;
			SendByte(sim);
			stroka++;
		}
	}
	// ==================================================
	void Digit(uint8_t len, int32_t digit)
	{
		if (digit < 0) { DigitZ(len, digit); return; }
// 		char sss[24+2];
		char sss[len + 3];
		sss[len + 1] = 0;
		//len--;

		uint8_t low = digit % 10;
		sss[len] = '0' + low;
		digit /= 10;
		while(len-- > 0)
		{
			low = digit % 10;
			if ( (low == 0) && (digit == 0) )
				sss[len] = ' ';
			else
				sss[len] = '0' + low;
			digit /= 10;
		}
		String(sss);
	}
	void DigitZ(uint8_t len, int32_t digit)
	{
		char sss[len + 3];
		volatile uint8_t por = 0;
		if (digit < 0)
		{
			por = 1;
			digit = -digit;
		}
		sss[len] = 0;
		//len--;

		volatile uint8_t low;// = digit % 10;
		//sss[len] = '0' + low;
		//digit /= 10;
		//len--;
		while(len-- > por)
		{
			low = digit % 10;
			sss[len] = '0' + low;
			digit /= 10;
		}
		sss[len] = '-';
		String(sss);
	}
	// ==================================================
	char Tetra(uint8_t dat)
	{
		dat &= 0x0f;
		if (dat>9)
		dat += 7;
		dat += '0';
		return dat;
	}
	// ==================================================
	void Hex(uint8_t dig)
	{
		SendByte( Tetra( (*((volatile BYTE_TETRA*)(&dig))).High ) );
		SendByte( Tetra( (*((volatile BYTE_TETRA*)(&dig))).Low ) );
	}
	// ==================================================
	void CrcInit()
	{
		crcDat = 0xff;
		//crcDat = 0;
	}
	void CrcSumm(uint8_t *Byte)
	{
		crc8_byte(&crcDat, *Byte);
		//crcDat++;
	}
	uint8_t CrcRead()
	{
		return crcDat;
	}
	// ==================================================
	void SendTetraByte(uint32_t dat)
	{
		SendByte( ((uint8_t *)&dat)[0]  );
		SendByte( ((uint8_t *)&dat)[1]  );
		SendByte( ((uint8_t *)&dat)[2]  );
		SendByte( ((uint8_t *)&dat)[3]  );
	}
	void SendDoubleByte(uint16_t dat)
	{
		SendByte( ((uint8_t *)&dat)[0]  );
		SendByte( ((uint8_t *)&dat)[1]  );
	}
	void SendDoubleByte(int16_t dat)
	{
		SendByte( ((uint8_t *)&dat)[0]  );
		SendByte( ((uint8_t *)&dat)[1]  );
	}
	// ==================================================
}
#endif // CONF_RS232
