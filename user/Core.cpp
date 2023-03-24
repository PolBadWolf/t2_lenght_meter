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
	flagBlockIzm = false;
} //Core

// default destructor
// Core::~Core()
// {
// } //~Core

void Core::mainCycle()
{
	int lenghtTube = 0;
	uint8_t ss = ns_sensors::sensorsStep;
	if (ss == SENSORS_STEP_Zero)
	{
		if (flagBlockIzm)
		{
			sendRS.SendChangeBlock(0);
			stat = CORESTAT_BLOCK;
			flagBlockIzm = false;
		}
	}
	if (ss == SENSORS_STEP_BlockIzm)
	{
		flagBlockIzm = true;
		ns_sensors::startOfDataCollection();
	}
	if ( (ss == SENSORS_STEP_BigTimeOut) || (ss == SENSORS_STEP_SmlTimeout) )
	{
		sendRS.SendErrorSensor();
		stat = CORESTAT_ERRSN;
		ns_sensors::startOfDataCollection();
	}
	if (ss == SENSORS_STEP_Ready)
	{
		lenghtTube = ns_sensors::renderLenght();
		if (ns_sensors::blockirovka)
		{
			sendRS.SendChangeBlock(lenghtTube);
			stat = CORESTAT_BLOCK;
			ns_sensors::startOfDataCollection();
		}
		else
		{
			if (lenghtTube < 6000 || lenghtTube > 13000)
			{
				sendRS.SendErrorRender(0);
				stat = CORESTAT_ERRIZ;
				ns_sensors::startOfDataCollection();
			}
			else
			{
				stat = CORESTAT_OK;
				currentLenghtTube = lenghtTube;
				if (++count99 > 99)		count99 = 1;
				if (count99 < 1)		count99 = 1;
				newTubeCallBack(currentLenghtTube, count99);
				sendRS.SendLenght(count99, currentLenghtTube);
				// save last timing
				lastTiming[0] = ns_sensors::time_sensors[0][0];
				lastTiming[1] = ns_sensors::time_sensors[1][0];
				lastTiming[2] = ns_sensors::time_sensors[2][1];
				nAlgoritm = ns_sensors::nAlgoritm;
				// ======
				ns_sensors::startOfDataCollection();
				newData = 0xff;
			}
		}
	}
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