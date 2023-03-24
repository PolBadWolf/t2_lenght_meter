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
#include "../device.h"
#include "MenuEdit_Uint8T.h"
#include "MenuEdit_Uint16T.h"

#define  key4	ns_device::key
#define  scr	ns_device::scr
#define  steck	ns_device::steckTube

namespace ns_menu
{
	uint8_t		mode = 0;
	uint16_t	timeOut = 0;
	uint8_t		flReboot = 0;
	//
	bool		fistView;
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
	// ------------
	MenuEdit_Uint8T		mEdit_Uint8T;
	MenuEdit_Uint16T	mEdit_Uint16T;
	// ==================================================
	// ******** ������ ������� ���� *********************
	typedef void(*TyFnMn)();
	extern const TyFnMn MassFnMn[][8];
	#define FnMenu(mode, stat) ((TyFnMn)pgm_read_ptr(& MassFnMn[mode][stat]))()
	// ==================================================
	// *********** ������ ���� **************************
#include "menu_mode.h"
	// ==================================================
	uint8_t key;
	// ==================================================
	// ��� �������������� uint8
	uint8_t editUint8T_dat, editUint8T_min, editUint8T_max;
	typedef void(*CallBackEndEdit_u8)(uint8_t dat);
	CallBackEndEdit_u8		endEdit_u8 = nullptr;
	
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

	// ====================== 1 ������� ����� =========================
	void Main_SetMode()
	{
		mode = MODE_MAIN;				// ����� ����
		// ��������� timeout
		CRITICAL_SECTION { timeOut = 0; }
		key4->setAutoRepeatOff();		// ��������� ������� ������ ������
		// ����������� ����� ������
		scr->flicker = false;
		scr->Clear();
		fistView = true;
	}
	// ------------------------------------------------------
	void detectedReboot() {
		if (flReboot != 0) {
			while (true) {
//				printf_P(PSTR("REBOOT "));
				__delay_ms(200);
			}
		}
	}
	
