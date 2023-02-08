/*
* ModeWorks.cpp
*
* Created: 25.05.2020 10:39:30
* Author: User
*/


#include "ModeWorks.h"
#include <avr/eeprom.h>

// ==============================
// ������������� ������ ������ �� ������, �������� �����
namespace ns_ModeWorks { void ModeCallBack(); }
const TypeFunct1 Sw_Event[1] PROGMEM = {ns_ModeWorks::ModeCallBack};
// ==============================
// ���������
#define ENGINE_INIT		DDRC	|=	(1 << 6)
#define ENGINE_ON		PORTC	&= ~(1 << 6)//; 
//scr::PutChar(13, '1')
#define ENGINE_OFF		PORTC	|=  (1 << 6)//; 
//scr::PutChar(13, '0')

#define switchMode	ns_device::switchMode
#define core		ns_device::core

// -----------------------------------------------
// � �������
#define M_STOP	0
#define M_PUSK	1
#define M_CICL	2
#define M_NONE	3
#define M_SETM	4

typedef void(*TypeFnMode)();
#define FnMode(mode, stat)	((TypeFnMode)pgm_read_ptr(& MassFnMn[mode][stat]))()
#define SetMode(Mode)		FnMode(Mode, M_SETM)

namespace ns_ModeWorks
{
	// *********************** ���������� ���������� *****************************
	extern const TypeFnMode MassFnMn[][5];
	// ����� ������
	uint8_t mode = WMODE_M_ALARM;
	// ���� ������ ��������� �������������
	uint8_t flNewSw = 0;
	// ��������� �����������
	uint8_t switchPos = 0;
	// �������� �������� �����
	uint16_t speedMoveStock = 0;
	// ���� �������� �����
	uint8_t flagMoveStock;
	// ***************** ���������� ��������/������������ �� ��� *****************
 	uint32_t	ticker;
	// ************************* ��������� ��������� *****************************
	// �������� �������� ��� ������ ��������� (�����)
	uint16_t safeIntervalEngineOn_set			= 1000;
	uint16_t safeIntervalEngineOn_value			= safeIntervalEngineOn_set;
	// -------------------------------------------------------------------------------------
	// �������� �������� ��� �������� ��������� (�����)
	uint16_t safeIntervalEngineOff_set			= 1000;
	uint16_t safeIntervalEngineOff_value		= safeIntervalEngineOff_set;
	// -------------------------------------------------------------------------------------
	// ����� ��������� ����������� ������ ���������� �����
	uint16_t thresholdSpeedStockTimeForwardStart_set	= 200;
	uint16_t thresholdSpeedStockTimeForwardStart_value	= thresholdSpeedStockTimeForwardStart_set;
	// ����� ��������� ����������� ��������� ���������� �����
	uint16_t thresholdSpeedStockTimeForwardStop_set		= 200;
	uint16_t thresholdSpeedStockTimeForwardStop_value	= thresholdSpeedStockTimeForwardStop_set;
	// ����� ��������� ����������� ������ ���������� �����
	uint16_t thresholdSpeedStockTimeBackStart_set		= 200;
	uint16_t thresholdSpeedStockTimeBackStart_value		= thresholdSpeedStockTimeBackStart_set;
	// ����� ��������� ����������� ��������� ���������� �����
	uint16_t thresholdSpeedStockTimeBackStop_set		= 200;
	uint16_t thresholdSpeedStockTimeBackStop_value		= thresholdSpeedStockTimeBackStop_set;
	// ===========================================================================
	// ����� �������� ������ ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	uint16_t thresholdSpeedStockForward_start	= 15;
	// ����� �������� ��������� ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	uint16_t thresholdSpeedStockForward_stop	= 10;
	// -------------------------------------------------------------------------------------
	// ����� �������� ������ ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	uint16_t thresholdSpeedStockBack_start		= 15;
	// ����� �������� ��������� ���������� ����� ( (������� ��� / ���) * ����� ������� ���������� ������ )
	uint16_t thresholdSpeedStockBack_stop		= 10;
	// ===========================================================================
	// ����� ���������� ����� �� ����� (�����)
	uint16_t timeStockShelf_set					= 3000;
	uint16_t timeStockShelf_value				= timeStockShelf_set;
	// -------------------------------------------------------------------------------------
	// ����� ���������� ����� � �������� ��������� (�����)
	uint16_t timeStockBegin_set					= 3000;
	uint16_t timeStockBegin_value				= timeStockBegin_set;
	// -------------------------------------------------------------------------------------
	// ����������� ����� alarm
	uint16_t timeAlarm_set						= 5000;	// � ��������
	uint16_t timeAlarm_value					= timeAlarm_set;
	// -------------------------------------------------------------------------------------
	// ��� ������ �����
	uint8_t alarmCode							= ALARM_CODE_NONE;
	uint16_t autoRepeatSendCout					= 0;
	#define  autoRepeatSendCoutMax				  1000
	// ===========================================================================
	// ������� ������ ��������� �� ��������
	uint8_t n_cycle_value;
	// ������������ ����� ��������� �� ��������
	uint8_t n_cycle_max = 15;
	// ===========================================================================
	// ********** �����  ������ � ���������� **********
	#define READPCMODE_SYNHRO		0
	#define READPCMODE_LENGHT		1
	#define READPCMODE_BODY			2
	void readFromPC();
	void readFromPC_fillSynhroBuf(uint8_t byte);
	void readPC_CheckedSynhro();
	// ---------------------- 0x80 ------------------------------
	//                ���� �� ����������
	uint8_t flag_StopFromPc = 0;
	// ---------------------- 0x81 ------------------------------
	//                 ����� ����������
	// ���� ����������
	uint8_t flag_calibrovka = 0;
	// ������� ������� ���������� �������� ������
	uint8_t calibrovkaBackTimeSendToPc = 0;
	// ����� �������� ���������� �������� ������
	const uint8_t calibrovkaBackTimeSendToPc_SET = 10;
	// ������� ������� ���������� ������ ����������
	uint16_t calibrovkaBackTimeFlag_mode = 0;
	// ����� ���������� ������ ���������� �� ������� �������
	const uint16_t calibrovkaBackTimeFlag_SET = 2000;
	//
	void calibrovka_checkSetFlags();
	void InitModeSet();
	// ===========================================================================
	// �������� ����� �� ������ �������������
	void ModeCallBack()
	{
		CRITICAL_SECTION
		{
			flNewSw = 1;
			switchPos = switchMode->Status();
		}
	}
	// =====================
	void setModeAlarm(uint8_t code) {
		alarmCode = code;
		SetMode(WMODE_M_ALARM);
	}
	// =====================
	// ���������
	void Init()
	{
		flag_StopFromPc = 0;
		ENGINE_INIT;
		ticker = 0;
		LoadVars();
		InitModeSet();
		switchPos = SWITCH_POS_INIT;
	}
	// ���������� ����������
	void InterruptTick()
	{
		ticker++;
		if (calibrovkaBackTimeFlag_mode > 0) calibrovkaBackTimeFlag_mode--;
		if (calibrovkaBackTimeSendToPc > 0) calibrovkaBackTimeSendToPc--;
	}
	// ���������� �� �������������
	void Interrupt()
	{
//		scr::Digit (scr::SetPosition(11, 1), 4, calibrovkaBackTimeFlag_mode);
		// ������ ������ �� ����������
		readFromPC();
		// ��������� ������ ����������
		calibrovka_checkSetFlags();
		// � ������ ����������
		if (flag_calibrovka != 0) {
			if (switchPos == SWITCH_POS_PUSK) {
				// �������� ���������
				ENGINE_ON;
			} else {
				// ��������� ���������
				ENGINE_OFF;
			}
			return;
		}
		// ������ ���������� � �������
		speedMoveStock = core->getSpeed();
		uint8_t stat = M_NONE;
		if (flNewSw)
		{
			flNewSw = 0;
			switch (switchPos)
			{
				// sop
				case SWITCH_POS_STOP:
				stat = M_STOP;
				break;
				case SWITCH_POS_PUSK:
				stat = M_PUSK;
				break;
				case SWITCH_POS_CICL:
				stat = M_CICL;
				break;
			}
		}
		FnMode(mode, stat);
	}
	void Dupm() {}
	// ===================================================
	// �������� ���������� � ����� � ����� "����"
	void ManualAlarmSet()
	{
		// ��������� ���������
		ENGINE_OFF;
		// ����� ������ � ���������
		//core->sendToPc_Mode(SEND2PC_MALARM);
		core->sendToPc_Alarm(alarmCode);
		// ����� ������
		mode = WMODE_M_ALARM;
		// ����������� ����� alarm
		timeAlarm_value = timeAlarm_set;
		//
		autoRepeatSendCout = autoRepeatSendCoutMax;
	}
	void ManualAlarmNone()
	{
		if (autoRepeatSendCout == 0) {
			core->sendToPc_Alarm(alarmCode);
			autoRepeatSendCout = autoRepeatSendCoutMax;
		} else {
			autoRepeatSendCout--;
		}
		// �������� ��������� �����
		if ( speedMoveStock > thresholdSpeedStockBack_stop ) {
			timeAlarm_value = timeAlarm_set;
			return;
		}
		// ����� � ������ �����
		if (timeAlarm_value > 0) {
			timeAlarm_value--;
			return;
		}
		// ����� ������ ����
		if (switchPos == SWITCH_POS_STOP){
			// ����� � pc
			core->sendToPc_Mode(SEND2PC_RESET);
			SetMode(WMODE_M_STOP);
		}
	}
	// ===================================================
	void ManualBackSet()
	{
		mode = WMODE_M_BACK;
		// ����� ������ � ���������
		core->sendToPc_Mode(SEND2PC_MBACK);
		// ��������� ���������
		ENGINE_OFF;
		// ���� �������� �����
		flagMoveStock = 0;
		// �������� �������� ��� �������� ��������� (�����)
		safeIntervalEngineOff_value = safeIntervalEngineOff_set;
		// ����� ��������� ����������� ������ ���������� �����
		thresholdSpeedStockTimeBackStart_value = thresholdSpeedStockTimeBackStart_set;
	}
	void ManualBackNone()
	{
		// �������� �������� ��� �������� ��������� (�����)
		if (--safeIntervalEngineOff_value == 0) {
			// ���� ��������� - ������
			//SetMode(WMODE_M_ALARM);
			setModeAlarm(ALARM_CODE_SAFE_ENGINE_OFF);
			return;
		}
		// ���� �������� �����
		if (flagMoveStock == 0) {
			// �������� ������ ���������� �����
			if (speedMoveStock > thresholdSpeedStockBack_start) {
				if (thresholdSpeedStockTimeBackStart_value > 0) thresholdSpeedStockTimeBackStart_value--;
				else {
					// ����� ��������� ����������� ��������� ���������� �����
					thresholdSpeedStockTimeBackStop_value = thresholdSpeedStockTimeBackStop_set;
					flagMoveStock = 1;
				}
			} else {
				thresholdSpeedStockTimeBackStart_value = thresholdSpeedStockTimeBackStart_set;
			}
		} else {
			// �������� ��������� �����
			if (speedMoveStock < thresholdSpeedStockBack_stop) {
				if (thresholdSpeedStockTimeBackStop_value > 0) thresholdSpeedStockTimeBackStop_value--;
				else {
					// ���������� �����
					SetMode(WMODE_M_STOP);
				}
			} else {
				thresholdSpeedStockTimeBackStop_value = thresholdSpeedStockTimeBackStop_set;
			}
		}
	}
	// ===================================================
	void ManualStopSet()
	{
		// ��������� ���������
		ENGINE_OFF;
		uint8_t old_mod = mode;
		mode = WMODE_M_STOP;
		// ����� ������ � ���������
		if (old_mod == WMODE_C_BACK) {
			core->sendToPc_Mode_sub(SEND2PC_STOP, thresholdSpeedStockTimeForwardStop_set + VG::adcCore_speedLenghtAve_ram + VG::adcCore_speedLenghtDlt_ram);
		} else {
			core->sendToPc_Mode(SEND2PC_STOP);
		}
		//
		autoRepeatSendCout = autoRepeatSendCoutMax;
	}
	void ManualStopNone() {
		if (autoRepeatSendCout == 0) {
			core->sendToPc_Mode(SEND2PC_RESET);
			core->sendToPc_Mode(SEND2PC_STOP);
			autoRepeatSendCout = autoRepeatSendCoutMax;
			} else {
			autoRepeatSendCout--;
		}
	}
	// ===================================================
	// ���� ������, ������ ���������
	void ManualPuskSet()
	{
		// �������� ��������� ��������
		LoadVars();
		// ����� ������ ���������
		CRITICAL_SECTION { ticker = 0; }
		// ����� ������
		mode = WMODE_M_PUSK;
		// ����� ������ � ���������
		core->sendToPc_Mode(SEND2PC_MFORWARD);
		// ��������� ������ � ���������
		core->sendToPc_ForceZero();
		// ���� �������� �����
		flagMoveStock = 0;
		// �������� �������� ��� ������ ��������� (�����)
		safeIntervalEngineOn_value = safeIntervalEngineOn_set;
		// ����� ��������� ����������� ������ ���������� �����
		thresholdSpeedStockTimeForwardStart_value = thresholdSpeedStockTimeForwardStart_set;
		// �������� ���������
		ENGINE_ON;
	}
	// ���� ������
	void ManualPuskNone() {
		// �������� �������� ��� ������ ��������� (�����)
		if (--safeIntervalEngineOn_value == 0) {
			// ����� ������ ������
			//SetMode(WMODE_M_ALARM);
			setModeAlarm(ALARM_CODE_SAFE_ENGINE_ON);
			return;
		}
		if (flagMoveStock == 0) {
			// �������� �������� �����
			if (speedMoveStock > thresholdSpeedStockForward_start) {
				if (thresholdSpeedStockTimeForwardStart_value > 0) thresholdSpeedStockTimeForwardStart_value--;
				else {
					// ����� �������� ����������� ��������� ���������� �����
					thresholdSpeedStockTimeForwardStop_value = thresholdSpeedStockTimeForwardStop_value;
					flagMoveStock = 1;
				}
			} else {
				thresholdSpeedStockTimeForwardStart_value = thresholdSpeedStockTimeForwardStart_set;
			}
		} else {
			// �������� ���������� �����
			if (speedMoveStock < thresholdSpeedStockForward_stop) {
				if (thresholdSpeedStockTimeForwardStop_value > 0) thresholdSpeedStockTimeForwardStop_value--;
				else {
					// ����� ������ �����
					SetMode(WMODE_M_SHELF);
				}
			} else {
				thresholdSpeedStockTimeForwardStop_value = thresholdSpeedStockTimeForwardStop_set;
			}
		}
	}
	// ������������� � ����
	void ManualPuskStop() {
		setModeAlarm(ALARM_CODE_ABORT);
	}
	// ������.������ � ����� ����
	void ManualPuskCycl() {
		setModeAlarm(ALARM_CODE_SWITCH);
	}
	// ===================================================
	// ��������� ��������, ��������� ������ ���������, ����� �� ������ ����� ������� - ������������� � ����
	void ManualShelfSet() {
		mode = WMODE_M_SHELF;
		// ��������� �������
		ENGINE_ON;
		// ����� ������ � ���������
		//core->sendToPc_Mode(SEND2PC_MSHELF);
		core->sendToPc_Mode_sub(SEND2PC_MSHELF, thresholdSpeedStockTimeForwardStop_set + VG::adcCore_speedLenghtAve_ram + VG::adcCore_speedLenghtDlt_ram);
		// ����� ���������� ����� �� ����� (�����)
//		timeStockShelf_value = timeStockShelf_set;
	}
// 	void ManualShelfNone() {
// 		// �������� ��������� ������� �����
// 		if (timeStockShelf_value > 0) {
// 			timeStockShelf_value--;
// 			return;
// 		}
// 		// ������� � ����� �������� ���
// 		SetMode(WMODE_M_BACK);
// 	}
	// ===================================================
	void CicleBackSet() {
		// ����� ������
		mode = WMODE_C_BACK;
		// ����� ������ � ���������
		core->sendToPc_Mode(SEND2PC_CBACK);
		// ��������� ���������
		ENGINE_OFF;
		// ���� �������� �����
		flagMoveStock = 0;
		// �������� �������� ��� �������� ��������� (�����)
		safeIntervalEngineOff_value = safeIntervalEngineOff_set;
		// ����� ��������� ����������� ������ ���������� �����
		thresholdSpeedStockTimeBackStart_value = thresholdSpeedStockTimeBackStart_set;
	}
	void CicleBackNone() {
		// �������� �������� ��� �������� ���������
		if (--safeIntervalEngineOff_value == 0) {
			// ���� ��������� - ������
			//SetMode(WMODE_M_ALARM);
			setModeAlarm(ALARM_CODE_SAFE_ENGINE_OFF);
			return;
		}
		// ���� �������� �����
		if (flagMoveStock == 0) {
			// �������� ������ �������� �����
			if (speedMoveStock > thresholdSpeedStockBack_start) {
				if (thresholdSpeedStockTimeBackStart_value > 0) thresholdSpeedStockTimeBackStart_value--;
				else {
					flagMoveStock = 1;
					// ����� ��������� ����������� ��������� ���������� �����
					thresholdSpeedStockTimeBackStop_value = thresholdSpeedStockTimeBackStop_set;
				}
			} else {
				thresholdSpeedStockTimeBackStart_value = thresholdSpeedStockTimeBackStart_set;
			}
		} else {
			// �������� ��������� �����
			if (speedMoveStock < thresholdSpeedStockBack_stop) {
				if (thresholdSpeedStockTimeBackStop_value > 0) thresholdSpeedStockTimeBackStop_value--;
				else {
					switch (switchPos) {
						// ������� �������
						case SWITCH_POS_STOP:
//							SetMode(WMODE_RESET);
							SetMode(WMODE_M_STOP);
						break;
						// ����������� �������
						case SWITCH_POS_PUSK:
							setModeAlarm(ALARM_CODE_SWITCH);
						break;
						default:
							// ����� ����� �������
							SetMode(WMODE_C_DELAY);
					}
				}
			} else {
				thresholdSpeedStockTimeBackStop_value = thresholdSpeedStockTimeBackStop_set;
			}
		}
	}
	void CicleBackStop() {
		setModeAlarm(ALARM_CODE_ABORT);
	}
	// ===================================================
	void CicleDelaySet() {
		// ��������� ���������
		ENGINE_OFF;
		// �������� ���������� ���������� ������
		if (n_cycle_value >= n_cycle_max) {
			// ����� ������ - ����� ����
			SetMode(WMODE_M_STOP);
			return;
		}
		// ����� ������
		mode = WMODE_C_DELAY;
		// ����� ������ � ���������
		//core->sendToPc_Mode(SEND2PC_CDELAY);
		core->sendToPc_Mode_sub(SEND2PC_CDELAY, thresholdSpeedStockTimeForwardStop_set + VG::adcCore_speedLenghtAve_ram + VG::adcCore_speedLenghtDlt_ram);
		// ����� ���������� ����� � �������� ��������� (�����)
		timeStockBegin_value = timeStockBegin_set;
	}
	void CicleDelayNone() {
		// �������� ����� ���������� ����� �� ����������
		if (flag_StopFromPc != 0) {
			// � ����� ����
			SetMode(WMODE_M_STOP);
			return;
		}
		// ����� ���������� ����� � �������� ���������
		if (timeStockBegin_value > 0) {
			timeStockBegin_value--;
			return;
		}
		// � ������ �����
		SetMode(WMODE_C_PUSK);
	}
	// ===================================================
	// ������ �����
	void CiclePuskSet()
	{
		// ����� ������
		if (mode == WMODE_M_STOP) {
			// ������ ����
			n_cycle_value = 1;
			// ����� ����� ���������� ������ � ����������
			flag_StopFromPc = 0;
			// �������� ��������� ��������
			LoadVars();
		} else {
			// ����������� �����
			n_cycle_value++;
		}
		// ����� ������
		mode = WMODE_C_PUSK;
		CRITICAL_SECTION { ticker = 0; }
		// ����� ������ � ���������
		core->sendToPc_Mode(SEND2PC_CFORWARD);
		// ��������� ������
		core->sendToPc_ForceZero();
		// ���� �������� �����
		flagMoveStock = 0;
		// �������� �������� ��� ������ ��������� �����
		safeIntervalEngineOn_value = safeIntervalEngineOn_set;
		// ����� ��������� ����������� ������ ���������� �����
		thresholdSpeedStockTimeForwardStart_value = thresholdSpeedStockTimeForwardStart_set;
		// �������� ���������
		ENGINE_ON;
	}
	// ���� ������
	void CiclePuskNone() {
		// �������� �������� ��� ������ ��������� �����
		if (--safeIntervalEngineOn_value == 0) {
			// ����� ������ - ������
			//SetMode(WMODE_M_ALARM);
			setModeAlarm(ALARM_CODE_SAFE_ENGINE_ON);
			return;
		}
		// ���� �������� �����
		if (flagMoveStock == 0) {
			// �������� �������� �����
			if (speedMoveStock > thresholdSpeedStockForward_start) {
				if (thresholdSpeedStockTimeForwardStart_value > 0) thresholdSpeedStockTimeForwardStart_value--;
				else {
					flagMoveStock = 1;
					// ����� ��������� ����������� ��������� ���������� �����
					thresholdSpeedStockTimeForwardStop_value = thresholdSpeedStockTimeForwardStop_set;
				}
			} else {
				thresholdSpeedStockTimeForwardStart_value = thresholdSpeedStockTimeForwardStart_set;
			}
		} else {
			// �������� ��������� �����
			if (speedMoveStock < thresholdSpeedStockForward_stop) {
				if (thresholdSpeedStockTimeForwardStop_value > 0) thresholdSpeedStockTimeForwardStop_value--;
				else {
					 // ����� ������ �����
					SetMode(WMODE_C_SHELF);
				}
			} else {
				thresholdSpeedStockTimeForwardStop_value = thresholdSpeedStockTimeForwardStop_set;
			}
		}
	}
	// ���� �� "����"
	void CiclePuskStop() {
		// ����� �������� �����
		if (n_cycle_value == 1) {
			// ����� ������
			//SetMode(WMODE_M_ALARM);
			setModeAlarm(ALARM_CODE_ABORT);
			return;
		}
		// ����� �����
		SetMode(WMODE_RESET);
	}
	// ����������� �������:
	// ��������� ����� - ���� �� "����"
	void CiclePuskPusk() {
		// ����� ������
		//SetMode(WMODE_M_ALARM);
		setModeAlarm(ALARM_CODE_SWITCH);
	}
	// ===================================================
	// ����� �����
	void CicleShelfSet()
	{
		// ����� ������
		mode = WMODE_C_SHELF;
		// �������� ���������
		ENGINE_ON;
		// ����� ������ � ���������
		//core->sendToPc_Mode(SEND2PC_CSHELF);
		core->sendToPc_Mode_sub(SEND2PC_CSHELF, thresholdSpeedStockTimeForwardStop_set + VG::adcCore_speedLenghtAve_ram + VG::adcCore_speedLenghtDlt_ram);
		// ����� ���������� ����� �� �����
		timeStockShelf_value = timeStockShelf_set;
	}
	void CicleShelfNone() {
		// �������� ��������� ������� �����
		if (timeStockShelf_value > 0) {
			timeStockShelf_value--;
			return;
		}
		// ������� � ����� �������� ���
		SetMode(WMODE_C_BACK);
	}
	void CicleShelfStop() {
		setModeAlarm(ALARM_CODE_ABORT);
	}
	// ===================================================
	uint16_t delayInitMode;
	void InitModeSet()
	{
		// ��������� ���������
		ENGINE_OFF;
		mode = WMODE_I_INIT;
		delayInitMode = 1000;
	}
	void InitModeNone() {
		if (delayInitMode > 0) {
			delayInitMode--;
			return;
		}
		if (switchMode->Status() == SWITCH_POS_STOP) {
			SetMode(WMODE_M_STOP);
			return;
		}
		setModeAlarm(ALARM_CODE_NONE);
	}
	// ===================================================
	void ResetModeSet() {
		// ��������� ���������
		ENGINE_OFF;
		// � ��������� �����
		core->sendToPc_Mode(SEND2PC_RESET);
		if (switchPos == SWITCH_POS_STOP) {
			SetMode(WMODE_M_STOP);
		}
	}
	// ===================================================
	// ����� ������ �� ����������
	uint8_t readPC_mode = READPCMODE_SYNHRO;
	#define readPC_synhroBufLenght 4
	uint8_t readPC_synhroBuf[readPC_synhroBufLenght];
	uint8_t readPC_lenghtBlock;
	#define readPC_blockMax 16
	uint8_t readPC_block[readPC_blockMax];
	uint8_t readPC_lenghtBlockIndx;
	void readFromPC() {
		uint8_t byte;
		while(ns_rs232::ReadByte(&byte) == 1) {
			readFromPC_fillSynhroBuf(byte);
			if (readPC_mode == READPCMODE_SYNHRO) {
				readPC_CheckedSynhro();
				continue;
			}
			if (readPC_mode == READPCMODE_LENGHT) {
				if (byte > readPC_blockMax) {
					readPC_mode = READPCMODE_SYNHRO;
					readPC_CheckedSynhro();
					continue;
				}
				readPC_lenghtBlock = byte;
				readPC_lenghtBlockIndx = 0;
				readPC_mode = READPCMODE_BODY;
				continue;
			}
			if (readPC_mode == READPCMODE_BODY) {
				readPC_block[readPC_lenghtBlockIndx] = byte;
				readPC_lenghtBlockIndx++;
				if (readPC_lenghtBlockIndx < readPC_lenghtBlock) continue;
				// �������� �.�.
				if (readPC_block[readPC_lenghtBlock - 1] != crc8_buf(readPC_block, readPC_lenghtBlock - 1)) {
					// ������ ��������
					readPC_mode = READPCMODE_SYNHRO;
					readPC_CheckedSynhro();
					continue;
				}
				readPC_mode = READPCMODE_SYNHRO;
				if (readPC_block[0] == _FromPcCommand_Stop) {
					// ���� �� ����������
					flag_StopFromPc = 1;
					continue;
				}
				if (readPC_block[0] == _FromPcCommand_CycleMax) {
					// ����� ������ � �������������� ������
					n_cycle_max = readPC_block[1];
					eeprom_update_block(&n_cycle_max, &VG::n_cycle_max, sizeof(uint8_t));
					continue;
				}
				if (readPC_block[0] == _FromPcCommand_Calibration) {
					// ����� ����������
					calibrovkaBackTimeFlag_mode = calibrovkaBackTimeFlag_SET;
				}
			}
		}
	}
	void readFromPC_fillSynhroBuf(uint8_t byte) {
		for (uint8_t i = 0; i < readPC_synhroBufLenght - 1; i++) {
			readPC_synhroBuf[i] = readPC_synhroBuf[i + 1];
		}
		readPC_synhroBuf[readPC_synhroBufLenght - 1] = byte;
	}
	void readPC_CheckedSynhro() {
		if (readPC_synhroBuf[0] != 0xe6) return;
		if (readPC_synhroBuf[1] != 0x19) return;
		if (readPC_synhroBuf[2] != 0x55) return;
		if (readPC_synhroBuf[3] != 0xaa) return;
		readPC_mode = READPCMODE_LENGHT;
	}
	// ===================================================
	// ��������� ������ ����������
	void calibrovka_checkSetFlags() {
		CRITICAL_SECTION {
			if (calibrovkaBackTimeFlag_mode > 0) {
				flag_calibrovka = 1;
			} else {
				flag_calibrovka = 0;
				calibrovkaBackTimeSendToPc = calibrovkaBackTimeSendToPc_SET;
			}
		}
	}
	// ===================================================
	const TypeFnMode MassFnMn[][5] PROGMEM =
	{//					stop				pusk				cycl				none			     setmode
		{				Dupm,				Dupm,				Dupm,	 ManualAlarmNone,		  ManualAlarmSet	},	// 0  ������ �����: �����
		{				Dupm,				Dupm,				Dupm,	  ManualBackNone,		   ManualBackSet	},	// 1  ������ �����: �����
		{				Dupm,	   ManualPuskSet,		CiclePuskSet,	  ManualStopNone,		   ManualStopSet	},	// 2  ������ �����: ����
		{	  ManualPuskStop,				Dupm,	  ManualPuskCycl,	  ManualPuskNone,		   ManualPuskSet	},	// 3  ������ �����: ����
		{	   ManualBackSet,				Dupm,				Dupm,				Dupm,		  ManualShelfSet	},	// 4  ������ �����: �����
		{				Dupm,				Dupm,				Dupm,				Dupm,					Dupm	},	// 5  ��������� �����: ����� - �����
		{	   CicleBackStop,				Dupm,				Dupm,	   CicleBackNone,			CicleBackSet	},	// 6  ��������� �����: �����
		{	   ManualStopSet,	  ManualAlarmSet,				Dupm,	  CicleDelayNone,		   CicleDelaySet	},	// 7  ��������� �����: ��������
		{	   CiclePuskStop,	   CiclePuskPusk,				Dupm,	   CiclePuskNone,			CiclePuskSet	},	// 8  ��������� �����: ����
		{	  CicleShelfStop,				Dupm,				Dupm,	  CicleShelfNone,		   CicleShelfSet	},	// 9  ��������� �����: �����
		{	   ManualStopSet,				Dupm,				Dupm,		InitModeNone,			 InitModeSet	},	// 10 ��������� ����� ����� ������
		{	   ManualStopSet,				Dupm,				Dupm,				Dupm,			ResetModeSet	},	// 11 RESET
		{				Dupm,				Dupm,				Dupm,				Dupm,					Dupm	}	// 12
	};
	const char Message_ManualAlarm[]	PROGMEM = "Manual Alarm";
	const char Message_ManualBack[]		PROGMEM = "Manual Back";
	const char Message_Stop[]			PROGMEM = "Stop";
	const char Message_ManualForward[]	PROGMEM = "Manual Forward";
	const char Message_ManualShelf[]	PROGMEM = "Manual Shelf";
	const char Message_CicleAlarm[]		PROGMEM = "Cicle Alarm";
	const char Message_CicleBack[]		PROGMEM = "Cicle Back";
	const char Message_CicleDelay[]		PROGMEM = "Cicle Delay";
	const char Message_CicleForward[]	PROGMEM = "Cicle Forward";
	const char Message_CicleShelf[]		PROGMEM = "Cicle Shelf";
	const char *Message_Status[] =
	{
		Message_ManualAlarm,
		Message_ManualBack,
		Message_Stop,
		Message_ManualForward,
		Message_ManualShelf,
		Message_CicleAlarm,
		Message_CicleBack,
		Message_CicleDelay,
		Message_CicleForward,
		Message_CicleShelf
	};
}
