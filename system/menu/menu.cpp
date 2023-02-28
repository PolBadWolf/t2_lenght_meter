/* 
* menu.cpp
*
* Created: 17.10.2016 15:41:31
* Author: GumerovMN
*/

#include "menu.h"

#ifdef CONF_MENU

#include <avr/pgmspace.h>
#include <math.h>

#define  key4 ns_device::key

namespace ns_menu
{
	uint8_t		mode = 0;
	uint16_t	timeOut = 0;
	uint8_t		flReboot = 0;
	// ��������
	void Dupm() {}
	#define MENU_VIEW			0		// 0 : no key (view)
	#define MENU_KEY1			1		// 1 : main key1
	#define MENU_KEY2			2		// 2 : main key2
	#define MENU_KEY3			3		// 3 : main key3
	#define MENU_KEY4			4		// 4 : main key4
	#define MENU_MULTIKEY		5		// 5 : multi key
	#define MENU_SETMODE		6		// 6 : set mode
	#define MENU_TIMEOUT		7		// 7 : timeout
	// ==================================================
	// ******** ������ ������� ���� *********************
	typedef void(*TyFnMn)();
	extern const TyFnMn MassFnMn[][8];
	#define FnMenu(mode, stat) ((TyFnMn)pgm_read_ptr(& MassFnMn[mode][stat]))()
	// ==================================================
	// *********** ������ ���� **************************
	#define MODE_MAIN				0				// ������� �����
	#define MODE_ENTER_PASS			1				// ���� ������
	#define MODE_TIMEOUT_TO_MAIN	2				// delay, go main
	#define MODE_TIMEOUT_TO_MENU	3				// delay, go menu
	#define MODE_SELECT_PARAM		4				// ����� ��������� � ����
	#define MODE_EDIT_MSEC_U8		5				// �������������� ���������� � ���� uint8_t
	#define MODE_EDIT_MSEC_U16		6				// �������������� ���������� � ���� uint16_t
	#define MODE_EDIT_UMSEC_U16		7				// �������������� ���������� � unit/mSec  uint16_t
	#define MODE_EDIT_UNIT_U8		8				// �������������� ���������� � ����� uint8_t
	#define MODE_REBOOT				9				// ������������
	#define MODE_SET_PASS			10				// ��������� ������
	#define MODE_TIMEOUT			11
	// ==================================================
	#define TIMEOUT_ENTER_PASS		30000
	#define TIMEOUT_EDIT			30000
	// ==================================================
	uint8_t key;
	// ==================================================
	// ��� �������������� uint8
	uint8_t editU8_dat, editU8_min, editU8_max;
	// ��� �������������� int8
	uint8_t editS8_dat, editS8_min, editS8_max;
	// ��� �������������� uint16
	uint16_t editU16_dat, editU16_min, editU16_max;
	// ��� �������������� int16
	int16_t  editS16_dat, editS16_min, editS16_max;
	// ==================================================
	void TimeoutToMain_Set(uint16_t time)
	{
		CRITICAL_SECTION
		{
			timeOut = time;
			mode = MODE_TIMEOUT_TO_MAIN;
		}
	}
	void TimeoutToMenu_Set(uint16_t time)
	{
		CRITICAL_SECTION
		{
			timeOut = time;
			mode = MODE_TIMEOUT_TO_MENU;
		}
	}
	// ==================================================
	void Init() {
		flReboot = 0;
		if (ns_device::key->InKey() == 1) {
			// ��� �������� ������
		}
		FnMenu(MODE_MAIN, MENU_SETMODE);
	}
	void Main() {
		//scr::DigitZ(scr::SetPosition(0,0), 3, 125);
		uint8_t keyStat = key4->Read(&key);
		uint16_t to;
		CRITICAL_SECTION { to = timeOut; }
		if (to == 1) {
			CRITICAL_SECTION { timeOut = 0; }
			FnMenu(mode, MENU_TIMEOUT);
		}
		else {
			FnMenu(mode, keyStat);
		}
	}
	void Interrupt() {
		CRITICAL_SECTION {
			if (timeOut > 1) timeOut--;
		}
	}
	// ��������� ���������
	void EnterPass_Set();
	void EnterPass_View();
	void EnterPass_Back();
	void EnterPass_Minus();
	void EnterPass_Plus();
	void EnterPass_Enter();
	// ============================================
	// ������� �������� ��� ����� ������
	uint8_t curPosPass;
	// �������� ������
	uint8_t inPass[5];
	uint8_t eePass[5] EEMEM         = {0, 0, 0, 0, 0 };
	const uint8_t ffPass[5] PROGMEM = {2, 2, 3, 6, 0 };
	//
	uint8_t fl_cal = 0;
	uint8_t fl_alarm = 0;
	void Main_SetMode()
	{
		mode = MODE_MAIN;				// ����� ����
		// ��������� timeout
		CRITICAL_SECTION { timeOut = 0; }
		key4->setAutoRepeatOff();		// ��������� ������� ������ ������
		// ����������� ����� ������
		scr::Clear();
		scr::String_P( scr::SetPosition( 0, 0) , PSTR("����.") );
		scr::String_P( scr::SetPosition( 6, 0) , PSTR("���.") );
		scr::String_P( scr::SetPosition( 11, 0) , PSTR("���.") );
		fl_cal = 0;
		fl_alarm = 0;
	}
	uint16_t peekSpeedMass[8];
	uint8_t  peekSpeedIndex = 0;
	uint8_t  peekSpeedCount = 0;
	uint16_t peekSpeed(uint16_t source) {
		if (peekSpeedMass[peekSpeedIndex] < source) peekSpeedMass[peekSpeedIndex] = source;
		uint16_t max = peekSpeedMass[0];
		for (uint8_t i = 1; i < 8; i++) {
			if (max < peekSpeedMass[i]) max = peekSpeedMass[i];
		}
		//--
		if (++peekSpeedCount >= 200) {
			peekSpeedCount = 0;
			if (++peekSpeedIndex >= 8) peekSpeedIndex = 0;
			peekSpeedMass[peekSpeedIndex] = 0;
		}
		// --
		return max;
	}
	void showCalibre() {
		if (ns_ModeWorks::flag_calibrovka != 0) {
			// ����������
			scr::String_P( scr::SetPosition( 0, 0) , PSTR("   ����������   ") );
			fl_cal = 1;
			} else {
			if (fl_cal != 0) {
				fl_cal = 0;
				Main_SetMode();
			}
		}
	}
	void showWorkMode() {
		char dw[3], *dwa = dw;
		switch (ns_ModeWorks::mode) {
			case WMODE_M_STOP:
				dwa = "ST";
				break;
			case WMODE_C_DELAY:
				dwa = "DL";
				break;
			case WMODE_M_PUSK:
			case WMODE_C_PUSK:
				dwa = "FW";
				break;
			case WMODE_M_SHELF:
			case WMODE_C_SHELF:
			dwa = "SH";
			break;
			case WMODE_M_BACK:
			case WMODE_C_BACK:
			dwa = "BC";
			break;
			default:
				dwa = "  ";
		}
		scr::String(scr::SetPosition(14, 1), dwa);
	}
	void showAlarm(uint16_t peek_speed) {
		if (ns_ModeWorks::mode == WMODE_M_ALARM) {
			if (fl_alarm == 0) {
				scr::String_P( scr::SetPosition( 11, 0), PSTR("ALARM") );
				scr::String_P( scr::SetPosition( 10, 1), PSTR("ERR:") );
				scr::DigitZ(   scr::SetPosition( 14, 1), 2, ns_ModeWorks::alarmCode);
				fl_alarm = 1;
			}
			} else {
			if (fl_alarm != 0) {
				scr::String_P( scr::SetPosition( 11, 0) , PSTR("���. ") );
				scr::String_P( scr::SetPosition( 10, 1), PSTR("      ") );
				fl_alarm = 0;
			}
			scr::Digit(    scr::SetPosition(10, 1), 3, peek_speed);
			showWorkMode();
		}
	}
	void detectedReboot() {
		if (flReboot != 0) {
			while (true) {
				printf_P(PSTR("REBOOT "));
				__delay_ms(200);
			}
		}
	}
	
