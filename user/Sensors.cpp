/* 
* Sensors.cpp
*
* Created: 28.02.2023 16:35:02
* Author: Gumerovmn
*/


#include "Sensors.h"
#include "Sensor.h"

#include "../device.h"
#define		SCR		ns_device::scr

namespace ns_sensors
{
	int32_t		time_sensors[3][2];								// запись перемещения трубы в тиках
	int32_t		s_count;										// счетчик тиков
	int32_t		v_count;										// счетчик тиков
	int32_t		t_count;										// счетчик тиков для таймаута
	Sensor		*sensor[3];										// объекты сенсоры
	bool		blockSensor;									// сигнал блокировки к сбору данных/готовность к расчету длины
	bool		blockirovka;									// блокировка замера внешним сигналом
	bool		countTimeOut;									//
	// =====
	int32_t		s_count_timeOut;
	int32_t		e_count_timeOut EEMEM	= 5000;
	//
	uint8_t		s_sensorInt[3];									// время интеграции замера на датчиках
	uint8_t		e_sensorInt[3] EEMEM	= {20, 20, 20};
	//
	int16_t		s_sensorPosition[3];									// позитции сенсоров
	int16_t		e_sensorPosition[3] EEMEM	= {0, 1063, 11045};			// позитции сенсоров
	// =========================
	Sensor		*blockIzmer;
	// ********************************
	volatile uint8_t		sensorsStep = SENSORS_STEP_NoZero;
	uint16_t safeTimeZero = 0;
#define safeTimeZeroSize	500
 	bool		sensorInv[4];
 	bool		e_sensorInv[4] EEMEM = {false, true, false, true};
	// ************* safe interval
	uint16_t	safeInterval[3] = {0, 0, 0};
#define safeIntervalSize	500

