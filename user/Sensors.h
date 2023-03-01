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
	extern uint32_t	time_sensors[][2];
	extern uint8_t	s_sensorInt[];
	//
	void ee_load();
	void ee_save();
	void Init();
	
} //Sensors

#endif //__SENSORS_H__
