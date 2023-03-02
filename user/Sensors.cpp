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
	uint8_t		s_sensorInt[3];									// время интеграции замера на датчиках
	uint8_t		e_sensorInt[3] EEMEM = {20, 20, 20};
	int32_t		s_count;										// счетчик тиков
	Sensor		*sensor[3];										// объекты сенсоры
	int16_t		sensorPosition[3] = {0, 1000, 11045};			// позитции сенсоров
	uint8_t		statusWork = 0;									// блокировка от повторных срабатываний событий на сенсорах
// 	bool		blockEventSensor[3][2] = { {true, true}, {true, true}, {true, true} };
	bool		blockSensor;									// сигнал блокировки к сбору данных/готовность к расчету длины
	bool		debugOn = false;
	bool		debugSensor[3];
	void ee_load()
	{
		for (uint8_t i = 0; i < 3; i++)
		{
			s_sensorInt[i] = eeprom_read_byte((uint8_t *) & e_sensorInt[i]);
		}
	}
	void ee_save()
	{
		for (uint8_t i = 0; i < 3; i++)
		{
			eeprom_update_byte((uint8_t *) & e_sensorInt[i], s_sensorInt[i]);
		}
	}
	// ==========
	void callBack_s0(bool stat)
	{
		bool sensorStat;
		if (debugOn)	sensorStat = debugSensor[0];
		else			sensorStat = sensor[0]->stat;
		//
		if ( sensorStat && ((statusWork & (1 << 0)) == 0))
		{
			// начало измерения
			if (debugOn)
			{
				if (!debugSensor[1] && !debugSensor[2])
				{
					statusWork |= 1 << 0;
					s_count = 0;
					time_sensors[0][1] = s_count;
				}
			}
			else
			{
				SCR->Hex(10, sensor[1]->stat);
				if (!sensor[1]->stat && !sensor[2]->stat)
				{
					statusWork |= 1 << 1;
					s_count = 0;
					time_sensors[0][1] = s_count;
				}
			}
		}
		if (!sensorStat && ((statusWork & (1 << 0)) == 0))	{ time_sensors[0][0] = s_count; statusWork |= 1 << 0; }
	}
	void callBack_s1(bool stat)
	{
		bool sensorStat;
		if (debugOn)	sensorStat = debugSensor[1];
		else			sensorStat = sensor[1]->stat;
		//
		if ( sensorStat && ((statusWork & (1 << 3)) == 0))	{ time_sensors[1][1] = s_count; statusWork |= 1 << 3; }
		if (!sensorStat && ((statusWork & (1 << 2)) == 0))	{ time_sensors[1][0] = s_count; statusWork |= 1 << 2; }
	}
	void callBack_s2(bool stat)
	{
		bool sensorStat;
		if (debugOn)	sensorStat = debugSensor[2];
		else			sensorStat = sensor[2]->stat;
		//
		if ( sensorStat && ((statusWork & (1 << 5)) == 0))	{ time_sensors[2][1] = s_count; statusWork |= 1 << 5; }
		if (!sensorStat && ((statusWork & (1 << 4)) == 0))	{ time_sensors[2][0] = s_count; statusWork |= 1 << 4; }
		
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
		if ((!sensor[0]->stat) && (!sensor[1]->stat) && ( sensor[2]->stat))
		{
			blockSensor = true;
		}
	}
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
		sensor[2] = new Sensor(2, &DDRC, &PORTC, &PINC, 2, callBack);
		initIntegr();
		s_count = 0;
		blockSensor = false;
		statusWork = 0;
	}
	// ===============
	void interrupt()
	{
		s_count++;
		sensor[0]->interrupt();
		sensor[1]->interrupt();
		sensor[2]->interrupt();
		SCR->Hex(13, PINC);
		// датчики освобождены - начало замера
// 		if (!blockSensor && !sensor[0]->stat && !sensor[0]->stat && !sensor[0]->stat)
// 		{
// 			statusWork = 0;
// 			s_count = 0;
// 		}
	}
	// ========================
	void startOfDataCollection()
	{
		blockSensor = false;
		if (!sensor[0]->stat && !sensor[0]->stat && !sensor[0]->stat)
		{
			statusWork = 0;
			s_count = 0;
		}
	}
	// =====================
	bool getStatSensor(uint8_t nSensor)
	{
		if (debugOn)
		{
			return debugSensor[nSensor];
		}
		else
		{
			return sensor[nSensor]->stat;
		}
	}
	// ====================
	bool getReadyData()
	{
		return blockSensor;
	}
	signed int renderLenght()
	{
		if (!blockSensor)	return -1;
		// определение базы
		int16_t baseMainSmall = sensorPosition[2] - sensorPosition[1];
		int16_t baseMainBig = sensorPosition[2] - sensorPosition[0];
		//
		int16_t baseRender = sensorPosition[1] - sensorPosition[0];
	
		// до счет в минус на малой базе
		int32_t timeRenderMinusBaseSmall	= time_sensors[1][0] - time_sensors[2][1];
		// до счет в плюс на большой базе
		int32_t timeRenderPlusBaseBig		= time_sensors[2][1] - time_sensors[0][0];
		// до счет в плюс на малой базе
		int32_t timeRenderPlusBaseSmall		= time_sensors[2][1] - time_sensors[1][0];
		// до счет в минус на большой базе
		int32_t timeRenderMinusBaseBig		= time_sensors[0][0] - time_sensors[2][1];
		//
		int32_t timeRender					= time_sensors[0][0] - time_sensors[1][0];
		
		// до счет в минус на малой базе
		if ( time_sensors[1][0] >= time_sensors[2][1])
		{
			return baseMainSmall - (baseRender * timeRenderMinusBaseSmall / timeRender);
		}
		// до счет в плюс на большой базе
		if ( time_sensors[2][1] >= time_sensors[0][0] )
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
}
