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
			// ��� ����������
			trigMessError = false;
			return;
			break;
		case SENSORS_READY_ErrorSn:
			// ������ ������ ��������
			{
				if (!trigMessError)		sendRS.SendErrorSensor();
				trigMessError = true;
				stat = CORESTAT_ERRSN;
				lenghtTube = -1;
			}
			break;
		case SENSORS_READY_BlockIzm:
			// ������������� ������ ����������
			{
				if (!trigMessError)		sendRS.SendChangeBlock();
				trigMessError = true;
				stat = CORESTAT_BLOCK;
				lenghtTube = -2;
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
			// ������ � ���������� ���������� �����
			if (lenghtTube == 0)
			{
				lenghtTube = ns_sensors::renderLenght();
				if (lenghtTube > 6000)
				{
					stat = CORESTAT_OK;
					trigMessError = false;
				}
				else
				{
					sendRS.SendErrorRender();
					stat = CORESTAT_ERRIZ;
				}
			}
			break;
	}
	// ������� ����
	ns_sensors::startOfDataCollection();
	currentLenghtTube = lenghtTube;
	// ��������� ������
	if (stat != CORESTAT_OK)		return;
	newData = 0xff;
	if (++count99 > 99)		count99 = 1;
	if (count99 < 1)		count99 = 1;
	newTubeCallBack(currentLenghtTube, count99);
	sendRS.SendLenght(count99, currentLenghtTube);
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