	void Main_View()
	{
// 		detectedReboot();
		// ************ ����� �� ����� **********************
		// ������� �������� � �����
		scr->flicker = false;
		// ---
		if ((ns_device::core->newData & (1 << 0)) || fistView)
		{
			ns_device::core->newData &= ~(1 << 0);
			fistView = false;
			// --
			int8_t steckIndx = steck->normIndxMass(steck->getCountSteckCurrent() - 1);
			StekTubeUnit unit;
			unit = steck->getLenghtTube(steckIndx - 1);
			if (unit.n > 0 && unit.n < 100)
			{
				scr->Digit ( 6, 2, unit.n);
				scr->DigitZ( 9, 5, unit.lenght);
			}
			unit = steck->getLenghtTube(steckIndx);
			if (unit.n > 0 && unit.n < 100)
			{
				scr->Digit (22, 2, unit.n);
				scr->DigitZ(25, 5, unit.lenght);
			}
			switch (ns_device::core->getStat())
			{
				case CORESTAT_BLOCK:
					scr->flicker = true;
					scr->PutChar(scr->SetPosition(15, 0), 'B');
					scr->flicker = false;
					break;
				case CORESTAT_ERRSN:
					scr->flicker = true;
					scr->PutChar(scr->SetPosition(15, 0), 'S');
					scr->flicker = false;
					break;
				case CORESTAT_ERRIZ:
					scr->flicker = true;
					scr->PutChar(scr->SetPosition(15, 0), 'E');
					scr->flicker = false;
					break;
				case CORESTAT_OK:
					scr->flicker = false;
					scr->PutChar(scr->SetPosition(15, 0), ' ');
					break;
			}
		}
		// ************** ������� **************************
		scr->PutChar(16, (ns_sensors::getSensorStat(0) ? 1 : 0) );
		scr->PutChar(17, (ns_sensors::getSensorStat(1) ? 1 : 0) );
		scr->PutChar(18, (ns_sensors::getSensorStat(2) ? 1 : 0) );
		scr->PutChar(19, (( ns_sensors::blockirovka)   ? 1 : 0) );
		// ************** ������� *********************
		scr->DigitZ((uint8_t)0, 5, ns_sensors::v_count);
	}
	void Main_menu()
	{
		FnMenu(MODE_SELECT_SCR, MENU_SETMODE);
	}
	// ====================== 2 ����� ������ � ���. ������� ====================
	#define VAR_SELECT_SCR_N_STECK		0
	#define VAR_SELECT_SCR_NASTROJKA	1
	#define VAR_SELECT_SCR_END			2
	uint8_t var_seclectScr_nScr;
	//MODE_SELECT_SCR
	void seclectScr_view()
	{
		scr->flicker = false;
		scr->Clear();
		switch (var_seclectScr_nScr)
		{
			case VAR_SELECT_SCR_N_STECK:
				scr->String_P(scr->SetPosition(0, 0), PSTR("��������") );
				scr->String_P(scr->SetPosition(0, 1), PSTR("������") );
				return;
				break;
			case VAR_SELECT_SCR_NASTROJKA:
			scr->String_P(scr->SetPosition(0, 0), PSTR("���������") );
			scr->String_P(scr->SetPosition(0, 1), PSTR("����������") );
			return;
			break;
			default:
				scr->flicker = true;
				scr->String_P(&scr->pos, PSTR("������ ���������"));
				scr->flicker = false;
				scr->Digit(&scr->pos, 4, var_seclectScr_nScr);
		}
	}
	// 
	void seclectScr_SetMode()
	{
		mode = MODE_SELECT_SCR;			// ����� ����
		CRITICAL_SECTION { timeOut = TIMEOUT_SECSRC; }
		key4->setAutoRepeatOff();		// ��������� ������� ������ ������
		var_seclectScr_nScr = VAR_SELECT_SCR_N_STECK;
		seclectScr_view();
	}
	// "-"
	void seclectScr_minus()
	{
		CRITICAL_SECTION { timeOut = TIMEOUT_SECSRC; }
		if (var_seclectScr_nScr > VAR_SELECT_SCR_N_STECK)	var_seclectScr_nScr--;
		seclectScr_view();
	}
	// "+"
	void seclectScr_plus()
	{
		CRITICAL_SECTION { timeOut = TIMEOUT_SECSRC; }
		if (var_seclectScr_nScr < (VAR_SELECT_SCR_END - 1))	var_seclectScr_nScr++;
		seclectScr_view();
	}
	// �����
	void seclectScr_ent()
	{
		CRITICAL_SECTION { timeOut = 0; }
		switch(var_seclectScr_nScr)
		{
			case VAR_SELECT_SCR_N_STECK:
				FnMenu(MODE_STECK_TUBE, MENU_SETMODE);
				break;
			case VAR_SELECT_SCR_NASTROJKA:
				FnMenu(MODE_ENTER_PASS, MENU_SETMODE);
// 				FnMenu(MODE_SELECT_PARAM, MENU_SETMODE);	// ����� ����� ������
				break;
		}
	}
	// ***************************** �������� ����� ���� *****************************
	uint8_t var_steckTube_cur;
	void steckTube_vDat()
	{
		StekTubeUnit unit1, unit2;
		unit1 = steck->getLenghtTube(var_steckTube_cur);
		unit2 = steck->getLenghtTube(var_steckTube_cur + 1);
		scr->Clear();
		scr->String_P((uint8_t)0, PSTR("�����."));
		scr->String_P(16, PSTR("������"));
		if (unit1.n != 0)
		{
			scr->Digit( 7, 2, unit1.n);
			scr->Digit(10, 5, unit1.lenght);
		}
		if (unit2.n != 0)
		{
			scr->Digit(23, 2, unit2.n);
			scr->Digit(26, 5, unit2.lenght);
		}
	}
	
	void steckTube_SetMode()
	{
		mode = MODE_STECK_TUBE;			// ����� ����
		// ��������� timeout
		CRITICAL_SECTION { timeOut = 0; }
		key4->setAutoRepeatOff();		// ��������� ������� ������ ������
		var_seclectScr_nScr = MODE_STECK_TUBE;
		var_steckTube_cur = 0;
		steckTube_vDat();
	}

	void steckTube_minus()
	{
		if (var_steckTube_cur > 0)	var_steckTube_cur--;
// 		else		var_steckTube_cur = steck->getCountSteckFull() - 2;
		steckTube_vDat();
	}

	void steckTube_plus()
	{
		if (var_steckTube_cur < (steck->getCountSteckFull() - 2))	var_steckTube_cur++;
// 		else		var_steckTube_cur = 0;
		steckTube_vDat();
	}
	
