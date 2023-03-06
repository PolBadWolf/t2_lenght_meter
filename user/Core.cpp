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
		default:
			// расчет и сохранение измеренной длины
			lenghtTube = ns_sensors::renderLenght();
			break;
	}
	if (lenghtTube < 0)
	{
		ns_device::scr->DigitM(24, 6, lenghtTube);
		// если ошибка работы сенсоров
		stat = CORESTAT_ERROR;
		// следущй цикл
		ns_sensors::startOfDataCollection();
		return;
	}
	stat = CORESTAT_OK;
	currentLenghtTube = lenghtTube;
	newTubeCallBack(lenghtTube);
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