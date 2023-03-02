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

#include "user/Sensors.h"

int main(void)
{
	ns_sensors::Init();
	ns_device::Init();
	sei();
#ifdef CONF_MENU
	while (true)
	{
		ns_device::MainCicle();
	}
#endif
#ifndef CONF_MENU
// 	int16_t x_count = 0;
// 	SCR->Clear();
// 	SCR->String_P((uint8_t)0, PSTR("Hello World !!!") );
// 	SCR->String_P((uint8_t)25, PSTR("Мир") );
// 	
// 	/* Replace with your application code */
// 	char ss = 0;
// 	uint8_t p[] = {16};
// 	while (1)
// 	{
// 		__delay_ms(1);
// 		if (++x_count == 800)
// 		{
// 			x_count = 0;
// 			ss++;
// 		}
// 		p[0] = 16;
// 		SCR->Digit(p, 3, ss);
// 		SCR->PutChar((*p)++, ':');
// 		SCR->Hex(p, ss);
// #ifdef CONF_KEY4
// 		SCR->Hex(14, KEY->InKey());
// #endif // CONF_KEY4
// 		//		SCRN->DigitZ(23, 5, SCRN->scrFlashPtr_x);
// 		//ns_device::scr->Interrupt();
// 	}
// ==================--------------=================
	SCR->Clear();
	SCR->flicker = false;
	ns_sensors::setDebugStat(false);
	uint8_t count_ind = 0;
	__delay_ms(1000);
	ns_sensors::startOfDataCollection();
	while(true)
	{
		if (++count_ind == 5)
		{
			count_ind = 0;
			if (ns_sensors::getDebugStat())			SCR->PutChar((uint8_t)0, 'D');
			else									SCR->PutChar((uint8_t)0, 'O');
			for (uint8_t i = 0; i < 3; i++)
			{
				if (ns_sensors::getStatSensor(i))	SCR->PutChar(1 + i, '1');
				else								SCR->PutChar(1 + i, '0');
			}
			if (ns_sensors::getReadyData())			SCR->PutChar(4, 'R');
			else									SCR->PutChar(4, 'I');
			//
			{
				uint8_t statusWork = ns_sensors::statusWork;
				char sim;
				for (uint8_t i = 0; i < 8; i++)
				{
					if (statusWork & (1 << i))	sim = '1'; else sim = '0';
					SCR->PutChar(16 + i, sim);
				}
			}
		}
	}
#endif
	return 0;
}