	void steckTube_view()
	{
		if (ns_device::core->newData & (1 << 1))
		{
			ns_device::core->newData &= !(1 << 1);
			steckTube_vDat();
		}
	}
	// ***************************** ���� ������ *****************************
	void EnterPass_Set()
	{
		mode = MODE_ENTER_PASS;						// ����� ����
		key4->setAutoRepeatOff();					// ��������� ������� ������ ������
		// ����������� ����� ������
		scr->Clear();
		scr->pos = scr->SetPosition(0, 0);
		scr->String_P(PSTR("������� ������:"));
		// ���������� ������� ��� ����� ������
		curPosPass = 0;
		for (uint8_t i = 0; i < 5; i++)
			inPass[i] = 0;
		// timeout
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		EnterPass_View();
	}
	// ����� �� ����� ��������� ����� ������
	void EnterPass_View()
	{
		// ���������� ������� �������� �� ������
		uint8_t pos = scr->SetPosition(0, 1);
		// ������� ��������� ����� ������ �� �����
		for (uint8_t i=0; i<5; i++)
		{
			if (i < curPosPass)
			{
				scr->flicker = false;
				scr->PutChar(&pos, '*');
				continue;
			}
			if (i == curPosPass)
			{
				scr->flicker = true;
				scr->PutChar(&pos, '0' + inPass[i]);
				scr->flicker = false;
				continue;
			}
			scr->flicker = false;
			scr->PutChar(&pos, '_');
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
				scr->Clear();
				scr->String_P(scr->SetPosition(0, 0), PSTR("������ �����"));
				scr->String_P(scr->SetPosition(0, 1), PSTR("������"));
				CRITICAL_SECTION	{timeOut = 5000;}
				//SetMode(TIMEOUT_TO_MAIN);
				FnMenu(MODE_TIMEOUT_TO_MAIN, MENU_SETMODE);
			}
			
		}
	}
	// =======================================================================
	// *************************** ����� ��������� ***************************
	// ���������� ���������
	// ����� �������������� ������ 1-3
	const char selectParametr0 [] PROGMEM = "D : t integr.   ";
	#define MN_TimeIntegrD													0
	// ��������� �� ������� 1 �� ������� 2
	const char selectParametr1 [] PROGMEM = "D1<->D2 distance";
	#define MN_DistanceD0_D1 												1
	// ��������� �� ������� 1 �� ������� 3
	const char selectParametr2 [] PROGMEM = "D1<->D3 distance";
	#define MN_DistanceD0_D2 												2
	// ----------------------------------------------------------------------
	// ����� ����������� �������� ����� ����� �2 � �3
	const char selectParametr3 [] PROGMEM = "Zero timeout    ";
	#define MN_ZeroTimeOut   												3
	// ----------------------------------------------------------------------
	// ����� ����������� �������� ����� ����� �2 � �3
	const char selectParametr4 [] PROGMEM = "Zero sensor inp ";
	#define MN_ZeroSensorInv   												4
	// ----------------------------------------------------------------------
	const char selectParametr5 [] PROGMEM = "Set Password";
	#define MN_SetPassword													5
	// ----------------------------------------------------------------------
	const char selectParametr6 [] PROGMEM = "Show timing     ";
	#define MN_ShowTiming													6
	// ----------------------------------------------------------------------
	const char *selectParamTab[] = {
		selectParametr0,
		selectParametr1,
		selectParametr2,
		selectParametr3,
		selectParametr4,
		selectParametr5,
		selectParametr6
	};
	uint8_t selectParametr_Idx= 0;
	const uint8_t selectParametr_Max = (sizeof(selectParamTab) / sizeof(char *));
	void SelectParam_View() {	// ������� ������
		uint8_t pos = scr->SetPosition(0, 1);
		for (uint8_t i = 0; i < scr->GetStolbcov(); i++) scr->PutChar(&pos, ' ');
		scr->pos = scr->SetPosition(0, 1);
		scr->String_P(selectParamTab[selectParametr_Idx]);
	}
	void SelectParam_Set() {
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		mode = MODE_SELECT_PARAM;
		key4->setAutoRepeatOff();
		scr->Clear();
		scr->pos = scr->SetPosition(0, 0);
		scr->String_P(PSTR("����:"));
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
	// ==============================
	const char editStr_mSec[] PROGMEM = "����";
	const char editStr_mm[]   PROGMEM = "��";
	char *editStr;
	// -----------
	void EndEditU8_IntD(uint8_t dat)
	{
		ns_sensors::s_sensorInt[0] = dat;
		ns_sensors::s_sensorInt[1] = dat;
		ns_sensors::s_sensorInt[2] = dat;
		ns_sensors::ee_save();
		SelectParam_Set();
	}
	// -----------
	void EndEditU16_DistD0D1(uint16_t dat)
	{
		ns_sensors::s_sensorPosition[0] = 0;
		ns_sensors::s_sensorPosition[1] = dat;
		ns_sensors::ee_save();
		SelectParam_Set();
	}
	void EndEditU16_DistD0D2(uint16_t dat)
	{
		ns_sensors::s_sensorPosition[0] = 0;
		ns_sensors::s_sensorPosition[2] = dat;
		ns_sensors::ee_save();
		SelectParam_Set();
	}
	void EndEditU16_ZeroTimeOut(uint16_t dat)
	{
		ns_sensors::s_count_timeOut = dat;
		ns_sensors::ee_save();
		SelectParam_Set();
	}
	// *******************************************
	// -----------
	void SelectParam_Enter() {
		switch (selectParametr_Idx) {
			case MN_TimeIntegrD:
				// ����� �������������� ������ 1
				mEdit_Uint8T.init(
					ns_sensors::s_sensorInt[0],
					1,
					120,
					(char *)selectParamTab[selectParametr_Idx],
					(char *)editStr_mSec,
					EndEditU8_IntD
					);
			break;
			case MN_DistanceD0_D1:
				// ��������� �� ������� 1 �� ������� 2
				mEdit_Uint16T.init(
					ns_sensors::s_sensorPosition[1],
					750,
					1500,
					(char *)selectParamTab[selectParametr_Idx],
					(char *)editStr_mm,
					EndEditU16_DistD0D1
				);
			break;
// -------------------------------------------------------------------------------------------
			case MN_DistanceD0_D2:
				mEdit_Uint16T.init(
				ns_sensors::s_sensorPosition[2],
				10500,
				11500,
				(char *)selectParamTab[selectParametr_Idx],
				(char *)editStr_mm,
				EndEditU16_DistD0D2
				);
			break;
			case MN_ZeroTimeOut:
				mEdit_Uint16T.init(
				ns_sensors::s_count_timeOut,
				3500,
				15000,
				(char *)selectParamTab[selectParametr_Idx],
				(char *)editStr_mSec,
				EndEditU16_ZeroTimeOut
				);
			break;
			case MN_ZeroSensorInv:
				ns_sensors::setZeroSensorInv();
				Main_SetMode();
				break;
			case MN_SetPassword:
 				FnMenu(MODE_SET_PASS, MENU_SETMODE);
			break;
			case MN_ShowTiming:
				FnMenu(MODE_SHOW_TIMING, MENU_SETMODE);
			break;
			default:
			break;
		}
	}
	// ==============================
	void Reboot_Set() {
		mode = MODE_REBOOT;
		scr->Clear();
		scr->pos = scr->SetPosition(4, 0);
	}
	void Reboot_None() {
		while (true) {
			scr->String_P(PSTR("ReBoot "));
			__delay_ms(100);
		}
	}
	// ==============================
	// ��������� ������ ������
	void Set_PassView()
	{
		uint8_t pos = scr->SetPosition(0, 1);
		for (uint8_t i = 0; i < 5;i++)
		{
			if (i < curPosPass)
			{
				scr->flicker = false;
				scr->PutChar(&pos, '*');
				continue;
			}
			if (i == curPosPass)
			{
				scr->flicker = true;
				scr->PutChar(&pos, '0' + inPass[i]);
				scr->flicker = false;
				continue;
			}
			scr->flicker = false;
			scr->PutChar(&pos, '_');
		}
	}
	void SetPass_Set()
	{
 		mode = MODE_SET_PASS;
		scr->Clear();
		scr->String_P(scr->SetPosition(0, 0), PSTR("����� ������:"));
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
			curPosPass++;
			inPass[curPosPass] = 0;
			EnterPass_View();
		}
		else
		{
			for (uint8_t i=0; i<5; i++)
				eeprom_update_byte((uint8_t *)&eePass[i], inPass[i]);
			scr->Clear();
			scr->String_P(scr->SetPosition(0, 0), PSTR("����� ������"));
			scr->String_P(scr->SetPosition(0, 1), PSTR("����������"));
			CRITICAL_SECTION { timeOut = 5000; }
 			FnMenu(MODE_TIMEOUT, MENU_SETMODE);
		}
	}
	// ==============================
	void Timeout_Set() {
 		mode = MODE_TIMEOUT;
		 selectParametr_Idx = MN_TimeIntegrD;
	}
	// ======================================================
	bool showLastTimingFist = false;
	void showLastTiming_view()
	{
		if (showLastTimingFist || (ns_device::core->newData & (1 << 2)) )
		{
			ns_device::core->newData &= ~(1 << 2);
			showLastTimingFist = false;
			scr->DigitZ(scr->SetPosition(0, 0), 5, ns_device::core->lastTiming[0]);
			scr->DigitZ(scr->SetPosition(6, 0), 5, ns_device::core->lastTiming[1]);
			scr->DigitZ(scr->SetPosition(0, 1), 5, ns_device::core->lastTiming[2]);
			scr->DigitZ(scr->SetPosition(6, 1), 1, ns_device::core->lastAlgoritm);
			scr->DigitZ(scr->SetPosition(8, 1), 5, ns_device::core->getCurrentLenghtTube());
		}
	}
	void showLastTiming_set()
	{
 		mode = MODE_SHOW_TIMING;
		selectParametr_Idx = MN_TimeIntegrD;
		showLastTimingFist = true;
		scr->Clear();
		showLastTiming_view();
	}
#include "menu_tab_fn.h"
}

#endif // CONF_MENU
