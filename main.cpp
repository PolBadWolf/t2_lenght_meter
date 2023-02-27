/*
 * t2_lenght_meter.cpp
 *
 * Created: 14.02.2023 13:13:59
 * Author : Gumerovmn
 */ 

#include <avr/io.h>
#include "device.h"

#define SCR ns_device::scr
#define KEY ns_device::key

int main(void)
{
#ifdef CONF_MENU
	while (true)
	{
		ns_device::MainCicle();
	}
#endif
#ifndef CONF_MENU
	int16_t x_count = 0;
	ns_device::Init();
	sei();
	SCR->Clear();
	SCR->String_P((uint8_t)0, PSTR("Hello World !!!") );
	SCR->String_P((uint8_t)25, PSTR("Мир") );
	
	/* Replace with your application code */
	char ss = 0;
	uint8_t p[] = {16};
	while (1)
	{
		__delay_ms(1);
		if (++x_count == 800)
		{
			x_count = 0;
			ss++;
		}
		p[0] = 16;
		SCR->Digit(p, 3, ss);
		SCR->PutChar((*p)++, ':');
		SCR->Hex(p, ss);
#ifdef CONF_KEY4
		SCR->Hex(14, KEY->InKey());
#endif // CONF_KEY4
		//		SCRN->DigitZ(23, 5, SCRN->scrFlashPtr_x);
		//ns_device::scr->Interrupt();
	}
#endif
}

