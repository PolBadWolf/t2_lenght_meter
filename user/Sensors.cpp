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
	int32_t		time_sensors[3][2];								// ������ ����������� ����� � �����
	int32_t		s_count;										// ������� �����
	int32_t		v_count;										// ������� �����
	int32_t		t_count;										// ������� ����� ��� ��������
	Sensor		*sensor[3];										// ������� �������
	uint8_t		statusWork = 0;									// ���������� �� ��������� ������������ ������� �� ��������
	bool		blockSensor;									// ������ ���������� � ����� ������/���������� � ������� �����
	bool		debugOn = false;
	bool		debugSensor[3];
	bool		*sensorMass[3];
	bool		blockirovka;									// ���������� ������ ������� ��������
	bool		countTimeOut;									//
	// =====
	int32_t		s_count_timeOut;
	int32_t		e_count_timeOut EEMEM	= 5000;
	//
	uint8_t		s_sensorInt[3];									// ����� ���������� ������ �� ��������
	uint8_t		e_sensorInt[3] EEMEM	= {20, 20, 20};
	//
	int16_t		s_sensorPosition[3];									// �������� ��������
	int16_t		e_sensorPosition[3] EEMEM	= {0, 1000, 11045};			// �������� ��������
	// =========================
	Sensor		*blockIzmer;
	// ********************************
	volatile uint8_t		sensorsStep = SENSORS_STEP_NoZero;
	uint16_t safeTimeZero = 0;

	void ee_load()
	{
		// ����� ���������� ������
		for (uint8_t i = 0; i < 3; i++)
		{
			s_sensorInt[i] = eeprom_read_byte((uint8_t *) & e_sensorInt[i]);
		}
		// ����� zero time_out
		eeprom_read_block(&s_count_timeOut, &e_count_timeOut, sizeof(s_count_timeOut));
		// �������� ��������
		for (uint8_t i = 0; i < 3; i++)
		{
			eeprom_read_block(&s_sensorPosition[i], &e_sensorPosition[i], sizeof(s_sensorPosition[0]));
		}
	}
	void ee_save()
	{
		// ����� ���������� ������
		for (uint8_t i = 0; i < 3; i++)
		{
			eeprom_update_byte((uint8_t *) & e_sensorInt[i], s_sensorInt[i]);
		}
		// ����� zero time_out
		eeprom_update_block(&s_count_timeOut, &e_count_timeOut, sizeof(s_count_timeOut));
		// �������� ��������
		for (uint8_t i = 0; i < 3; i++)
		{
			eeprom_update_block(&s_sensorPosition[i], &e_sensorPosition[i], sizeof(s_sensorPosition[0]));
		}
	}
	// ==========
	void callBack_s0(bool stat)
	{
		bool sensorStat = *sensorMass[0];
		if ( sensorStat)
		{	// ����� �� ������
			if ( ((statusWork & (1 << 0)) == 0) && !(*sensorMass[1]) && !(*sensorMass[2]) && (sensorsStep == SENSORS_STEP_Zero))
			{	// ������ ���������
				statusWork |= 1 << 0;
				s_count = 0;
				time_sensors[0][1] = s_count;
				countTimeOut = false;
				blockirovka = false;
				sensorsStep = SENSORS_STEP_Start;
			}
		}
		else
		{	// ����� � �������
			if ((statusWork & (1 << 1)) == 0)
			{
				time_sensors[0][0] = s_count;
				statusWork |= 1 << 1;
			}
		}
	}
	void callBack_s1(bool stat)
	{
		bool sensorStat = *sensorMass[1];
		if (sensorStat)
		{	// ����� �� ������
			if ((statusWork & (1 << 2)) == 0)	{ time_sensors[1][1] = s_count; statusWork |= 1 << 2; }
		}
		else
		{	// ����� � �������
			if ((statusWork & (1 << 3)) == 0)	{ time_sensors[1][0] = s_count; statusWork |= 1 << 3; }
		}
	}
	void callBack_s2(bool stat)
	{
		bool sensorStat = *sensorMass[2];
		if (sensorStat)
		{	// ����� �� ������
			if ((statusWork & (1 << 4)) == 0)	{ time_sensors[2][1] = s_count; statusWork |= 1 << 4; }
		}
		else
		{	// ����� � �������
			if ((statusWork & (1 << 5)) == 0)	{ time_sensors[2][0] = s_count; statusWork |= 1 << 5; }
		}
	}
	void callBack(uint8_t nSensor, bool stat)
	{
		if (debugOn)		sensorMass[nSensor] = &debugSensor[nSensor];
		else				sensorMass[nSensor] = &sensor[nSensor]->stat;
		//��������� �������������
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
		// �������� �� ��������� ����� ������
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
		blockIzmer->setCountMax(40);
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
		// ������������� ������� ��������
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
	uint16_t	delayZero = 0;
	void interrupt()
	{
		if (!*sensorMass[0] && !*sensorMass[1] && !*sensorMass[2])
		{
			if ( ((sensorsStep == SENSORS_STEP_NoZero) /*|| (sensorsStep == SENSORS_STEP_BigTimeOut) || (sensorsStep == SENSORS_STEP_SmlTimeout)*/) && (++delayZero == 1000 ) )
			{
				sensorsStep = SENSORS_STEP_Zero;
				statusWork = 0;
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
		if ( ((statusWork & (1 << 0)) && ( (sensorsStep == SENSORS_STEP_Start) || (sensorsStep == SENSORS_STEP_BlockIzm) ) ) )	s_count++;
		if (s_count != 0 && s_count != s_count_timeOut) v_count = s_count;
		// Big Time Out
		if ( (s_count >= 20000) && (sensorsStep != SENSORS_STEP_NoZero) )
		{
			sensorsStep = SENSORS_STEP_BigTimeOut;
			s_count = 0;
			countTimeOut = true;	// *************
		}
		sensor[0]->interrupt();
		sensor[1]->interrupt();
		sensor[2]->interrupt();
		//
		blockIzmer->interrupt();
		blockirovka |= blockIzmer->stat;
		if ( (!*sensorMass[0] && !*sensorMass[1] && *sensorMass[2]) && (sensorsStep == SENSORS_STEP_Start) )
		{
			sensorsStep = SENSORS_STEP_Ready;
		}
		// ************
		if ( (sensorsStep == SENSORS_STEP_Zero)  && !*sensorMass[0] && !*sensorMass[1] && !*sensorMass[2]  )
		{
			if (safeTimeZero < 500)	safeTimeZero++;
			if (safeTimeZero == 500)
			{
				safeTimeZero = 501;
				statusWork = 0;
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
	// =====================
	bool getStatSensor(uint8_t nSensor)
	{
		return *sensorMass[nSensor];
	}
	// ====================
	int getReadyData()
	{
		if (countTimeOut)	return		SENSORS_READY_TimeOutCn;
		if (!blockSensor)	return		SENSORS_READY_NotReady;
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
		if (time_sensors[0][0] < time_sensors[1][1])	return -6;
		// ����������� ����
		int16_t baseMainSmall = s_sensorPosition[2] - s_sensorPosition[1];
		int16_t baseMainBig = s_sensorPosition[2] - s_sensorPosition[0];
		//
		int16_t baseRender = s_sensorPosition[1] - s_sensorPosition[0];
	
		// �� ���� � ����� �� ����� ����
		int32_t timeRenderMinusBaseSmall	= time_sensors[2][1] - time_sensors[1][0];
		// �� ���� � ���� �� ������� ����
		int32_t timeRenderPlusBaseBig		= time_sensors[0][0] - time_sensors[2][1];
		// �� ���� � ���� �� ����� ����
		int32_t timeRenderPlusBaseSmall		= time_sensors[1][0] - time_sensors[2][1];
		// �� ���� � ����� �� ������� ����
		int32_t timeRenderMinusBaseBig		= time_sensors[2][1] - time_sensors[0][0];
		//
		int32_t timeRender					= time_sensors[1][0] - time_sensors[0][0];

		// �� ���� � ����� �� ����� ����
		if ( (time_sensors[2][1] >= time_sensors[1][0]) && (time_sensors[1][0] >= time_sensors[0][0]))
		{
			return baseMainSmall - (baseRender * timeRenderMinusBaseSmall / timeRender);
		}
		// �� ���� � ���� �� ������� ����
		if ( time_sensors[0][0] >= time_sensors[2][1] )
		{
			return baseMainBig + (baseRender * timeRenderPlusBaseBig / timeRender);
		}
		// �� ���� � ���� �� ����� ����
		if ( timeRenderMinusBaseBig >= timeRenderPlusBaseSmall )
		{
			return baseMainSmall + (baseRender * timeRenderPlusBaseSmall / timeRender);
		}
		// �� ���� � ����� �� ������� ����
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
	void mainCicle()
	{
// 		SCR->DigitZ(6, 1, sensorsStep);
// 		SCR->Hex(8, statusWork);
	}
	unsigned char getStatusWork()
	{
		return statusWork;
	}
}
