/* 
* LcdKey_Timer.cpp
*
* Created: 04.02.2017 11:34:08
* Author: User
*/


#include "config.h"

#ifdef CONF_TIMER_LCDKEY

// #include "path.h"
#include "../device.h"
// #include "LcdKey_Timer.h"
// #include <avr/interrupt.h>

// timer0 - ������������ 1, 8, 64, 256, 1024


namespace ns_LcdKeyTimer
{
	// ========================
	void Init()
	{
		volatile uint16_t prDiv;
#if defined (__AVR_ATmega128__)
		TCCR0 = 0;
		// CTC mode
		TCCR0_WGM01 = 1;
		TCCR0_WGM00 = 0;
		TCCR0_COM01 = 0;
		TCCR0_COM00 = 0;
		// pre Div
		prDiv = (uint16_t)( ((uint32_t)F_CPU) / (  ((uint32_t)FT_LcdKeyTimer) * ((uint32_t)256) * ((uint32_t)1) )  );
		if (prDiv <= 1)
		{
			prDiv = 1;
			TCCR0_CS02 = 0;
			TCCR0_CS01 = 0;
			TCCR0_CS00 = 1;
		}
		else if (prDiv <= 8)
		{
			prDiv = 8;
			TCCR0_CS02 = 0;
			TCCR0_CS01 = 1;
			TCCR0_CS00 = 0;
		}
		else if (prDiv <= 32)
		{
			prDiv = 32;
			TCCR0_CS02 = 0;
			TCCR0_CS01 = 1;
			TCCR0_CS00 = 1;
		}
		else if (prDiv <= 64)
		{
			prDiv = 64;
			TCCR0_CS02 = 1;
			TCCR0_CS01 = 0;
			TCCR0_CS00 = 0;
		}
		else if (prDiv <= 128)
		{
			prDiv = 128;
			TCCR0_CS02 = 1;
			TCCR0_CS01 = 0;
			TCCR0_CS00 = 1;
		}
		else if (prDiv <= 256)
		{
			prDiv = 256;
			TCCR0_CS02 = 1;
			TCCR0_CS01 = 1;
			TCCR0_CS00 = 0;
		}
		else
		{
			prDiv = 1024;
			TCCR0_CS02 = 1;
			TCCR0_CS01 = 1;
			TCCR0_CS00 = 1;
		}

		// ��������� �������
		TCCR0_FOC0 = 1;
		TCNT0 = 0;
		//uint8_t div = (((unsigned long)C_Fosc) / prDiv / FT_LcdKeyTimer) - 1;
		uint8_t div = (uint8_t)( ((uint32_t)F_CPU) / ( ((uint32_t)prDiv) * ((uint32_t)FT_LcdKeyTimer) * ((uint32_t)1) ) ) - 1;
		OCR0 = div;
		TIFR_OCF0 = 0;
		TIFR_TOV0 = 0;
		TIMSK_OCIE0 = 1;
#elif defined (__AVR_ATmega16__)
		TCCR0 = 0;
		// CTC mode
		TCCR0_WGM01 = 1;
		TCCR0_WGM00 = 0;
		TCCR0_COM01 = 0;
		TCCR0_COM00 = 0;
		// pre Div
		prDiv = (uint16_t)( ((uint32_t)F_CPU) / (  ((uint32_t)FT_LcdKeyTimer) * ((uint32_t)256) * ((uint32_t)1) )  );
		if (prDiv <= 1)
		{
			prDiv = 1;
			TCCR0_CS02 = 0;
			TCCR0_CS01 = 0;
			TCCR0_CS00 = 1;
		}
		else if (prDiv <= 8)
		{
			prDiv = 8;
			TCCR0_CS02 = 0;
			TCCR0_CS01 = 1;
			TCCR0_CS00 = 0;
		}
		else if (prDiv <= 64)
		{
			prDiv = 64;
			TCCR0_CS02 = 0;
			TCCR0_CS01 = 1;
			TCCR0_CS00 = 1;
		}
		else if (prDiv <= 256)
		{
			prDiv = 256;
			TCCR0_CS02 = 1;
			TCCR0_CS01 = 0;
			TCCR0_CS00 = 0;
		}
		else
		{
			prDiv = 1024;
			TCCR0_CS02 = 1;
			TCCR0_CS01 = 0;
			TCCR0_CS00 = 1;
		}

		// ��������� �������
		TCCR0_FOC0 = 1;
		TCNT0 = 0;
		//uint8_t div = (((unsigned long)C_Fosc) / prDiv / FT_LcdKeyTimer) - 1;
//		uint8_t div = (uint8_t)( ((uint32_t)F_CPU) / ( ((uint32_t)prDiv) * ((uint32_t)FT_LcdKeyTimer) * ((uint32_t)1) ) ) - 1;
		uint8_t div = (uint8_t)( ((uint32_t)F_CPU) / ( ((uint32_t)prDiv) * ((uint32_t)FT_LcdKeyTimer) * ((uint32_t)2) ) ) - 1;
		OCR0 = div;
		TIFR_OCF0 = 0;
		TIFR_TOV0 = 0;
		TIMSK_OCIE0 = 1;
#else
#error no support chip in LcdKey_Timer
#endif
	}

	ISR (TIMER0_COMP_vect)
	{
		TIMSK_OCIE0 = 0;
		sei();
		ns_device::Timer_lcd();
		TIMSK_OCIE0 = 1;
	}

}

#endif // CONF_TIMER_LCDKEY
