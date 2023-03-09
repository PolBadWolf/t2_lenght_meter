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
#define STECK_TUBE ns_device::steckTube

#include "DeB.h" // ***********

signed char normX(signed char x, signed char min, signed char max)
{
	while (x < min)		x += max;
	while (x >= max)	x -= max;
	return x;
}

int main(void)
{
	DeB *deb =  new DeB();
	deb->main();
	// ====================
// 	StekTube *steck = new StekTube(3);
// 	StekTube::newTube(5500);
// 	int xn = steck->getCountSteck();
// 	unsigned int len = steck->getLenghtTube(xn);
	// ====================
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
// 	SCR->String_P((uint8_t)25, PSTR("ћир") );
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
	if (ns_sensors::getStatusWork() != 0)
	{
		ns_sensors::startOfDataCollection();
	}
// 	ns_sensors::startOfDataCollection();
	STECK_TUBE->tubes[0].lenght = 11;
	STECK_TUBE->tubes[1].lenght = 22;
	STECK_TUBE->tubes[2].lenght = 33;
	while(true)
	{
		if (++count_ind == 5)
		{
			count_ind = 0;
			
// 			if (ns_sensors::getDebugStat())			SCR->PutChar((uint8_t)0, 'D');
// 			else									SCR->PutChar((uint8_t)0, 'O');
// 			for (uint8_t i = 0; i < 3; i++)
// 			{
// 				if (ns_sensors::getStatSensor(i))	SCR->PutChar(1 + i, '1');
// 				else								SCR->PutChar(1 + i, '0');
// 			}
// 			if (ns_sensors::getReadyData())			SCR->PutChar(4, 'R');
// 			else									SCR->PutChar(4, 'I');


			//
// 			{
// 				uint8_t statusWork = ns_sensors::statusWork;
//  				SCR->Hex(25, statusWork);
// 				char sim;
// 				for (uint8_t i = 0; i < 6; i++)
// 				{
// 					if (statusWork & (1 << i))	sim = '1'; else sim = '0';
// 					SCR->PutChar(16 + i, sim);
// 				}
// 			}
			// ==
			//ns_sensors::mainCicle();
// 			{
// 				switch (ns_sensors::getReadyData())
// 				{
// 					case 0:
// 						// нет готовности
// 						break;
// 					case -1:
// 						SCR->Clear();
// 						SCR->DigitM(16, 4, ns_sensors::time_sensors[0][1]);
// 						SCR->DigitM(21, 4, ns_sensors::time_sensors[1][1]);
// 						SCR->DigitM(26, 4, ns_sensors::time_sensors[2][1]);
// 						SCR->String_P(PSTR("ERROR "));
// 						ns_sensors::startOfDataCollection();
// 						break;
// 					default:
// 						int x = ns_sensors::renderLenght();
// 						SCR->Clear();
// 						SCR->DigitM(6, 6, x);
// 						ns_sensors::startOfDataCollection();
// 						SCR->DigitZ(16, 3, ns_device::steckTube->getCountSteck());
// 						break;
// 				}
// 			}
			{
				ns_device::MainCicle();
				switch (ns_device::core->getStat())
				{
					case CORESTAT_ERROR:
// 						SCR->Clear();
// 						SCR->String_P((uint8_t)0, PSTR("ќшибка"));
						ns_device::core->resetStat();
						break;
					case CORESTAT_OK:
 						SCR->Clear();
						SCR->DigitZ((uint8_t)0, 5, ns_device::core->getCurrentLenghtTube());
//						SCR->DigitZ(8, 1, STECK_TUBE->getCountSteck());
						ns_device::core->resetStat();
// 						SCR->DigitM( 8, 6, STECK_TUBE->getLenghtTube(STECK_TUBE->getCountSteck()-1));
// 						SCR->DigitM(24, 6, STECK_TUBE->getLenghtTube(STECK_TUBE->getCountSteck()-0));
// 						SCR->DigitZ(16, 5, STECK_TUBE->tubes[0].lenght);
// 						SCR->DigitZ(21, 5, STECK_TUBE->tubes[1].lenght);
// 						SCR->DigitZ(26, 5, STECK_TUBE->tubes[2].lenght);
// 						{
// 							StekTubeUnit unit = STECK_TUBE->getLenghtTube(STECK_TUBE->getCountSteck());
//  							SCR->DigitZ(10, 5, unit.lenght);
// 						}
						{
// 							signed char max = STECK_TUBE->getCountSteckMax() - 1;
 							signed char x;
							
// 							x = normX(STECK_TUBE->getCountSteckCurrent() - 0, 0, max);
							x = STECK_TUBE->getCountSteckCurrent();
							SCR->DigitM(16, 3, x);
							StekTubeUnit unit;
							unit = STECK_TUBE->getLenghtTube(x - 2);
							if (unit.n > 0 && unit.n < 100)
							{
 								SCR->DigitM( 7, 2, unit.n);
 								SCR->DigitM(10, 5, unit.lenght);
							}
 							unit = STECK_TUBE->getLenghtTube(x - 1);
							if (unit.n > 0 && unit.n < 100)
							{
  								SCR->DigitM(23, 2, unit.n);
  								SCR->DigitM(26, 5, unit.lenght);
							}
							
// 							// массив номеров
// 							SCR->DigitZ(16, 2, STECK_TUBE->tubes[0].n);
// 							SCR->DigitZ(19, 2, STECK_TUBE->tubes[1].n);
// 							SCR->DigitZ(22, 2, STECK_TUBE->tubes[2].n);
// 							// текущий(на самом деле следующий) номер
// 							x = STECK_TUBE->getCountSteckCurrent();
// 							SCR->DigitZ(25, 2, x);
// 							// чтение из текущего номера
// 							y = STECK_TUBE->getLenghtTube(x - 1).n;
// 							SCR->DigitZ(28, 2, y);
						}
						break;
					default:
// 						SCR->PutChar(16, ((*ns_sensors::sensorMass[0]) ? 1 : 0) );
// 						SCR->PutChar(17, ((*ns_sensors::sensorMass[1]) ? 1 : 0) );
// 						SCR->PutChar(18, ((*ns_sensors::sensorMass[2]) ? 1 : 0) );
						break;
				}
			}
			
		}
	}
#endif
	return 0;
}

