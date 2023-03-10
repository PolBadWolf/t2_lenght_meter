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
	// заглушка
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
	// ==================================================
	// ******** массив функций меню *********************
	typedef void(*TyFnMn)();
	extern const TyFnMn MassFnMn[][8];
	#define FnMenu(mode, stat) ((TyFnMn)pgm_read_ptr(& MassFnMn[mode][stat]))()
	// ==================================================
	// *********** режимы меню **************************
#include "menu_mode.h"
	// ==================================================
	uint8_t key;
	// ==================================================
	// для редактирование uint8
	uint8_t editUint8T_dat, editUint8T_min, editUint8T_max;
	typedef void(*CallBackEndEdit_u8)(uint8_t dat);
	CallBackEndEdit_u8		endEdit_u8 = nullptr;
	
	// для редактирование int8
	uint8_t editS8_dat, editS8_min, editS8_max;
	
	// для редактирование uint16
	uint16_t editU16_dat, editU16_min, editU16_max;
	// для редактирование int16
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
			// для скрытого режима
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
	// локальные прототипы
	void EnterPass_Set();
	void EnterPass_View();
	void EnterPass_Back();
	void EnterPass_Minus();
	void EnterPass_Plus();
	void EnterPass_Enter();
	// ============================================
	// текущяя позитция при вводе пароля
	uint8_t curPosPass;
	// вводимый пароль
	uint8_t inPass[5];
	uint8_t eePass[5] EEMEM         = {0, 0, 0, 0, 0 };
	const uint8_t ffPass[5] PROGMEM = {2, 2, 3, 6, 0 };
	//
	uint8_t fl_cal = 0;
	uint8_t fl_alarm = 0;

	// ====================== 1 Главный экран =========================
	void Main_SetMode()
	{
		mode = MODE_MAIN;				// пункт меню
		// отключить timeout
		CRITICAL_SECTION { timeOut = 0; }
		key4->setAutoRepeatOff();		// отключить быстрый повтор кнопок
		// статическая часть экрана
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
		// ************ длина из стека **********************
		// текущая позитция в стеке
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
				scr->Digit ( 7, 2, unit.n);
				scr->DigitZ(10, 5, unit.lenght);
			}
			unit = steck->getLenghtTube(steckIndx);
			if (unit.n > 0 && unit.n < 100)
			{
				scr->Digit (23, 2, unit.n);
				scr->DigitZ(26, 5, unit.lenght);
			}
		}
		// ************** датчики **************************
		scr->PutChar(16, ((*ns_sensors::sensorMass[0]) ? 1 : 0) );
		scr->PutChar(17, ((*ns_sensors::sensorMass[1]) ? 1 : 0) );
		scr->PutChar(18, ((*ns_sensors::sensorMass[2]) ? 1 : 0) );
		scr->PutChar(19, (( ns_sensors::blockirovka)   ? 1 : 0) );
		// ************** счетчик *********************
		scr->DigitZ((uint8_t)0, 5, ns_sensors::v_count);
	}
	void Main_menu()
	{
		//FnMenu(MODE_ENTER_PASS, MENU_SETMODE);
// 		FnMenu(MODE_SELECT_PARAM, MENU_SETMODE); // обход ввода пароля
		FnMenu(MODE_SELECT_SCR, MENU_SETMODE);
	}
	// ====================== 2 Выбор экрана с доп. данными ====================
	#define VAR_SELECT_SCR_N_STECK		0
	#define VAR_SELECT_SCR_NASTROJKA	1
	#define VAR_SELECT_SCR_END			2
	uint8_t var_seclectScr_nScr;
	//MODE_SELECT_SCR
	void seclectScr_view()
	{
		scr->flicker = false;
		scr->Clear();
		scr->pos = 0;
		switch (var_seclectScr_nScr)
		{
			case VAR_SELECT_SCR_N_STECK:
				scr->String_P(&scr->pos, PSTR("Стек данных") );
				return;
				break;
			case VAR_SELECT_SCR_NASTROJKA:
			scr->String_P(&scr->pos, PSTR("Настройка параметров") );
			return;
			break;
			default:
				//scr->flicker = true;
				scr->String_P(&scr->pos, PSTR("Ошибка параметра"));
				scr->flicker = false;
				scr->Digit(&scr->pos, 4, var_seclectScr_nScr);
		}
	}
	// 
	void seclectScr_SetMode()
	{
		mode = MODE_SELECT_SCR;			// пункт меню
		// отключить timeout
		CRITICAL_SECTION { timeOut = 0; }
		key4->setAutoRepeatOff();		// отключить быстрый повтор кнопок
		var_seclectScr_nScr = VAR_SELECT_SCR_N_STECK;
		seclectScr_view();
	}
	// "-"
	void seclectScr_minus()
	{
		if (var_seclectScr_nScr > VAR_SELECT_SCR_N_STECK)	var_seclectScr_nScr--;
		seclectScr_view();
	}
	// "+"
	void seclectScr_plus()
	{
		if (var_seclectScr_nScr < (VAR_SELECT_SCR_END - 1))	var_seclectScr_nScr++;
		seclectScr_view();
	}
	// выбор
	void seclectScr_ent()
	{
		switch(var_seclectScr_nScr)
		{
			case VAR_SELECT_SCR_N_STECK:
				FnMenu(MODE_STECK_TUBE, MENU_SETMODE);
				break;
			case VAR_SELECT_SCR_NASTROJKA:
// 				FnMenu(MODE_ENTER_PASS, MENU_SETMODE);
				FnMenu(MODE_SELECT_PARAM, MENU_SETMODE);
				break;
		}
	}
	// ***************************** просмотр стека труб *****************************
	uint8_t var_steckTube_cur;
	uint8_t var_steckTube_max;
	void steckTube_vDat()
	{
		StekTubeUnit unit1, unit2;
		unit1 = steck->getLenghtTube(var_steckTube_cur);
		unit2 = steck->getLenghtTube(var_steckTube_cur + 1);
		scr->Clear();
		scr->String_P((uint8_t)0, PSTR("Просм."));
		scr->String_P(16, PSTR("данных"));
		scr->Digit( 7, 2, unit1.n);
		scr->Digit(10, 5, unit1.lenght);
		scr->Digit(23, 2, unit2.n);
		scr->Digit(26, 5, unit2.lenght);
	}
	
	void steckTube_SetMode()
	{
		mode = MODE_STECK_TUBE;			// пункт меню
		// отключить timeout
		CRITICAL_SECTION { timeOut = 0; }
		key4->setAutoRepeatOff();		// отключить быстрый повтор кнопок
		var_seclectScr_nScr = MODE_STECK_TUBE;
// 		var_steckTube_max = steck->getCountSteckMax();
		var_steckTube_max = steck->getCountSteckFull();
		var_steckTube_cur = 0;
		steckTube_vDat();
	}

	void steckTube_minus()
	{
		if (var_steckTube_cur > 0)	var_steckTube_cur--;
		steckTube_vDat();
	}

	void steckTube_plus()
	{
		if (var_steckTube_cur < (var_steckTube_max - 2))	var_steckTube_cur++;
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
	// ***************************** ввод пароля *****************************
	void EnterPass_Set()
	{
		mode = MODE_ENTER_PASS;						// пункт меню
		key4->setAutoRepeatOff();					// отключить быстрый повтор кнопок
		// статическая часть экрана
		scr->Clear();
		scr->pos = scr->SetPosition(0, 0);
		scr->String_P(PSTR("Введите пароль:"));
		// подготовка массива для ввода пароля
		curPosPass = 0;
		for (uint8_t i = 0; i < 5; i++)
			inPass[i] = 0;
		// timeout
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		EnterPass_View();
	}
	// вывод на экран введенной части пароля
	void EnterPass_View()
	{
		// установить текущую позитцию на экране
		uint8_t pos = scr->SetPosition(0, 1);
		// вывести введенную часть пароля на экран
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
	// нажата кнопка меню (назад)
	void EnterPass_Back()
	{
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		if (curPosPass > 0)
		{	// убрать последнюю введеную цифру
			curPosPass--;
			EnterPass_View();
		}
		else
		{	// возврат в главный экран
			FnMenu(MODE_MAIN, MENU_SETMODE);
		}
	}
	// нажата кнопка минус
	void EnterPass_Minus()
	{
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		//key4->autoRepeat = true;
		if (inPass[curPosPass] > 0) inPass[curPosPass]--;
		else						inPass[curPosPass] = 9;
		EnterPass_View();
	}
	// нажата кнопка плюс
	void EnterPass_Plus()
	{
		CRITICAL_SECTION { timeOut = TIMEOUT_ENTER_PASS; }
		//key4->autoRepeat = true;
		if (inPass[curPosPass] < 9) inPass[curPosPass]++;
		else						inPass[curPosPass] = 0;
		EnterPass_View();
	}
	// нажата кнопка ввод
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
		{	// проверка пароля
			bool fl1 = true;
			bool fl2 = true;
			for (uint8_t i=0; i<5; i++)
			{
				if (inPass[i] != (char)eeprom_read_byte((uint8_t *)&eePass[i])) fl1 = false;
				if (inPass[i] != (char)pgm_read_byte((uint8_t *)&ffPass[i])) fl2 = false;
			}
			if (fl1 || fl2)
			{	// пароль верен
				FnMenu(MODE_SELECT_PARAM, MENU_SETMODE);
			}
			else
			{	// пароль не верен
				scr->Clear();
//				printf_P(PSTR("Ошибка ввода"));
				scr->pos = scr->SetPosition(0, 1);
//				printf_P(PSTR("пароля"));
				CRITICAL_SECTION	{timeOut = 5000;}
				//SetMode(TIMEOUT_TO_MAIN);
				FnMenu(MODE_TIMEOUT_TO_MAIN, MENU_SETMODE);
			}
			
		}
	}
	// =======================================================================
	// *************************** выбор параметра ***************************
	// выбираемые параметры
	// время интегрирования датчик 1
	const char selectParametr0 [] PROGMEM = "D1 : t integr.  ";
	#define MN_TimeIntegrD0													0
	// время интегрирования датчик 2
	const char selectParametr1 [] PROGMEM = "D2 : t integr.  ";
	#define MN_TimeIntegrD1													1
	// время интегрирования датчик 3
	const char selectParametr2 [] PROGMEM = "D3 : t integr.  ";
	#define MN_TimeIntegrD2													2
	// дистанция от датчика 1 до датчика 2
	const char selectParametr3 [] PROGMEM = "D1<->D2 distance";
	#define MN_DistanceD0_D1 												3
	// дистанция от датчика 1 до датчика 3
	const char selectParametr4 [] PROGMEM = "D1<->D3 distance";
	#define MN_DistanceD0_D2 												4
	// ----------------------------------------------------------------------
	// время прохождения короткой трубы между Д2 и Д3
	const char selectParametr5 [] PROGMEM = "Zero timeout    ";
	#define MN_ZeroTimeOut   												5
	// ----------------------------------------------------------------------
	const char selectParametr6 [] PROGMEM = "Set Password";
	#define MN_SetPassword													6
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
	void SelectParam_View() {	// очистка строки
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
		scr->String_P(PSTR("Меню:"));
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
	const char editStr_mSec[] PROGMEM = "мСек";
	const char editStr_mm[]   PROGMEM = "мм";
	char *editStr;
	// -----------
	void EndEditU8_IntD0(uint8_t dat)
	{
		ns_sensors::s_sensorInt[0] = dat;
		ns_sensors::ee_save();
	}
	void EndEditU8_IntD1(uint8_t dat)
	{
		ns_sensors::s_sensorInt[1] = dat;
		ns_sensors::ee_save();
	}
	void EndEditU8_IntD2(uint8_t dat)
	{
		ns_sensors::s_sensorInt[2] = dat;
		ns_sensors::ee_save();
	}
	// -----------
	// -----------
	void SelectParam_Enter() {
		switch (selectParametr_Idx) {
			case MN_TimeIntegrD0:
				// время интегрирования датчик 1
// 				editUint8T_dat = ns_sensors::s_sensorInt[0];
// 				editUint8T_min = 1;
// 				editUint8T_max = 120;
// 				endEdit_u8 = EndEditU8_IntD0;
// 				editStr = (char *) editStr_mSec;
// 				FnMenu(MODE_EDIT_UINT8T, MENU_SETMODE);
				mEdit_Uint8T.Init(
					ns_sensors::s_sensorInt[0],
					1,
					120,
					(char *)selectParamTab[selectParametr_Idx],
					(char *)editStr_mSec,
					EndEditU8_IntD0
					);
			break;
			case MN_TimeIntegrD1:
				// время интегрирования датчик 2
				editUint8T_dat = ns_sensors::s_sensorInt[1];
				editUint8T_min = 1;
				editUint8T_max = 120;
				endEdit_u8 = EndEditU8_IntD1;
				FnMenu(MODE_EDIT_UINT8T, MENU_SETMODE);
 			break;
			case MN_TimeIntegrD2:
				// время интегрирования датчик 3
				editUint8T_dat = ns_sensors::s_sensorInt[2];
				editUint8T_min = 1;
				editUint8T_max = 120;
				endEdit_u8 = EndEditU8_IntD2;
				FnMenu(MODE_EDIT_UINT8T, MENU_SETMODE);
			break;
			case MN_DistanceD0_D1:
				// дистанция от датчика 1 до датчика 2
				editU16_dat = ns_sensors::s_sensorPosition[1];
				editU16_min = 750;
				editU16_max = 1400;
				FnMenu(MODE_EDIT_UINT8T, MENU_SETMODE);
			break;
// -------------------------------------------------------------------------------------------
			case MN_DistanceD0_D2:
			case MN_SetPassword:
// 				FnMenu(MODE_SET_PASS, MENU_SETMODE);
			break;
			default:
			break;
		}
	}
	// ==============================
	void EditUmSecU16_View() {
		uint8_t pos = scr->SetPosition(0, 1);
		scr->Digit(&pos, 5, editU16_dat);
	}
	void EditUmSecU16_Set() {
// 		mode = MODE_EDIT_UMSEC_U16;
		key4->setAutoRepeatOff();
		scr->Clear();
		scr->pos = scr->SetPosition(0, 0);
//		printf_P(selectParamTab[selectParametr_Idx]);
		scr->String_P(PSTR(" :"));
		scr->String_P(scr->SetPosition(0, 1), PSTR("     шт./мсек."));
// 		EditUint16T_View();
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
//  		if (selectParametr_Idx == MN_ThresholdSpeedStockForwardStart)	eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockForward_start,	sizeof(int16_t));
//  		if (selectParametr_Idx == MN_ThresholdSpeedStockForwardStop)	eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockForward_stop,		sizeof(int16_t));
//  		if (selectParametr_Idx == MN_ThresholdSpeedStockBackStart)		eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockBack_start,		sizeof(int16_t));
//  		if (selectParametr_Idx == MN_ThresholdSpeedStockBackStop)		eeprom_update_block(&editU16_dat, &VG::thresholdSpeedStockBack_stop,		sizeof(int16_t));
		FnMenu(MODE_SELECT_PARAM, MENU_SETMODE);
	}
	// ==============================
	void EditUnitU8_View() {
		uint8_t pos = scr->SetPosition(0, 1);
		scr->Digit(&pos, 4, editUint8T_dat);
	}
	void EditUnitU8_Set() {
// 		mode = MODE_EDIT_UNIT_U8;
		key4->setAutoRepeatOff();
		scr->Clear();
		scr->pos = scr->SetPosition(0, 0);
//		printf_P(selectParamTab[selectParametr_Idx]);
		scr->String_P(PSTR(" :"));
//		scr->String_P(scr->SetPosition(0, 1), PSTR("    мсек."));
		EditUnitU8_View();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
	}
	void EditUnitU8_Mns() {
		key4->setAutoRepeatOn();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
		if (editUint8T_dat > editUint8T_min)
		{
			editUint8T_dat--;
		}
		else editUint8T_dat = editUint8T_min;
		EditUnitU8_View();
	}
	void EditUnitU8_Pls() {
		key4->setAutoRepeatOn();
		CRITICAL_SECTION { timeOut = TIMEOUT_EDIT; }
		if (editUint8T_dat < editUint8T_max)
		{
			editUint8T_dat++;
		}
		else editUint8T_dat = editUint8T_max;
		EditUnitU8_View();
	}
	void EditUnitU8_Ent() {
		CRITICAL_SECTION { timeOut = 0; }
		key4->setAutoRepeatOff();
//  		if (selectParametr_Idx == MN_nCycleMax)	eeprom_update_block(&editU8_dat, &VG::n_cycle_max, sizeof(uint8_t));
		FnMenu(MODE_SELECT_PARAM, MENU_SETMODE);
	}
	// ==============================
	void Reboot_Set() {
// 		mode = MODE_REBOOT;
		//flReboot = 1;
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
	// установка нового пароля
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
// 		mode = MODE_SET_PASS;
		scr->Clear();
//		printf_P(PSTR("новый пароль:"));
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
			scr->Clear();
//			printf_P(PSTR("Новый пароль"));
			scr->pos = scr->SetPosition(0, 1);
//			printf_P(PSTR("установлен"));
			CRITICAL_SECTION { timeOut = 5000; }
// 			FnMenu(MODE_TIMEOUT, MENU_SETMODE);
		}
	}
	// ==============================
	void Timeout_Set() {
// 		mode = MODE_TIMEOUT;
	}
#include "menu_tab_fn.h"
}

#endif // CONF_MENU
