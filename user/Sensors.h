/* 
* Sensors.h
*
* Created: 28.02.2023 16:35:02
* Author: Gumerovmn
*/


#ifndef __SENSORS_H__
#define __SENSORS_H__
#include "path/path.h"

namespace ns_sensors
{
	extern int32_t		time_sensors[][2];
	extern uint8_t		s_sensorInt[3];
	extern uint8_t		statusWork;
	//
	void ee_load();
	void ee_save();
	void Init();
	void interrupt();
	//
	void initIntegr();
	void startOfDataCollection();
	bool getStatSensor(uint8_t nSensor);
	bool getReadyData();
	signed int renderLenght();
	// *******
	bool getDebugStat();
	void setDebugStat(bool stat);
	void setDebugStatSensors(bool *statSensors);
	void setDebugStatSensor(uint8_t nSensor, bool statSensor);
	
} //Sensors

#endif //__SENSORS_H__
