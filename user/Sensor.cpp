/* 
* Sensor.cpp
*
* Created: 01.03.2023 12:54:23
* Author: Gumerovmn
*/


#include "Sensor.h"

// default constructor
Sensor::Sensor(unsigned char n_sensor, volatile unsigned char* ddr, volatile unsigned char* port, volatile unsigned char* s_pin, unsigned char nBit, SensorCallBack callBack)
{
	this->pin = s_pin; mask = 1 << (nBit & 7);
	*ddr |= mask;	*port |= mask;
	this->n_sensor = n_sensor;
	this->callBack = *callBack;
	count = 0;
	countMax = 20;
	triger = stat = false;
} //Sensor

// default destructor
Sensor::~Sensor()
{
} //~Sensor

void Sensor::interrupt()
{
	if (*pin ^ mask)
	{
		if (count <  countMax)		count++;
		if ((count >= countMax) && !triger)
		{
			count = countMax;
			stat = triger = true;
			if (callBack != nullptr) callBack(n_sensor, stat);
		}
	}
	else
	{
		if (count > 0)		count--;
		if ((count == 0) && triger)
		{
			count = 0;
			stat = triger = false;
			if (callBack != nullptr) callBack(n_sensor, stat);
		}
	}
}
