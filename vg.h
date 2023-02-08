/* 
* vg.h
*
* Created: 07.11.2017 13:19:00
* Author: GumerovMN
*/


#ifndef __PATH_H_
#include "path.h"
#endif

#ifndef __VG_H__
#define __VG_H__

#include <avr/eeprom.h>

namespace VG
{
	// *************** adc core **********************
	// ����� ������� ���������� ��� ���������� ��������
	extern uint16_t EEMEM adcCore_speedLenghtAve;
	extern uint16_t adcCore_speedLenghtAve_ram;
	// ����� ������� ������ ��� ���������� ��������
	extern uint8_t EEMEM adcCore_speedLenghtDlt;
	extern uint8_t adcCore_speedLenghtDlt_ram;
	// ����� ������� ���������� ������ ��� ��������� ���������
	extern uint8_t EEMEM adcCore_distanceLenghtAve;
	// ����� ������� ���������� ������ ��� ��������� �������
	extern uint8_t EEMEM adcCore_forceLenghtAve;
	// ************* mode work ***********************
	// �������� �������� ��� ������ ��������� (�����)
	extern uint16_t EEMEM safeIntervalEngineOn_set;
	// -------------------------------------------------------------------------------------
	// �������� �������� ��� �������� ��������� (�����)
	extern uint16_t EEMEM safeIntervalEngineOff_set;
	// -------------------------------------------------------------------------------------
	// ����� ��������� ����������� ������ ���������� �����
	extern uint16_t EEMEM thresholdSpeedStockTimeForwardStart_set;
	// ����� ��������� ����������� ��������� ���������� �����
	extern uint16_t EEMEM thresholdSpeedStockTimeForwardStop_set;
	// ����� ��������� ����������� ������ ���������� �����
	extern uint16_t EEMEM thresholdSpeedStockTimeBackStart_set;
	// ����� ��������� ����������� ��������� ���������� �����
	extern uint16_t EEMEM thresholdSpeedStockTimeBackStop_set;
	// ===========================================================================
	// ����� �������� ������ ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	extern uint16_t EEMEM thresholdSpeedStockForward_start;
	// ����� �������� ��������� ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	extern uint16_t EEMEM thresholdSpeedStockForward_stop;
	// -------------------------------------------------------------------------------------
	// ����� �������� ������ ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	extern uint16_t EEMEM thresholdSpeedStockBack_start;
	// ����� �������� ��������� ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	extern uint16_t EEMEM thresholdSpeedStockBack_stop;
	// ===========================================================================
	// ����� ���������� ����� �� ����� (�����)
	extern uint16_t EEMEM timeStockShelf_set;
	// -------------------------------------------------------------------------------------
	// ����� ���������� ����� � �������� ��������� (�����)
	extern uint16_t EEMEM timeStockDelay_set;
	// -------------------------------------------------------------------------------------
	// ����������� ����� alarm
	extern uint16_t EEMEM timeAlarm_set;
	// ===========================================================================
	// ������������ ����� ��������� �� ��������
	extern uint8_t EEMEM n_cycle_max;
	// ===========================================================================
	// ����� ������������ ��������� �������������****
	extern uint16_t EEMEM switch1_delay;
	extern uint16_t EEMEM switch2_delay;
}

#endif //__VG_H__
