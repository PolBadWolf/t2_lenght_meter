/* 
* Core.cpp
*
* Created: 06.03.2023 9:51:05
* Author: Gumerovmn
*/


#include "Core.h"
#include "Sensors.h"
#include "../device.h"

// default constructor
Core::Core(NewTubeCallBack newTubeCallBack)
{
	this->newTubeCallBack = newTubeCallBack;
	ns_sensors::startOfDataCollection();
	count99 = ns_device::steckTube->getLenghtTube(ns_device::steckTube->getCountSteckCurrent() - 1).n;
} //Core

// default destructor
// Core::~Core()
// {
// } //~Core

void Core::mainCycle()
{
	int lenghtTube = -1;
	int s = ns_sensors::getReadyData();
	switch (s)
	{
		case 0:
			// нет готовности
			return;
			break;
		case -1:
			// ошибка работы сенсоров
			ns_device::scr->PutChar(15, 'S');
			break;
		case -2:
			// заблокирована работа измерителя
			ns_device::scr->PutChar(15, 'B');
			lenghtTube = -2;
			break;
		default:
			// расчет и сохранение измеренной длины
			lenghtTube = ns_sensors::renderLenght();
			break;
	}
	newData = 0xff;
	if (lenghtTube < 0)
	{
// 		ns_device::scr->DigitM(24, 6, lenghtTube);
		// если ошибка работы сенсоров
		stat = CORESTAT_ERROR;
		// следущй цикл
		ns_sensors::startOfDataCollection();
		return;
	}
	stat = CORESTAT_OK;
	currentLenghtTube = lenghtTube;
	if (++count99 > 99)		count99 = 1;
	if (count99 < 1)		count99 = 1;
	newTubeCallBack(lenghtTube, count99);
	// следущй цикл
	ns_sensors::startOfDataCollection();
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