/* 
* Sensors.cpp
*
* Created: 28.02.2023 16:35:02
* Author: Gumerovmn
*/


#include "Sensors.h"
#include "Sensor.h"

namespace ns_sensors
{
	uint32_t	time_sensors[3][2];
	uint8_t		s_sensorInt[3];
	uint8_t		e_sensorInt[3] EEMEM = {20, 20, 20};
	uint32_t	s_count;
	Sensor		*sensor[3];
	uint8_t		statusWork = 255;
	bool		blockSensor;
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
		// начало измерения
		if (( sensor[0]->stat) && ((statusWork & (1 << 0)) == 0))
		{
			if (!sensor[1]->stat && !sensor[2]->stat)
			{
				statusWork |= 1 << 0;
				s_count = 0;
				time_sensors[0][1] = s_count;
			}
		}
		if ((!sensor[0]->stat) && ((statusWork & (1 << 0)) == 0))	{ time_sensors[0][0] = s_count; statusWork |= 1 << 0; }
	}
	void callBack_s1(bool stat)
	{
		if (( sensor[1]->stat) && ((statusWork & (1 << 3)) == 0))	{ time_sensors[1][1] = s_count; statusWork |= 1 << 3; }
		if ((!sensor[1]->stat) && ((statusWork & (1 << 2)) == 0))	{ time_sensors[1][0] = s_count; statusWork |= 1 << 2; }
	}
	void callBack_s2(bool stat)
	{
		if (( sensor[2]->stat) && ((statusWork & (1 << 5)) == 0))	{ time_sensors[2][1] = s_count; statusWork |= 1 << 5; }
		if ((!sensor[2]->stat) && ((statusWork & (1 << 4)) == 0))	{ time_sensors[2][0] = s_count; statusWork |= 1 << 4; }
		
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
	}
	// ===============
	void interrupt()
	{
		s_count++;
		sensor[0]->interrupt();
		sensor[1]->interrupt();
		sensor[2]->interrupt();
		// датчики освобождены - начало замера
		if (!sensor[0]->stat && !sensor[0]->stat && !sensor[0]->stat)	statusWork = 0;
	}
	// ========================
	void startOfDataCollection()
	{
		blockSensor = false;
	}
	// =====================
	bool getReadyData()
	{
		return blockSensor;
	}
}