	void Main_View()
	{
		uint16_t peek_speed = peekSpeed(ns_ModeWorks::speedMoveStock);
		showCalibre();
		showAlarm(peek_speed);
 		scr::Digit (scr::SetPosition(0, 1), 4, ns_device::adcCore->getDistance());
 		scr::Digit (scr::SetPosition(5, 1), 4, ns_device::core->getForce());
		detectedReboot();
	}
	void Main_menu()
	{
		//FnMenu(MODE_ENTER_PASS, MENU_SETMODE);
		FnMenu(MODE_SELECT_PARAM, MENU_SETMODE); // ����� ����� ������
	}
	// =======================================================================
	// ***************************** ���� ������ *****************************
	void EnterPass_Set()
	{
		mode = MODE_ENTER_PASS;						// ����� ����
		key4->setAutoRepeatOff();					// ��������� ������� ������ ������
		// ����������� ����� ������
		scr::Clear();
		scr::pos = scr::SetPosition(0, 0);
		printf_P(PSTR("������� ������:"));
		// ���������� ������� ��� ����� ������
		curPosPass = 0;
		for (uint8_t i = 0; i < 5; i++)
			inPass[i] = 0;
		// timeout
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
	}
	// ����� �� ����� ��������� ����� ������
	void EnterPass_View()
	{
		// ���������� ������� �������� �� ������
		uint8_t pos = scr::SetPosition(0, 1);
		// ������� ��������� ����� ������ �� �����
		for (uint8_t i=0; i<5; i++)
		{
			if (i < curPosPass)
			{
				scr::flicker = false;
				scr::PutChar(&pos, '*');
				continue;
			}
			if (i == curPosPass)
			{
				scr::flicker = true;
				scr::PutChar(&pos, '0' + inPass[i]);
				scr::flicker = false;
				continue;
			}
			scr::flicker = false;
			scr::PutChar(&pos, '_');
		}
	}
	// ������ ������ ���� (�����)
	void EnterPass_Back()
	{
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		if (curPosPass > 0)
		{	// ������ ��������� �������� �����
			curPosPass--;
			EnterPass_View();
		}
		else
		{	// ������� � ������� �����
			FnMenu(MODE_MAIN, MENU_SETMODE);
		}
	}
	// ������ ������ �����
	void EnterPass_Minus()
	{
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		//key4->autoRepeat = true;
		if (inPass[curPosPass] > 0) inPass[curPosPass]--;
		else						inPass[curPosPass] = 9;
		EnterPass_View();
	}
	// ������ ������ ����
	void EnterPass_Plus()
	{
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		//key4->autoRepeat = true;
		if (inPass[curPosPass] < 9) inPass[curPosPass]++;
		else						inPass[curPosPass] = 0;
		EnterPass_View();
	}
	// ������ ������ ����
	void EnterPass_Enter()
	{
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		//key4->autoRepeat = false;
		if (curPosPass < 4)
		{
			curPosPass++;
			inPass[curPosPass] = 0; //t;
			EnterPass_View();
		}
		else
		{	// �������� ������
			bool fl1 = true;
			bool fl2 = true;
			for (uint8_t i=0; i<5; i++)
			{
				if (inPass[i] != (char)eeprom_read_byte((uint8_t *)&eePass[i])) fl1 = false;
				if (inPass[i] != (char)pgm_read_byte((uint8_t *)&ffPass[i])) fl2 = false;
			}
			if (fl1 || fl2)
			{	// ������ �����
				FnMenu(MODE_SELECT_PARAM, MENU_SETMODE);
			}
			else
			{	// ������ �� �����
				scr::Clear();
				printf_P(PSTR("������ �����"));
				scr::pos = scr::SetPosition(0, 1);
				printf_P(PSTR("������"));
				CRITICAL_SECTION	{timeOut = 5000;}
				//SetMode(TIMEOUT_TO_MAIN);
				FnMenu(MODE_TIMEOUT_TO_MAIN, MENU_SETMODE);
			}
			
		}
	}
	// =======================================================================
	// *************************** ����� ��������� ***************************
	// ���������� ���������
	// ����� ������� ���������� ��� ���������� ��������
	const char selectParametr0 [] PROGMEM = "Buf.Speed Aver. ";
	#define MN_SpeedLenghtAve												0
	// ����� ������� ������ ��� ���������� ��������
	const char selectParametr1 [] PROGMEM = "Buf.Speed Delta ";
	#define MN_SpeedLenghtDlt												1
	// ����� ������� ���������� ������ ��� ��������� ���������
	const char selectParametr2 [] PROGMEM = "Buf.Dist. Aver. ";
	#define MN_DistanceLenghtAve											2
	// ����� ������� ���������� ������ ��� ��������� �������
	const char selectParametr3 [] PROGMEM = "Buf.Force Aver. ";
	#define MN_ForceLenghtAve												3
	// ----------------------------------------------------------------------
	// �������� �������� ��� ������ ��������� (�����)
	const char selectParametr4 [] PROGMEM = "Safe Engine On  ";
	#define MN_SafeIntervalEngineOn											4
	// �������� �������� ��� �������� ��������� (�����)
	const char selectParametr5 [] PROGMEM = "Safe Engine Off ";
	#define MN_SafeIntervalEngineOff										5
	// ----------------------------------------------------------------------
	// ����� ��������� ����������� ������ ���������� �����
	const char selectParametr6 [] PROGMEM = "Time Forwd.Start";
	#define MN_SpeedStockTimeForwardStart									6
	// ����� ��������� ����������� ��������� ���������� �����
	const char selectParametr7 [] PROGMEM = "Time Forwd.Stop ";
	#define MN_SpeedStockTimeForwardStop									7
	// ����� ��������� ����������� ������ ���������� �����
	const char selectParametr8 [] PROGMEM = "Time Back Start ";
	#define MN_SpeedStockTimeBackStart										8
	// ����� ��������� ����������� ��������� ���������� �����
	const char selectParametr9 [] PROGMEM = "Time Back Stop  ";
	#define MN_SpeedStockTimeBackStop										9
	// ----------------------------------------------------------------------
	// ����� �������� ������ ���������� �����
	const char selectParametr10[] PROGMEM = "Thr.Forwd.Start ";
	#define MN_ThresholdSpeedStockForwardStart								10
	// ����� �������� ��������� ���������� �����
	const char selectParametr11[] PROGMEM = "Thr.Forwd.Stop  ";
	#define MN_ThresholdSpeedStockForwardStop								11
	// ----------------------------------------------------------------------
	// ����� �������� ������ ���������� �����
	const char selectParametr12[] PROGMEM = "Thr.Back Start  ";
	#define MN_ThresholdSpeedStockBackStart									12
	// ����� �������� ��������� ���������� �����
	const char selectParametr13[] PROGMEM = "Thr.Back Stop   ";
	#define MN_ThresholdSpeedStockBackStop									13
	// ----------------------------------------------------------------------
	// ����� ���������� ����� �� �����
	const char selectParametr14[] PROGMEM = "Time Shelf      ";
	#define MN_TimeStockShelf												14
	// ----------------------------------------------------------------------
	// ����� ���������� ����� � �������� ���������
	const char selectParametr15[] PROGMEM = "Time Delay      ";
	#define MN_TimeStockDelay												15
	// ----------------------------------------------------------------------
	// ����������� ����� alarm
	const char selectParametr16[] PROGMEM = "Time Alarm      ";
	#define MN_TimeStockAlarm												16
	// ----------------------------------------------------------------------
	// ������������ ���������� ������ ��������������� ������
	const char selectParametr17[] PROGMEM = "Auto Cycle Max  ";
	#define MN_nCycleMax													17
	// ----------------------------------------------------------------------
	// ����� ������������ ��������� �������������****
	const char selectParametr18[] PROGMEM = "Switch1 Delay   ";
	#define MN_Switch1Delay													18
	// ����� ������������ ��������� �������������****
	const char selectParametr19[] PROGMEM = "Switch2 Delay   ";
	#define MN_Switch2Delay													19
	// ----------------------------------------------------------------------
	const char selectParametr20[] PROGMEM = "Reboot";
	#define MN_Reboot														20
	const char selectParametr21[] PROGMEM = "Set Password";
	#define MN_SetPassword													21
	const char *selectParamTab[] = {
		selectParametr0,
		selectParametr1,
		selectParametr2,
		selectParametr3,
		selectParametr4,
		selectParametr5,
		selectParametr6,
		selectParametr7,
		selectParametr8,
		selectParametr9,
		selectParametr10,
		selectParametr11,
		selectParametr12,
		selectParametr13,
		selectParametr14,
		selectParametr15,
		selectParametr16,
		selectParametr17,
		selectParametr18,
		selectParametr19,
		selectParametr20,
		selectParametr21
	};
	uint8_t selectParametr_Idx= 0;
	const uint8_t selectParametr_Max = (sizeof(selectParamTab) / sizeof(char *));
	void SelectParam_View() {	// ������� ������
		uint8_t pos = scr::SetPosition(0, 1);
		for (uint8_t i = 0; i < scr::Stolbcov(); i++) scr::PutChar(&pos, ' ');
		scr::pos = scr::SetPosition(0, 1);
		printf_P(selectParamTab[selectParametr_Idx]);
	}
	void SelectParam_Set() {
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		mode = MODE_SELECT_PARAM;
		key4->setAutoRepeatOff();
		scr::Clear();
		scr::pos = scr::SetPosition(0, 0);
		printf_P(PSTR("����:"));
		if (selectParametr_Idx >= selectParametr_Max) selectParametr_Idx = 0;
		SelectParam_View();
	}
	void SelectParam_Minus()
	{
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		if (selectParametr_Idx > 0) {
			selectParametr_Idx--;
			SelectParam_View();
		}
	}
	void SelectParam_Plus() {
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		if (selectParametr_Idx < (selectParametr_Max - 1) ) {
			selectParametr_Idx++;
			SelectParam_View();
		}
	}
	void SelectParam_Enter() {
		switch (selectParametr_Idx) {
			case MN_SpeedLenghtAve:
				// ����� ������� ���������� ��� ���������� ��������
				eeprom_read_block(&editU16_dat, &VG::adcCore_speedLenghtAve, sizeof(uint16_t));
				editU16_min = 1;
				editU16_max = 400;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
			break;
			case MN_SpeedLenghtDlt:
				// ����� ������� ������ ��� ���������� ��������
				eeprom_read_block(&editU8_dat, &VG::adcCore_speedLenghtDlt, sizeof(uint8_t));
				editU8_min = 1;
				editU8_max = 250;
				FnMenu(MODE_EDIT_MSEC_U8, MENU_SETMODE);
 			break;
			case MN_DistanceLenghtAve:
				// ����� ������� ���������� ������ ��� ��������� ���������
				eeprom_read_block(&editU8_dat, &VG::adcCore_distanceLenghtAve, sizeof(uint8_t));
				editU8_min = 1;
				editU8_max = 250;
				FnMenu(MODE_EDIT_MSEC_U8, MENU_SETMODE);
			break;
			case MN_ForceLenghtAve:
				// ����� ������� ���������� ������ ��� ��������� �������
				eeprom_read_block(&editU8_dat, &VG::adcCore_forceLenghtAve, sizeof(uint8_t));
				editU8_min = 1;
				editU8_max = 250;
				FnMenu(MODE_EDIT_MSEC_U8, MENU_SETMODE);
			break;
// -------------------------------------------------------------------------------------------
			case MN_SafeIntervalEngineOn:
				// �������� �������� ��� ������ ���������
				eeprom_read_block(&editU16_dat, &VG::safeIntervalEngineOn_set, sizeof(uint16_t));
				editU16_min = 100;
				editU16_max = 10000;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
			break;
			case MN_SafeIntervalEngineOff:
				// �������� �������� �� ������������ ��� ��������� ���������
				eeprom_read_block(&editU16_dat, &VG::safeIntervalEngineOff_set, sizeof(uint16_t));
				editU16_min = 100;
				editU16_max = 10000;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
			break;
// -------------------------------------------------------------------------------------------
			case MN_SpeedStockTimeForwardStart:
				// ����� ��������� ����������� ������ ���������� �����
				eeprom_read_block(&editU16_dat, &VG::thresholdSpeedStockTimeForwardStart_set, sizeof(int16_t));
				editU16_min = 1;
				editU16_max = 500;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
			break;
			case MN_SpeedStockTimeForwardStop:
				// ����� ��������� ����������� ��������� ���������� �����
				eeprom_read_block(&editU16_dat, &VG::thresholdSpeedStockTimeForwardStop_set, sizeof(int16_t));
				editU16_min = 1;
				editU16_max = 500;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
			break;
			case MN_SpeedStockTimeBackStart:
				// ����� ��������� ����������� ������ ���������� �����
				eeprom_read_block(&editU16_dat, &VG::thresholdSpeedStockTimeBackStart_set, sizeof(int16_t));
				editU16_min = 1;
				editU16_max = 500;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
			break;
			case MN_SpeedStockTimeBackStop:
				// ����� ��������� ����������� ��������� ���������� �����
				eeprom_read_block(&editU16_dat, &VG::thresholdSpeedStockTimeBackStop_set, sizeof(int16_t));
				editU16_min = 1;
				editU16_max = 500;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
				break;
// -------------------------------------------------------------------------------------------
			case MN_ThresholdSpeedStockForwardStart:
				// ����� �������� ������ ���������� �����
				eeprom_read_block(&editU16_dat, &VG::thresholdSpeedStockForward_start, sizeof(int16_t));
				editU16_min = 1;
				editU16_max = 100;
				FnMenu(MODE_EDIT_UMSEC_U16, MENU_SETMODE);
				break;
			case MN_ThresholdSpeedStockForwardStop:
				// ����� �������� ��������� ���������� �����
				eeprom_read_block(&editU16_dat, &VG::thresholdSpeedStockForward_stop, sizeof(int16_t));
				editU16_min = 1;
				editU16_max = 100;
				FnMenu(MODE_EDIT_UMSEC_U16, MENU_SETMODE);
				break;
// -------------------------------------------------------------------------------------------
			case MN_ThresholdSpeedStockBackStart:
				// ����� �������� ������ ���������� �����
				eeprom_read_block(&editU16_dat, &VG::thresholdSpeedStockBack_start, sizeof(int16_t));
				editU16_min = 1;
				editU16_max = 100;
				FnMenu(MODE_EDIT_UMSEC_U16, MENU_SETMODE);
				break;
			case MN_ThresholdSpeedStockBackStop:
				// ����� �������� ��������� ���������� �����
				eeprom_read_block(&editU16_dat, &VG::thresholdSpeedStockBack_stop, sizeof(int16_t));
				editU16_min = 1;
				editU16_max = 100;
				FnMenu(MODE_EDIT_UMSEC_U16, MENU_SETMODE);
				break;
// -------------------------------------------------------------------------------------------
			case MN_TimeStockShelf:
				// ����� ���������� ����� �� �����
				eeprom_read_block(&editU16_dat, &VG::timeStockShelf_set, sizeof(int16_t));
				editU16_min = 500;
				editU16_max = 10000;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
				break;
// -------------------------------------------------------------------------------------------
			case MN_TimeStockDelay:
				// ����� ���������� ����� � �������� ���������
				eeprom_read_block(&editU16_dat, &VG::timeStockDelay_set, sizeof(int16_t));
				editU16_min = 500;
				editU16_max = 10000;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
				break;
// -------------------------------------------------------------------------------------------
			case MN_TimeStockAlarm:
				// ����������� ����� alarm
				eeprom_read_block(&editU16_dat, &VG::timeAlarm_set, sizeof(int16_t));
				editU16_min = 1000;
				editU16_max = 10000;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
				break;
// -------------------------------------------------------------------------------------------
			case MN_nCycleMax:
				// ������������ ���������� ������ ��������������� ������
				eeprom_read_block(&editU8_dat, &VG::n_cycle_max, sizeof(int8_t));
				editU8_min = 2;
				editU8_max = 20;
				FnMenu(MODE_EDIT_UNIT_U8, MENU_SETMODE);
				break;
// -------------------------------------------------------------------------------------------
			case MN_Switch1Delay:
				// ����� ������������ ��������� �������������****
				eeprom_read_block(&editU16_dat, &VG::switch1_delay, sizeof(int16_t));
				editU16_min = 10;
				editU16_max = 1000;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
			break;
			case MN_Switch2Delay:
				// ����� ������������ ��������� �������������****
				eeprom_read_block(&editU16_dat, &VG::switch2_delay, sizeof(int16_t));
				editU16_min = 10;
				editU16_max = 1000;
				FnMenu(MODE_EDIT_MSEC_U16, MENU_SETMODE);
			break;
// -------------------------------------------------------------------------------------------
			case MN_Reboot:
				FnMenu(MODE_REBOOT, MENU_SETMODE);
				break;
			case MN_SetPassword:
				FnMenu(MODE_SET_PASS, MENU_SETMODE);
			break;
			default:
			break;
		}
	}
	// ==============================
	// �������������� ���������� � ��������
	void EditMsecU8_View() {
		uint8_t pos = scr::SetPosition(0, 1);
		scr::Digit(&pos, 3, editU8_dat);
	}
	void EditMsecU8_Set() {
		mode = MODE_EDIT_MSEC_U8;
		key4->setAutoRepeatOff();
		scr::Clear();
		scr::pos = scr::SetPosition(0, 0);
		printf_P(selectParamTab[selectParametr_Idx]);
		scr::String_P(PSTR(" :"));
		scr::String_P(scr::SetPosition(0, 1), PSTR("    ����."));
		EditMsecU8_View();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
	}
	void EditMsecU8_Mns() {
		key4->setAutoRepeatOn();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
		if (editU8_dat > editU8_min) {
			editU8_dat--;
		}
		else editU8_dat = editU8_min;
		EditMsecU8_View();
	}
	void EditMsecU8_Pls() {
		key4->setAutoRepeatOn();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
		if (editU8_dat < editU8_max)
		{
			editU8_dat++;
		}
		else editU8_dat = editU8_max;
		EditMsecU8_View();
	}
	void EditMsecU8_Ent() {
		key4->setAutoRepeatOff();
		CRITICAL_SECTION { timeOut = 0; }
 		if (selectParametr_Idx == MN_SpeedLenghtAve)	{ eeprom_update_block(&editU16_dat, &VG::adcCore_speedLenghtAve, sizeof(uint16_t)); VG::adcCore_speedLenghtAve_ram = editU16_dat;	flReboot = 1; }
 		if (selectParametr_Idx == MN_SpeedLenghtDlt)	{ eeprom_update_block(&editU8_dat, &VG::adcCore_speedLenghtDlt, sizeof(uint8_t));	VG::adcCore_speedLenghtDlt_ram = editU16_dat;	flReboot = 1; }
 		if (selectParametr_Idx == MN_DistanceLenghtAve)	{ eeprom_update_block(&editU8_dat, &VG::adcCore_distanceLenghtAve, sizeof(uint8_t));												flReboot = 1; }
 		if (selectParametr_Idx == MN_ForceLenghtAve)	{ eeprom_update_block(&editU8_dat, &VG::adcCore_forceLenghtAve, sizeof(uint8_t));													flReboot = 1; }
		FnMenu(MODE_SELECT_PARAM, MENU_SETMODE);
	}
	// ==============================
	void EditMsecU16_View() {
		uint8_t pos = scr::SetPosition(0, 1);
		scr::Digit(&pos, 5, editU16_dat);
	}
	void EditMsecU16_Set() {
		mode = MODE_EDIT_MSEC_U16;
		key4->setAutoRepeatOff();
		scr::Clear();
		scr::pos = scr::SetPosition(0, 0);
		printf_P(selectParamTab[selectParametr_Idx]);
		scr::String_P(PSTR(" :"));
		scr::String_P(scr::SetPosition(0, 1), PSTR("     ����."));
		EditMsecU16_View();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
	}
	void EditMsecU16_Mns() {
		key4->setAutoRepeatOn();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
		if (editU16_dat > editU16_min) {
			editU16_dat--;
		}
		else editU16_dat = editU16_min;
		EditMsecU16_View();
	}
	void EditMsecU16_Pls() {
		key4->setAutoRepeatOn();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
		if (editU16_dat < editU16_max) {
			editU16_dat++;
		}
		else editU16_dat = editU16_max;
		EditMsecU16_View();
	}
	void EditMsecU16_Ent() {
		CRITICAL_SECTION { timeOut = 0; }
		key4->setAutoRepeatOff();
 		if (selectParametr_Idx == MN_SpeedLenghtAve)			{	eeprom_update_block(&editU16_dat, &VG::adcCore_speedLenghtAve,					sizeof(uint16_t)); VG::adcCore_speedLenghtAve_ram = editU16_dat; flReboot = 1; }
 		if (selectParametr_Idx == MN_SafeIntervalEngineOn)			eeprom_update_block(&editU16_dat, &VG::safeIntervalEngineOn_set,				sizeof(uint16_t));
 		if (selectParametr_Idx == MN_SafeIntervalEngineOff)			eeprom_update_block(&editU16_dat, &VG::safeIntervalEngineOff_set,				sizeof(uint16_t));
 		if (selectParametr_Idx == MN_SpeedStockTimeForwardStart)	eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockTimeForwardStart_set,	sizeof(uint16_t));
 		if (selectParametr_Idx == MN_SpeedStockTimeForwardStop)		eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockTimeForwardStop_set,	sizeof(uint16_t));
 		if (selectParametr_Idx == MN_SpeedStockTimeBackStart)		eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockTimeBackStart_set,	sizeof(uint16_t));
 		if (selectParametr_Idx == MN_SpeedStockTimeBackStop)		eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockTimeBackStop_set,		sizeof(uint16_t));
 		if (selectParametr_Idx == MN_TimeStockShelf)				eeprom_update_block(&editU16_dat, &VG::timeStockShelf_set,						sizeof(uint16_t));
 		if (selectParametr_Idx == MN_TimeStockDelay)				eeprom_update_block(&editU16_dat, &VG::timeStockDelay_set,						sizeof(uint16_t));
 		if (selectParametr_Idx == MN_TimeStockAlarm)				eeprom_update_block(&editU16_dat, &VG::timeAlarm_set,							sizeof(uint16_t));
 		if (selectParametr_Idx == MN_Switch1Delay)				{	eeprom_update_block(&editU16_dat, &VG::switch1_delay,							sizeof(uint16_t)); flReboot = 1; }
 		if (selectParametr_Idx == MN_Switch2Delay)				{	eeprom_update_block(&editU16_dat, &VG::switch2_delay,							sizeof(uint16_t)); flReboot = 1; }
		FnMenu(MODE_SELECT_PARAM, MENU_SETMODE);
	}
	// ==============================
	void EditUmSecU16_View() {
		uint8_t pos = scr::SetPosition(0, 1);
		scr::Digit(&pos, 5, editU16_dat);
	}
	void EditUmSecU16_Set() {
		mode = MODE_EDIT_UMSEC_U16;
		key4->setAutoRepeatOff();
		scr::Clear();
		scr::pos = scr::SetPosition(0, 0);
		printf_P(selectParamTab[selectParametr_Idx]);
		scr::String_P(PSTR(" :"));
		scr::String_P(scr::SetPosition(0, 1), PSTR("     ��./����."));
		EditMsecU16_View();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
	}
	void EditUmSecU16_Mns() {
		key4->setAutoRepeatOn();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
		if (editU16_dat > editU16_min) {
			editU16_dat--;
		}
		else editU16_dat = editU16_min;
		EditUmSecU16_View();
	}
	void EditUmSecU16_Pls() {
		key4->setAutoRepeatOn();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
		if (editU16_dat < editU16_max) {
			editU16_dat++;
		}
		else editU16_dat = editU16_max;
		EditUmSecU16_View();
	}
	void EditUmSecU16_Ent() {
		CRITICAL_SECTION { timeOut = 0; }
		key4->setAutoRepeatOn();
 		if (selectParametr_Idx == MN_ThresholdSpeedStockForwardStart)	eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockForward_start,	sizeof(int16_t));
 		if (selectParametr_Idx == MN_ThresholdSpeedStockForwardStop)	eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockForward_stop,		sizeof(int16_t));
 		if (selectParametr_Idx == MN_ThresholdSpeedStockBackStart)		eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockBack_start,		sizeof(int16_t));
 		if (selectParametr_Idx == MN_ThresholdSpeedStockBackStop)		eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockBack_stop,		sizeof(int16_t));
		FnMenu(MODE_SELECT_PARAM, MENU_SETMODE);
	}
	// ==============================
	void EditUnitU8_View() {
		uint8_t pos = scr::SetPosition(0, 1);
		scr::Digit(&pos, 4, editU8_dat);
	}
	void EditUnitU8_Set() {
		mode = MODE_EDIT_UNIT_U8;
		key4->setAutoRepeatOff();
		scr::Clear();
		scr::pos = scr::SetPosition(0, 0);
		printf_P(selectParamTab[selectParametr_Idx]);
		scr::String_P(PSTR(" :"));
//		scr::String_P(scr::SetPosition(0, 1), PSTR("    ����."));
		EditUnitU8_View();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
	}
	void EditUnitU8_Mns() {
		key4->setAutoRepeatOn();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
		if (editU8_dat > editU8_min)
		{
			editU8_dat--;
		}
		else editU8_dat = editU8_min;
		EditUnitU8_View();
	}
	void EditUnitU8_Pls() {
		key4->setAutoRepeatOn();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
		if (editU8_dat < editU8_max)
		{
			editU8_dat++;
		}
		else editU8_dat = editU8_max;
		EditUnitU8_View();
	}
	void EditUnitU8_Ent() {
		CRITICAL_SECTION { timeOut = 0; }
		key4->setAutoRepeatOff();
 		if (selectParametr_Idx == MN_nCycleMax)	eeprom_update_block(&editU8_dat, &VG::n_cycle_max, sizeof(uint8_t));
		FnMenu(MODE_SELECT_PARAM, MENU_SETMODE);
	}
	// ==============================
	void Reboot_Set() {
		mode = MODE_REBOOT;
		//flReboot = 1;
		scr::Clear();
		scr::pos = scr::SetPosition(4, 0);
	}
	void Reboot_None() {
		while (true) {
			scr::String_P(PSTR("ReBoot "));
			__delay_ms(100);
		}
	}
	// ==============================
	// ��������� ������ ������
	void Set_PassView()
	{
		uint8_t pos = scr::SetPosition(0, 1);
		for (uint8_t i = 0; i < 5;i++)
		{
			if (i < curPosPass)
			{
				scr::flicker = false;
				scr::PutChar(&pos, '*');
				continue;
			}
			if (i == curPosPass)
			{
				scr::flicker = true;
				scr::PutChar(&pos, '0' + inPass[i]);
				scr::flicker = false;
				continue;
			}
			scr::flicker = false;
			scr::PutChar(&pos, '_');
		}
	}
	void SetPass_Set()
	{
		mode = MODE_SET_PASS;
		scr::Clear();
		printf_P(PSTR("����� ������:"));
		for (uint8_t i=0; i<5; i++) inPass[i] = 0;
			curPosPass = 0;
			Set_PassView();
			key4->setAutoRepeatOff();
	}
	void SetPass_Bck()
	{
		key4->setAutoRepeatOff();
		if (curPosPass > 0) {
			curPosPass--;
			Set_PassView();
			return;
		}
		FnMenu(MODE_SELECT_PARAM, MENU_SETMODE);
	}
	void SetPass_Mns()
	{
		key4->setAutoRepeatOn();
		if (inPass[curPosPass]>0)
		{
			inPass[curPosPass]--;
			Set_PassView();
		}
	}
	void SetPass_Pls()
	{
		key4->setAutoRepeatOn();
		if (inPass[curPosPass]<9)
		{
			inPass[curPosPass]++;
			Set_PassView();
		}
	}
	void SetPass_Ent()
	{
		key4->setAutoRepeatOff();
		if (curPosPass < 4)
		{
			//char t = inPass[curPosPass];
			curPosPass++;
			inPass[curPosPass] = 0;
			EnterPass_View();
		}
		else
		{
			for (uint8_t i=0; i<5; i++)
				eeprom_update_byte((uint8_t *)&eePass[i], inPass[i]);
			scr::Clear();
			printf_P(PSTR("����� ������"));
			scr::pos = scr::SetPosition(0, 1);
			printf_P(PSTR("����������"));
			CRITICAL_SECTION { timeOut = 5000; }
			FnMenu(MODE_TIMEOUT, MENU_SETMODE);
		}
	}
	// ==============================
	void Timeout_Set() {
		mode = MODE_TIMEOUT;
	}
	// ==============================
	// ============================================
	// 0 : no key (view)
	// 1-4 : main key
	// 5 : add key
	// 6 : timeout
	const TyFnMn MassFnMn[][8] PROGMEM =
	{//					view				key1				key2				key3				key4				key5			 setmode			 timeout          
		{	       Main_View,		   Main_menu,				Dupm,				Dupm,				Dupm,				Dupm,       Main_SetMode,				Dupm },	//	0 main
		{	  EnterPass_View,	  EnterPass_Back,	 EnterPass_Minus,	  EnterPass_Plus,	 EnterPass_Enter,				Dupm,	   EnterPass_Set,		Main_SetMode }, //	1 enter to menu
		{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,		Main_SetMode }, //	2 delay, go main
		{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,	 SelectParam_Set }, //	3 delay, go menu
		{				Dupm,		Main_SetMode,  SelectParam_Minus,	SelectParam_Plus,  SelectParam_Enter,				Dupm,	 SelectParam_Set,		Main_SetMode }, //	4 ����� ��������� � ����
		{				Dupm,	 SelectParam_Set,	  EditMsecU8_Mns,	  EditMsecU8_Pls,	  EditMsecU8_Ent,				Dupm,	  EditMsecU8_Set,	 SelectParam_Set }, //  5 MODE_EDIT_MSEC_U8
		{				Dupm,	 SelectParam_Set,	 EditMsecU16_Mns,	 EditMsecU16_Pls,	 EditMsecU16_Ent,				Dupm,	 EditMsecU16_Set,	 SelectParam_Set }, //  6 MODE_EDIT_MSEC_U16
		{				Dupm,	 SelectParam_Set,	EditUmSecU16_Mns,	EditUmSecU16_Pls,	EditUmSecU16_Ent,				Dupm,	EditUmSecU16_Set,	 SelectParam_Set }, //  7 MODE_EDIT_UMSEC_U16
		{				Dupm,	 SelectParam_Set,	  EditUnitU8_Mns,	  EditUnitU8_Pls,	  EditUnitU8_Ent,				Dupm,	  EditUnitU8_Set,	 SelectParam_Set }, //  8 MODE_EDIT_UNIT_U8
 		{		 Reboot_None,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,		  Reboot_Set,				Dupm }, //  9 ������������
 		{				Dupm,		 SetPass_Bck,		 SetPass_Mns,		 SetPass_Pls,		 SetPass_Ent,				Dupm,		 SetPass_Set,				Dupm }, // 10 ��������� ������
		{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,		 Timeout_Set,	 SelectParam_Set },
		{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm },
		{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm },
		{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm },
		{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm },
		{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm },
		{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm }
	};

}

#endif // CONF_MENU
