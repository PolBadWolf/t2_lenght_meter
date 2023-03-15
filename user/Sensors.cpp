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
	uint8_t		statusWork = 0;									// блокировка от повторных срабатываний событий на сенсорах
	bool		blockSensor;									// сигнал блокировки к сбору данных/готовность к расчету длины
	bool		debugOn = false;
	bool		debugSensor[3];
	bool		*sensorMass[3];
	bool		blockirovka;									// блокировка замера внешним сигналом
	bool		countTimeOut;									//
	// =====
	int32_t		s_count_timeOut			= 5000;
	int32_t		e_count_timeOut EEMEM	= 5000;
	//
	uint8_t		s_sensorInt[3];									// время интеграции замера на датчиках
	uint8_t		e_sensorInt[3] EEMEM	= {20, 20, 20};
	//
	int16_t		s_sensorPosition[3]			= {0, 1000, 11045};			// позитции сенсоров
	int16_t		e_sensorPosition[3] EEMEM	= {0, 1000, 11045};			// позитции сенсоров
	// =========================
	Sensor		*blockIzmer;

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
	}
	// ==========
	void callBack_s0(bool stat)
	{
		bool sensorStat;
		sensorStat = *sensorMass[0];
		//
		// начало измерения
		if ( sensorStat && ((statusWork & (1 << 0)) == 0))
		{
			if (!(*sensorMass[1]) && !(*sensorMass[2]))
			{
				statusWork |= 1 << 0;
				s_count = 0;
				time_sensors[0][1] = s_count;
				countTimeOut = false;
			}
		}
		// откл
		if (!sensorStat && ((statusWork & (1 << 1)) == 0))		{ time_sensors[0][0] = s_count; statusWork |= 1 << 1; }
	}
	void callBack_s1(bool stat)
	{
		bool sensorStat = *sensorMass[1];
		if ( sensorStat && ((statusWork & (1 << 2)) == 0))	{ time_sensors[1][1] = s_count; statusWork |= 1 << 2; }
		if (!sensorStat && ((statusWork & (1 << 3)) == 0))	{ time_sensors[1][0] = s_count; statusWork |= 1 << 3; }
	}
	void callBack_s2(bool stat)
	{
		bool sensorStat = *sensorMass[2];
		if ( sensorStat && ((statusWork & (1 << 4)) == 0))	{ time_sensors[2][1] = s_count; statusWork |= 1 << 4; }
		if (!sensorStat && ((statusWork & (1 << 5)) == 0))	{ time_sensors[2][0] = s_count; statusWork |= 1 << 5; }
		
	}
	void callBack(uint8_t nSensor, bool stat)
	{
		if (debugOn)		sensorMass[nSensor] = &debugSensor[nSensor];
		else				sensorMass[nSensor] = &sensor[nSensor]->stat;
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
		if (!*sensorMass[0] && !*sensorMass[1] && *sensorMass[2])
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
	}
	// ==========
	void Init()
	{
		ee_load();
		sensor[0] = new Sensor(0, &DDRC, &PORTC, &PINC, 0, callBack);
		sensor[1] = new Sensor(1, &DDRC, &PORTC, &PINC, 1, callBack);
		sensor[2] = new Sensor(2, &DDRC, &PORTC, &PINC, 3, callBack);
		blockIzmer = new Sensor(0, &DDRC, &PORTC, &PINC, 4, callBackBlock);
		initIntegr();
		s_count = 0;
		for (uint8_t i = 0; i < 3; i++)		sensorMass[i] = &sensor[i]->stat;
		statusWork = 0;
		t_count = 0;
		blockirovka = false;
		countTimeOut = false;
		// инициализация массива сенсоров
		blockSensor = true;
		for (uint8_t i = 0; i < 3; i++)
		{
			for (uint8_t j = 0; j < (sensor[i]->getCountMax() + 5); j++)
			{
				__delay_ms(1);
				sensor[i]->interrupt();
			}
			sensorMass[i] = &sensor[i]->stat;
		}
		for (uint8_t j = 0; j < (blockIzmer->getCountMax() + 5); j++)
		{
			__delay_ms(1);
			blockIzmer->interrupt();
		}
		blockSensor = false;
	}
	// ===============
	void interrupt()
	{
		if (statusWork & (1 << 0) && !blockSensor)	s_count++;
		else blockirovka = false;
		if (s_count != 0 && s_count != s_count_timeOut) v_count = s_count;
		sensor[0]->interrupt();
		sensor[1]->interrupt();
		sensor[2]->interrupt();
		//
		blockIzmer->interrupt();
		blockirovka |= blockIzmer->stat;
		// Time Out
		if (s_count > 40000)
		{
				statusWork = 0;
				s_count = s_count_timeOut;
				countTimeOut = true;
		}
		// Zero TimeOut
		if (!*sensorMass[0] && !*sensorMass[1] && !*sensorMass[2])
		{
			if ((statusWork & (1 << 5)) != 0 )
			{
				statusWork = 0;
				s_count = 0;
			}
			else
			{
				if (t_count < s_count_timeOut)
				{
					t_count++;
				}
				else
				{
					statusWork = 0;
					s_count = 0;
				}
			}
		}
		else
		{
			t_count = 0;
		}
	}
	// ========================
	void startOfDataCollection()
	{
		blockSensor = false;
		if (!sensor[0]->stat && !sensor[0]->stat && !sensor[0]->stat)
		{
			statusWork = 0;
			s_count = s_count_timeOut;
			for (uint8_t i = 0; i < 3; i++)
			{
				for (uint8_t j = 0; j < 2; j++)
				{
					time_sensors[i][j] = 0;
				}
			}
		}
	}
	// =====================
	bool getStatSensor(uint8_t nSensor)
	{
		return *sensorMass[nSensor];
	}
	// ====================
	int getReadyData()
	{
		if (!blockSensor)	return		SENSORS_READY_NotReady;
		if (blockirovka)	return		SENSORS_READY_BlockIzm;
		if (countTimeOut)	return		SENSORS_READY_TimeOutCn;
		if (	(time_sensors[2][1] > time_sensors[1][1])
			&&	(time_sensors[1][1] > time_sensors[0][1])
			)	return SENSORS_READY_Ready;
		return SENSORS_READY_ErrorSn;
	}
	int renderLenght()
	{
		if (!blockSensor)	return -1;
		if (time_sensors[0][0] == 0)  return -2;
		if (time_sensors[1][0] == 0)  return -3;
		if (time_sensors[1][1] == 0)  return -4;
		if (time_sensors[2][1] == 0)  return -5;
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

//   		SCR->DigitM(16, 4, time_sensors[0][0]);
//  		SCR->DigitM(21, 4, time_sensors[1][0]);
//  		SCR->DigitM(26, 5, time_sensors[2][1]);
// 
//  		SCR->DigitM(0, 5, timeRender);

		
		// до счет в минус на малой базе
		if ( (time_sensors[2][1] >= time_sensors[1][0]) && (time_sensors[1][0] >= time_sensors[0][0]))
		{
// 			SCR->PutChar(15, '1');
			return baseMainSmall - (baseRender * timeRenderMinusBaseSmall / timeRender);
		}
		// до счет в плюс на большой базе
		if ( time_sensors[0][0] >= time_sensors[2][1] )
		{
// 			SCR->PutChar(15, '4');
			return baseMainBig + (baseRender * timeRenderPlusBaseBig / timeRender);
		}
		// до счет в плюс на малой базе
		if ( timeRenderMinusBaseBig >= timeRenderPlusBaseSmall )
		{
// 			SCR->PutChar(15, '2');
			return baseMainSmall + (baseRender * timeRenderPlusBaseSmall / timeRender);
		}
		// до счет в минус на большой базе
		if ( timeRenderPlusBaseSmall >= timeRenderMinusBaseBig )
		{
// 			SCR->PutChar(15, '3');
			return baseMainBig - (baseRender * timeRenderMinusBaseBig / timeRender);
		}
		return -1;
	}
	bool getDebugStat()
	{
		return	debugOn;
	}
	void setDebugStat(bool stat)
	{
		debugOn = stat;
	}
	void setDebugStatSensors(bool *statSensors)
	{
		for (uint8_t i = 0; i < 3; i++)
		{
			debugSensor[i] = statSensors[i];
		}
	}
	void setDebugStatSensor(uint8_t nSensor, bool statSensor)
	{
		debugSensor[nSensor] = statSensor;
	}
	void mainCicle()
	{
//   		SCR->DigitZ(19, 6, s_count);
	}
	unsigned char getStatusWork()
	{
		return statusWork;
	}
}
