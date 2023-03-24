/* 
* Sensors.h
*
* Created: 28.02.2023 16:35:02
* Author: Gumerovmn
*/


#ifndef __SENSORS_H__
#define __SENSORS_H__
#include "path/path.h"

#define		SENSORS_STEP_NoZero			1
#define		SENSORS_STEP_Zero			2
#define		SENSORS_STEP_Start			3
#define		SENSORS_STEP_BigTimeOut		4
#define		SENSORS_STEP_SmlTimeout		5
#define		SENSORS_STEP_BlockIzm		6
#define		SENSORS_STEP_Ready			7

namespace ns_sensors
{
	extern int32_t		time_sensors[][2];
	extern uint8_t		s_sensorInt[3];
// 	extern uint8_t		statusWork;
// 	extern bool			*sensorMass[3];
	extern int32_t		s_count_timeOut;
	extern int32_t		v_count;									// счетчик тиков
	extern bool			blockirovka;								// блокировка замера внешним сигналом
	
	extern int16_t		s_sensorPosition[3];						// позитции сенсоров
	
	extern	volatile uint8_t		sensorsStep;
	
	extern	uint8_t nAlgoritm;
#define N_Algoritm_NONE			0
#define N_Algoritm_BIG_PLS		1
#define N_Algoritm_BIG_MNS		2
#define N_Algoritm_SML_PLS		3
#define N_Algoritm_SML_MNS		4
	
	//
	void ee_load();
	void ee_save();
	void Init();
	void interrupt();
	//
	void initIntegr();
	void startOfDataCollection();
	bool getStatSensor(uint8_t nSensor);
	int	 renderLenght();
	// ===============
	bool getSensorStat(uint8_t n);
	void setZeroSensorInv();
	// *******
	void mainCicle();
} //Sensors

#endif //__SENSORS_H__
