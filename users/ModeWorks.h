/* 
* ModeWorks.h
*
* Created: 25.05.2020 10:39:30
* Author: User
*/
#ifndef __PATH_H_
#include "path.h"
#endif


#ifndef __MODEWORKS_H__
#define __MODEWORKS_H__

// ������ ������
#define		WMODE_M_ALARM		0
#define		WMODE_M_BACK		1
#define		WMODE_M_STOP		2
#define		WMODE_M_PUSK		3
#define		WMODE_M_SHELF		4
//#define		WMODE_C_ALARM		5
#define		WMODE_C_BACK		6
#define		WMODE_C_DELAY		7
#define		WMODE_C_PUSK		8
#define		WMODE_C_SHELF		9
#define		WMODE_I_INIT		10
#define		WMODE_RESET			11

// ���� ������ alarm
#define		ALARM_CODE_NONE				0
// ���������� ������ ��������� ����������
#define		ALARM_CODE_ABORT			1
// ������ �������������
#define		ALARM_CODE_SWITCH			2
// �������� �������� ��� ����� ���������� �����
#define		ALARM_CODE_SAFE_ENGINE_ON	3
// �������� �������� ��� ����� ���������� �����
#define		ALARM_CODE_SAFE_ENGINE_OFF	4

// -----------------------------
// **************** ������� �� ���������� *****************
// ���������� ��������������� ������ ������
#define		_FromPcCommand_Stop			0x80
// ���������� ������ � �������������� ������
#define		_FromPcCommand_CycleMax		0x81
// ����� ����������
#define		_FromPcCommand_Calibration	0x82

extern const TypeFunct1 Sw_Event[1];

namespace ns_ModeWorks
{
	// ���������
	void Init();
	// ����������
	void InterruptTick();
	void Interrupt();
	// =====================
 	extern uint32_t		ticker;
	// =====================
	// ����� ������
	extern uint8_t		mode;
	extern uint8_t		flBackWork;
	// =====================
	// ���������� � ������
	extern uint16_t speedMoveStock;
	// =====================
	extern const char *Message_Status[];
	// ---------------------- 0x81 ------------------------------
	//                 ����� ����������
	// ���� ����������
	extern uint8_t flag_calibrovka;
	// ������� ������� ���������� �������� ������
	extern uint8_t calibrovkaBackTimeSendToPc;
	// ����� �������� ���������� �������� ������
	extern const uint8_t calibrovkaBackTimeSendToPc_SET;
	// ������� ������� ���������� ������ ����������
	extern uint16_t calibrovkaBackTimeFlag_mode;
	// ����� ���������� ������ ���������� �� ������� �������
	extern const uint16_t calibrovkaBackTimeFlag_SET;
	// ********************** ��������� ��������� ***************************
	// �������� �������� ��� ������ ��������� (�����)
	extern uint16_t safeIntervalEngineOn_set;
	// -------------------------------------------------------------------------------------
	// �������� �������� ��� �������� ��������� (�����)
	extern uint16_t safeIntervalEngineOff_set;
	// -------------------------------------------------------------------------------------
	// ����� ��������� ����������� ������ ���������� �����
	extern uint16_t thresholdSpeedStockTimeForwardStart_set;
	// ����� ��������� ����������� ��������� ���������� �����
	extern uint16_t thresholdSpeedStockTimeForwardStop_set;
	// ����� ��������� ����������� ������ ���������� �����
	extern uint16_t thresholdSpeedStockTimeBackStart_set;
	// ����� ��������� ����������� ��������� ���������� �����
	extern uint16_t thresholdSpeedStockTimeBackStop_set;
	// ===========================================================================
	// ����� �������� ������ ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	extern uint16_t thresholdSpeedStockForward_start;
	// ����� �������� ��������� ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	extern uint16_t thresholdSpeedStockForward_stop;
	// -------------------------------------------------------------------------------------
	// ����� �������� ������ ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	extern uint16_t thresholdSpeedStockBack_start;
	// ����� �������� ��������� ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	extern uint16_t thresholdSpeedStockBack_stop;
	// ===========================================================================
	// ����� ���������� ����� �� ����� (�����)
	extern uint16_t timeStockShelf_set;
	// -------------------------------------------------------------------------------------
	// ����� ���������� ����� � �������� ��������� (�����)
	extern uint16_t timeStockBegin_set;
	// -------------------------------------------------------------------------------------
	// ����������� ����� alarm
	extern uint16_t timeAlarm_set;
	// ��� ������ �����
	extern uint8_t alarmCode;
	// ===========================================================================
	// ������������ ����� ��������� �� ��������
	extern uint8_t n_cycle_max;
}

#endif //__MODEWORKS_H__
