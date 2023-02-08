/* 
* LoadVars.cpp
*
* Created: 17.06.2020 8:23:16
* Author: Gumerovmn
*/

#include "LoadVars.h"

namespace ns_ModeWorks
{
}

void LoadVars()
{
	// ************* mode work ***********************
	// �������� �������� ��� ������ ��������� (�����)
	eeprom_read_block(&ns_ModeWorks::safeIntervalEngineOn_set, &VG::safeIntervalEngineOn_set, sizeof(uint16_t));
	// -------------------------------------------------------------------------------------
	// �������� �������� ��� �������� ��������� (�����)
	eeprom_read_block(&ns_ModeWorks::safeIntervalEngineOff_set, &VG::safeIntervalEngineOff_set, sizeof(uint16_t));
	// -------------------------------------------------------------------------------------
	// ����� ��������� ����������� ������ ���������� �����
	eeprom_read_block(&ns_ModeWorks::thresholdSpeedStockTimeForwardStart_set, &VG::thresholdSpeedStockTimeForwardStart_set, sizeof(uint16_t));
	// ����� ��������� ����������� ��������� ���������� �����
	eeprom_read_block(&ns_ModeWorks::thresholdSpeedStockTimeForwardStop_set, &VG::thresholdSpeedStockTimeForwardStop_set, sizeof(uint16_t));
	// ����� ��������� ����������� ������ ���������� �����
	eeprom_read_block(&ns_ModeWorks::thresholdSpeedStockTimeBackStart_set, &VG::thresholdSpeedStockTimeBackStart_set, sizeof(uint16_t));
	// ����� ��������� ����������� ��������� ���������� �����
	eeprom_read_block(&ns_ModeWorks::thresholdSpeedStockTimeBackStop_set, &VG::thresholdSpeedStockTimeBackStop_set, sizeof(uint16_t));
	// ===========================================================================
	// ����� �������� ������ ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	eeprom_read_block(&ns_ModeWorks::thresholdSpeedStockForward_start, &VG::thresholdSpeedStockForward_start, sizeof(uint16_t));
	// ����� �������� ��������� ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	eeprom_read_block(&ns_ModeWorks::thresholdSpeedStockForward_stop, &VG::thresholdSpeedStockForward_stop, sizeof(uint16_t));
	// -------------------------------------------------------------------------------------
	// ����� �������� ������ ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	eeprom_read_block(&ns_ModeWorks::thresholdSpeedStockBack_start, &VG::thresholdSpeedStockBack_start, sizeof(uint16_t));
	// ����� �������� ��������� ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	eeprom_read_block(&ns_ModeWorks::thresholdSpeedStockBack_stop, &VG::thresholdSpeedStockBack_stop, sizeof(uint16_t));
	// ===========================================================================
	// ����� ���������� ����� �� ����� (�����)
	eeprom_read_block(&ns_ModeWorks::timeStockShelf_set, &VG::timeStockShelf_set, sizeof(uint16_t));
	// -------------------------------------------------------------------------------------
	// ����� ���������� ����� � �������� ��������� (�����)
	eeprom_read_block(&ns_ModeWorks::timeStockBegin_set, &VG::timeStockDelay_set, sizeof(uint16_t));
	// -------------------------------------------------------------------------------------
	// ����������� ����� alarm
	eeprom_read_block(&ns_ModeWorks::timeAlarm_set, &VG::timeAlarm_set, sizeof(uint16_t));
	// ===========================================================================
	// ������������ ����� ��������� �� ��������
	eeprom_read_block(&ns_ModeWorks::n_cycle_max, &VG::n_cycle_max, sizeof(uint8_t));
}