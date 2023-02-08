/*
* SwitchMode.cpp
*
* Created: 21.05.2020 15:17:22
* Author: User
*/


#include "SwitchMode.h"

// default constructor
void SwitchMode::Init(
	volatile uint8_t* sw1_ddr, volatile uint8_t *sw1_port, volatile uint8_t *sw1_pin, uint8_t sw1_bit,
	volatile uint8_t *sw2_ddr, volatile uint8_t *sw2_port, volatile uint8_t *sw2_pin, uint8_t sw2_bit,
	uint16_t sw1_levelIntegr,
	uint16_t sw2_levelIntegr,
	TypeFunct1 *LinkFunction
)
{
	//	LinkFunction
	this->LinkFunction = LinkFunction;
	// save ports
	sw_ddr[0] = sw1_ddr;
	sw_ddr[1] = sw2_ddr;
	sw_port[0] = sw1_port;
	sw_port[1] = sw2_port;
	sw_pin[0] = sw1_pin;
	sw_pin[1] = sw2_pin;
	sw_mask[0] = 1 << sw1_bit;
	sw_mask[1] = 1 << sw2_bit;
	// init
	for (uint8_t i = 0; i < 2; i++)
	{
		*sw_ddr[i]  |= sw_mask[i];
		*sw_port[i] |= sw_mask[i];
	}
	// delay
	__delay_us(20);
	// init vars
	sw_level[0] = sw1_levelIntegr;
	sw_level[1] = sw2_levelIntegr;
	//
	sw_stat[0] = *sw_pin[0] & sw_mask[0];
	sw_stat[1] = *sw_pin[1] & sw_mask[1];
	//
	if (sw_stat[0] == 0)	sw_count[0] = 0;
	else					sw_count[0] = sw_level[0];
	if (sw_stat[1] == 0)	sw_count[1] = 0;
	else					sw_count[1] = sw_level[1];
}
SwitchMode::SwitchMode(
volatile uint8_t* sw1_ddr, volatile uint8_t *sw1_port, volatile uint8_t *sw1_pin, uint8_t sw1_bit,
volatile uint8_t *sw2_ddr, volatile uint8_t *sw2_port, volatile uint8_t *sw2_pin, uint8_t sw2_bit,
uint16_t sw1_levelIntegr, uint16_t sw2_levelIntegr
)
{
	Init(sw1_ddr, sw1_port, sw1_pin, sw1_bit, sw2_ddr, sw2_port, sw2_pin, sw2_bit, sw1_levelIntegr, sw2_levelIntegr, NULL);
}
SwitchMode::SwitchMode(
volatile uint8_t* sw1_ddr, volatile uint8_t *sw1_port, volatile uint8_t *sw1_pin, uint8_t sw1_bit,
volatile uint8_t *sw2_ddr, volatile uint8_t *sw2_port, volatile uint8_t *sw2_pin, uint8_t sw2_bit,
uint16_t sw1_levelIntegr, uint16_t sw2_levelIntegr, TypeFunct1 *CallBack
)
{
	Init(sw1_ddr, sw1_port, sw1_pin, sw1_bit, sw2_ddr, sw2_port, sw2_pin, sw2_bit, sw1_levelIntegr, sw2_levelIntegr, CallBack);
}

// default destructor
SwitchMode::~SwitchMode()
{
} //~SwitchMode

// заглушка
void SwitchMode::MainCicle()
{
	// CallBack();
}

// прерывание, для устранения дребезга
void SwitchMode::Interrupt()
{
	flagEvent = 0;
	for (uint8_t i=0; i<2; i++)
	{
		if ( (*sw_pin[i] & sw_mask[i]) == 0 )
		{
			if (sw_count[i] > 0)	sw_count[i]--;
			if ( (sw_count[i] == 0) && (sw_stat[i] != 0) )
			{
				sw_stat[i] = 0;
				flagEvent = 1;
			}
		}
		else
		{
			if (sw_count[i] < sw_level[i])	sw_count[i]++;
			if ( (sw_count[i] == sw_level[i]) && (sw_stat[i] == 0) )
			{
				sw_stat[i] = 1;
				flagEvent = 1;
			}
		}
	}
	if (flagEvent) CallBack();
}

void SwitchMode::CallBack()
{
	if (this->LinkFunction != NULL)
	{
		//((TypeFunct1)pgm_read_ptr(& LinkFunction[0]))();
		CallFunct1(LinkFunction, 0);
	}
}

uint8_t SwitchMode::Status() // статус: 0 - стоп, 1 - пуск, 2 - цикл
{
	uint8_t stat = SWITCH_POS_STOP;
	CRITICAL_SECTION
	{
		if ( !sw_stat[0] && sw_stat[1] ) stat = SWITCH_POS_PUSK;
		if ( sw_stat[0] && !sw_stat[1] ) stat = SWITCH_POS_CICL;
	}
	return stat;
}
