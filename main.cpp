/*
 * t2_lenght_meter.cpp
 *
 * Created: 14.02.2023 13:13:59
 * Author : Gumerovmn
 */ 

#include <avr/io.h>
#include "device.h"

#define SCRN ns_device::scr

int main(void)
{
	ns_device::Init();
	sei();
	SCRN->Clear();
	SCRN->String_P((uint8_t)0, PSTR("Hello World !!!") );
	SCRN->String_P((uint8_t)25, PSTR("Мир") );
	
    /* Replace with your application code */
		char ss = 0;
		uint8_t p[] = {16};
    while (1) 
    {
		//if (++ss > 99) ss =0;
		ss++;
		//ns_device::scr->screen[1] = ss + '0';
		p[0] = 16;
		SCRN->Digit(p, 3, ss);
		SCRN->PutChar((*p)++, ':');
		SCRN->Hex(p, ss);
		__delay_ms(1000);
		SCRN->Hex(30, ns_device::key->InKey());
//		SCRN->DigitZ(23, 5, SCRN->scrFlashPtr_x);
 		//ns_device::scr->Interrupt();
    }
}

