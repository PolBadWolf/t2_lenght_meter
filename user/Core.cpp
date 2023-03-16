/* 
* Core.cpp
*
* Created: 06.03.2023 9:51:05
* Author: Gumerovmn
*/


#include "Core.h"
#include "Sensors.h"
#include "../device.h"

#define  scr_	ns_device::scr

// default constructor
Core::Core(NewTubeCallBack newTubeCallBack)
{
	this->newTubeCallBack = newTubeCallBack;
	stat = CORESTAT_NONE;
	ns_sensors::startOfDataCollection();
	count99 = ns_device::steckTube->getLenghtTube(ns_device::steckTube->getCountSteckCurrent() - 1).n;
	trigMessError = false;
} //Core

// default destructor
// Core::~Core()
// {
// } //~Core

void Core::mainCycle()
{
	int lenghtTube = 0;
	int s = ns_sensors::getReadyData();
	switch (s)
	{
		case SENSORS_READY_NotReady:
			// нет готовности
			trigMessError = false;
			return;
			break;
		case SENSORS_READY_ErrorSn:
			// ошибка работы сенсоров
			{
				if (!trigMessError)		sendRS.SendErrorSensor();
				trigMessError = true;
				stat = CORESTAT_ERRSN;
				lenghtTube = -1;
			}
			break;
		case SENSORS_READY_TimeOutCn:
			{
				if (!trigMessError)		sendRS.SendErrorSensor();
				trigMessError = true;
				stat = CORESTAT_ERRSN;
				lenghtTube = -1;
			}
			break;
		default:
			// расчет и сохранение измеренной длины
			if (lenghtTube == 0)
			{
				lenghtTube = ns_sensors::renderLenght();
				if (lenghtTube < 6000 || lenghtTube > 12500)
				{
					sendRS.SendErrorRender(lenghtTube);
					stat = CORESTAT_ERRIZ;
				}
				else
				{
					stat = CORESTAT_OK;
					trigMessError = false;
				}
			}
			break;
	}
	// следущй цикл
	ns_sensors::startOfDataCollection();
	currentLenghtTube = lenghtTube;
	newData |= 1 << 0;
	// измерение прошло
	if (stat != CORESTAT_OK)		return;
	if (ns_sensors::blockirovka)
	{
		if (!trigMessError)		sendRS.SendChangeBlock(currentLenghtTube);
		trigMessError = true;
		stat = CORESTAT_BLOCK;
	}
	else
	{
		if (++count99 > 99)		count99 = 1;
		if (count99 < 1)		count99 = 1;
		newTubeCallBack(currentLenghtTube, count99);
		sendRS.SendLenght(count99, currentLenghtTube);
	}
	newData = 0xff;
}

signed char	Core::getStat()
{
	return stat;
}

void Core::resetStat()
{
	stat = CORESTAT_NONE;
}

unsigned int Core::getCurrentLenghtTube()
{
	return currentLenghtTube;
}

unsigned char Core::getCurrentCount()
{
	return count99;
}