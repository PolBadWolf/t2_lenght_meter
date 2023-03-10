/* 
* Sensor.cpp
*
* Created: 01.03.2023 12:54:23
* Author: Gumerovmn
*/


#include "Sensor.h"
// 
// #include "../device.h"
// #define  SCR	ns_device::scr

// default constructor
Sensor::Sensor(unsigned char n_sensor, volatile unsigned char* ddr, volatile unsigned char* port, volatile unsigned char* s_pin, unsigned char nBit, SensorCallBack callBack)
{
	this->pin = s_pin; mask = 1 << (nBit & 7);
	*ddr &= ~mask;	*port |= mask;
	this->n_sensor = n_sensor;
	this->callBack = *callBack;
	count = 0;
	countMax = 20;
	if (*pin & mask)
	{
		triger = true;
		stat = false;
	}
	else
	{
		triger = false;
		stat = true;
	}
} //Sensor

// default destructor
Sensor::~Sensor()
{
} //~Sensor

void Sensor::interrupt()
{
	if (*pin & mask)
	{
		if (count <  countMax)		count++;
		if ((count >= countMax) && !triger)
		{
			count	= countMax;
			triger	= true;
			stat	= false;
			if (callBack != nullptr) callBack(n_sensor, stat);
		}
	}
	else
	{
		if (count > 0)		count--;
		if ((count == 0) && triger)
		{
			count	= 0;
			triger	= false;
			stat	= true;
			if (callBack != nullptr) callBack(n_sensor, stat);
		}
	}
}