	void ee_load()
	{
		// время интеграции замера
		for (uint8_t i = 0; i < 3; i++)
		{
			s_sensorInt[i] = eeprom_read_byte((uint8_t *) & e_sensorInt[i]);
		}
		// время zero time_out
		eeprom_read_block(&s_count_timeOut, &e_count_timeOut, sizeof(s_count_timeOut));
		// позитция сенсоров
		for (uint8_t i = 0; i < 3; i++)
		{
			eeprom_read_block(&s_sensorPosition[i], &e_sensorPosition[i], sizeof(s_sensorPosition[0]));
		}
		// инверсия сенсоров
		for (uint8_t i = 0; i < 4; i++)
		{
			sensorInv[i] = (bool) eeprom_read_byte((uint8_t *) &e_sensorInv[i]);
		}
	}
	void ee_save()
	{
		// время интеграции замера
		for (uint8_t i = 0; i < 3; i++)
		{
			eeprom_update_byte((uint8_t *) & e_sensorInt[i], s_sensorInt[i]);
		}
		// время zero time_out
		eeprom_update_block(&s_count_timeOut, &e_count_timeOut, sizeof(s_count_timeOut));
		// позитция сенсоров
		for (uint8_t i = 0; i < 3; i++)
		{
			eeprom_update_block(&s_sensorPosition[i], &e_sensorPosition[i], sizeof(s_sensorPosition[0]));
		}
		// инверсия сенсоров
		for (uint8_t i = 0; i < 4; i++)
		{
			eeprom_update_byte((uint8_t *)&e_sensorInv[i], sensorInv[i]);
		}
	}
	// ==========
	void callBack_s0(bool stat)
	{
		if (safeInterval[0] != 0)	return;
		safeInterval[0] = safeIntervalSize;
		if (stat)
		{	// наезд на датчик
			if ( !sensor[1]->getStat() && !sensor[2]->getStat() && (sensorsStep == SENSORS_STEP_Zero))
			{	// начало измерения
				s_count = 0;
				for (uint8_t i = 0; i < 3; i++)
				{	// reset counts
					time_sensors[i][0] = 0;
					time_sensors[i][1] = 0;
				}
				countTimeOut = false;
				blockirovka = false;
				sensorsStep = SENSORS_STEP_Start;
			}
		}
		else
		{	// съезд с датчика
			time_sensors[0][0] = s_count;
		}
	}
	void callBack_s1(bool stat)
	{
		if (safeInterval[1] != 0)	return;
		safeInterval[1] = safeIntervalSize;
		if (stat)
		{	// наезд на датчик
			time_sensors[1][1] = s_count;
		}
		else
		{	// съезд с датчика
			time_sensors[1][0] = s_count;
		}
	}
	void callBack_s2(bool stat)
	{
		if (safeInterval[2] != 0)	return;
		safeInterval[2] = safeIntervalSize;
		if (stat)
		{	// наезд на датчик
			time_sensors[2][1] = s_count;
		}
		else
		{	// съезд с датчика
			time_sensors[2][0] = s_count;
		}
	}
	void callBack(uint8_t nSensor, bool stat)
	{
		//измерения заблокированы
		if (blockSensor)	return;
		switch (nSensor)
		{
			case 0:
				callBack_s0(stat);
				break;
			case 1:
				callBack_s1(stat);
				break;
			case 2:
				callBack_s2(stat);
				break;
			default:
				break;
		}
		// проверка на окончание сбора данных
		if (!sensor[0]->getStat() && !sensor[1]->getStat() && sensor[2]->getStat())
		{
			blockSensor = true;
		}
	}
	void callBackBlock(uint8_t nSensor, bool stat)	{ }
	// ==========
	void initIntegr()
	{
		for (uint8_t i = 0; i < 3; i++)
		{
			sensor[i]->setCountMax(s_sensorInt[i]);
		}
		blockIzmer->setCountMax(40);
	}
	// ==========
	void Init()
	{
		ee_load();
		sensor[0] = new Sensor(0, &DDRC, &PORTC, &PINC, 0, callBack, sensorInv[0]);
		sensor[1] = new Sensor(1, &DDRC, &PORTC, &PINC, 1, callBack, sensorInv[1]);
		sensor[2] = new Sensor(2, &DDRC, &PORTC, &PINC, 3, callBack, sensorInv[2]);
		blockIzmer = new Sensor(0, &DDRC, &PORTC, &PINC, 4, callBackBlock, sensorInv[3]);
		initIntegr();
		s_count = 0;
		t_count = 0;
		blockirovka = false;
		countTimeOut = false;
		// инициализация массива сенсоров
		blockSensor = false;
	}
	// ===============
	uint16_t	delayZero = 0;
	void interrupt()
	{
		if (!sensor[0]->getStat() && !sensor[1]->getStat() && !sensor[2]->getStat())
		{
			if ( ((sensorsStep == SENSORS_STEP_NoZero) /*|| (sensorsStep == SENSORS_STEP_BigTimeOut) || (sensorsStep == SENSORS_STEP_SmlTimeout)*/) && (++delayZero == 1000 ) )
			{
				sensorsStep = SENSORS_STEP_Zero;
				blockSensor = false;
				blockirovka = false;
				t_count = 0;
				delayZero = 0;
			}
			if (sensorsStep == SENSORS_STEP_Start)
			{
				if (t_count < s_count_timeOut)	t_count++;
				else	sensorsStep = SENSORS_STEP_SmlTimeout;
				//***
				if (blockirovka)	sensorsStep = SENSORS_STEP_BlockIzm;
			}
		}
		if ( sensorsStep == SENSORS_STEP_Start || sensorsStep == SENSORS_STEP_BlockIzm )	s_count++;
		if (s_count != 0 && s_count != s_count_timeOut) v_count = s_count;
		// Big Time Out
		if ( (s_count >= 20000) && (sensorsStep != SENSORS_STEP_NoZero) )
		{
			sensorsStep = SENSORS_STEP_BigTimeOut;
			s_count = 0;
			countTimeOut = true;	// *************
		}
		// ===========
		for (uint8_t i = 0; i < 3; i++)
		{
			if (safeInterval[i] > 0)	safeInterval[i]--;
		}
		// ===========
		sensor[0]->interrupt();
		sensor[1]->interrupt();
		sensor[2]->interrupt();
		//
		blockIzmer->interrupt();
		blockirovka |= blockIzmer->getStat();
		if ( (!sensor[0]->getStat() && !sensor[1]->getStat() && sensor[2]->getStat()) && (sensorsStep == SENSORS_STEP_Start) )
		{
			sensorsStep = SENSORS_STEP_Ready;
		}
		// ************
		if ( (sensorsStep == SENSORS_STEP_Zero)  && !sensor[0]->getStat() && !sensor[1]->getStat() && !sensor[2]->getStat()  )
		{
			if (safeTimeZero < safeTimeZeroSize)	safeTimeZero++;
			if (safeTimeZero == safeTimeZeroSize)
			{
				safeTimeZero = safeTimeZeroSize + 1;
				blockSensor = false;
			}
		}
		else
		{
			safeTimeZero = 0;
		}
	}
	// ========================
	void startOfDataCollection()
	{
		sensorsStep = SENSORS_STEP_NoZero;
	}
	// ====================
	int renderLenght()
	{
		if (!blockSensor)	return -1;
		if (time_sensors[0][0] == 0)  return -2;
		if (time_sensors[1][0] == 0)  return -3;
		if (time_sensors[1][1] == 0)  return -4;
		if (time_sensors[2][1] == 0)  return -5;
		if (time_sensors[0][0] < time_sensors[1][1])	return -6;
		// определение базы
		int16_t baseMainSmall = s_sensorPosition[2] - s_sensorPosition[1];
		int16_t baseMainBig = s_sensorPosition[2] - s_sensorPosition[0];
		//
		int16_t baseRender = s_sensorPosition[1] - s_sensorPosition[0];
	
		// до счет в минус на малой базе
		int32_t timeRenderMinusBaseSmall	= time_sensors[2][1] - time_sensors[1][0];
		// до счет в плюс на большой базе
		int32_t timeRenderPlusBaseBig		= time_sensors[0][0] - time_sensors[2][1];
		// до счет в плюс на малой базе
		int32_t timeRenderPlusBaseSmall		= time_sensors[1][0] - time_sensors[2][1];
		// до счет в минус на большой базе
		int32_t timeRenderMinusBaseBig		= time_sensors[2][1] - time_sensors[0][0];
		//
		int32_t timeRender					= time_sensors[1][0] - time_sensors[0][0];

		// до счет в минус на малой базе
		if ( (time_sensors[2][1] >= time_sensors[1][0]) && (time_sensors[1][0] >= time_sensors[0][0]))
		{
			return baseMainSmall - (baseRender * timeRenderMinusBaseSmall / timeRender);
		}
		// до счет в плюс на большой базе
		if ( time_sensors[0][0] >= time_sensors[2][1] )
		{
			return baseMainBig + (baseRender * timeRenderPlusBaseBig / timeRender);
		}
		// до счет в плюс на малой базе
		if ( timeRenderMinusBaseBig >= timeRenderPlusBaseSmall )
		{
			return baseMainSmall + (baseRender * timeRenderPlusBaseSmall / timeRender);
		}
		// до счет в минус на большой базе
		if ( timeRenderPlusBaseSmall >= timeRenderMinusBaseBig )
		{
			return baseMainBig - (baseRender * timeRenderMinusBaseBig / timeRender);
		}
		return -1;
	}
	void mainCicle()
	{
// 		SCR->DigitZ(6, 1, sensorsStep);
// 		SCR->Hex(8, statusWork);
	}
// 	unsigned char getStatusWork()
// 	{
// 		return statusWork;
// 	}
	//==============
	bool getSensorStat(uint8_t n)
	{
		return sensor[n]->getStat();
	}
	void setZeroSensorInv()
	{
		bool stat;
		for (uint8_t i = 0; i < 3; i++)
		{
			stat = sensor[i]->triger;
			sensorInv[i] = stat;
			sensor[i]->statInv = stat;
		}
		stat = blockIzmer->triger;
		sensorInv[3] = stat;
		blockIzmer->statInv = stat;
		ee_save();
	}
}
