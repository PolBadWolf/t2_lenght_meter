/* 
* timer0.cpp
*
* Created: 19.10.2016 8:06:54
* Author: GumerovMN
*/


#include "timer2.h"

#ifdef CONF_TIMER_MAIN

#include <avr/interrupt.h>
#include "../device.h"

namespace ns_timerMain
{
#if defined (__AVR_ATmega16__)
	// предделители 1, 8, 32, 64, 128, 256, 1024
	void Init()
	{
		TCCR2 = 0;
		// CTC mode
		TCCR2_WGM21 = 1;
		TCCR2_WGM20 = 0;
		TCCR2_COM21 = 0;
		TCCR2_COM20 = 0;
		// pre Div
		uint16_t prDiv = (uint16_t)(F_CPU/(((uint32_t)FtTimerMain)*((uint32_t)256)));
		if (prDiv <= 1)
		{
			prDiv = 1;
			TCCR2_CS20 = 1;
			TCCR2_CS21 = 0;
			TCCR2_CS22 = 0;
		}
		else if (prDiv <= 8)
		{
			prDiv = 8;
			TCCR2_CS20 = 0;
			TCCR2_CS21 = 1;
			TCCR2_CS22 = 0;
		}
		else if (prDiv <= 32)
		{
			prDiv = 32;
			TCCR2_CS20 = 1;
			TCCR2_CS21 = 1;
			TCCR2_CS22 = 0;
		}
		else if (prDiv <= 64)
		{
			prDiv = 64;
			TCCR2_CS20 = 0;
			TCCR2_CS21 = 0;
			TCCR2_CS22 = 1;
		}
		else if (prDiv <= 128)
		{
			prDiv = 128;
			TCCR2_CS20 = 1;
			TCCR2_CS21 = 0;
			TCCR2_CS22 = 1;
		}
		else if (prDiv <= 256)
		{
			prDiv = 256;
			TCCR2_CS20 = 0;
			TCCR2_CS21 = 1;
			TCCR2_CS22 = 1;
		}
		else
		{
			prDiv = 1024;
			TCCR2_CS20 = 0;
			TCCR2_CS21 = 1;
			TCCR2_CS22 = 0;
		}
		// настройка частоты
		TCCR2_FOC2 = 0;
		TCNT2 = 0;
		uint8_t div = (((unsigned long)F_CPU)/prDiv/FtTimerMain)-1;;
		OCR2 = div;
		TIFR_OCF2 = 0;
		TIFR_TOV2 = 0;
		TIMSK_OCIE2 = 1;
	}
#endif
#if defined (__AVR_ATmega128__)
// предделители 1, 8, 64, 256, 1024
void Init()
{
	TCCR2 = 0;
	// CTC mode
	TCCR2_WGM21 = 1;
	TCCR2_WGM20 = 0;
	TCCR2_COM21 = 0;
	TCCR2_COM20 = 0;
	// pre Div
	uint16_t prDiv = (uint16_t)(F_CPU/(((uint32_t)FtTimerMain)*((uint32_t)256)));
	if (prDiv <= 1)
	{
		prDiv = 1;
		TCCR2_CS20 = 1;
		TCCR2_CS21 = 0;
		TCCR2_CS22 = 0;
	}
	else if (prDiv <= 8)
	{
		prDiv = 8;
		TCCR2_CS20 = 0;
		TCCR2_CS21 = 1;
		TCCR2_CS22 = 0;
	}
	else if (prDiv <= 64)
	{
		prDiv = 64;
		TCCR2_CS20 = 1;
		TCCR2_CS21 = 1;
		TCCR2_CS22 = 0;
	}
	else if (prDiv <= 256)
	{
		prDiv = 256;
		TCCR2_CS20 = 0;
		TCCR2_CS21 = 0;
		TCCR2_CS22 = 1;
	}
	else
	{
		prDiv = 1024;
		TCCR2_CS20 = 1;
		TCCR2_CS21 = 0;
		TCCR2_CS22 = 1;
	}
	// настройка частоты
	TCCR2_FOC2 = 0;
	TCNT2 = 0;
	uint8_t div = (((unsigned long)F_CPU)/prDiv/FtTimerMain)-1;;
	OCR2 = div;
	TIFR_OCF2 = 0;
	TIFR_TOV2 = 0;
	TIMSK_OCIE2 = 1;
}
#endif
	// =============
	ISR(TIMER2_COMP_vect)
	{
		ns_device::Timer_Usr();
	}
}

#endif // CONF_TIMER_MAIN
