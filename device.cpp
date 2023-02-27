/*
 * device.cpp
 *
 * Created: 22.09.2017 14:36:35
 *  Author: GumerovMN
 */ 

#include "config.h"
//#include "path.h"
#include "device.h"


#ifdef CONF_LCD
#include "Lcd_hard.h"
#endif // CONF_LCD

//uint16_t acp_div = 0;

char xxx = 0;
int xxx_c = 2;

namespace ns_device
{
	FILE		*strmScr = nullptr;
//	FILE		*rs232StdOut;
	// users
//	SwitchMode	*switchMode;
//	Core		*core;
//	AdcCore		*adcCore; // = new AdcCore();
	// -
//	uint16_t preDiv = PREDIV_ARCHIVE;
#ifdef CONF_LCD
 	Lcd_hard *scr_vn = nullptr;
 	Lcd	*scr = nullptr;
#endif // CONF_LCD
#ifdef CONF_KEY4
	tc_key4		*key;
#endif // CONF_KEY4

	void Init()
	{
#ifdef CONF_LCD
		scr_vn = new Lcd_hard();
		scr = (Lcd *)scr_vn;
#ifdef CONF_LCD_24
		strmScr = scr_vn->Init(scr_vn, 24);
#else
		strmScr = scr_vn->Init(scr_vn, 16);
#endif // CONF_LCD_24
#endif // CONF_LCD

#ifdef CONF_KEY4
 		key = new tc_key4;
#endif

#ifdef CONF_TIMER_LCDKEY
 		ns_LcdKeyTimer::Init();
#endif // CONF_TIMER_LCDKEY
// 		ns_timerMain::Init();
// 		strmScr = scr::Init(16);
		
// 		rs232StdOut = ns_rs232::Init(baud57600, DISABLE, bit8, size128, size256);
// 		ns_rs232::String_P(PSTR(" Start programm"));
		// ====== users ======
		// ������������� ������
// 		switchMode = new SwitchMode(
// 			&DDRC, &PORTC, &PINC, 0,
// 			&DDRC, &PORTC, &PINC, 1,
// 			// ����� �����������
// 			eeprom_read_word(&VG::switch1_delay) , eeprom_read_word(&VG::switch2_delay),
// 			(TypeFunct1 *)&Sw_Event
//		);
		// ======================
		// ���
// 		eeprom_read_block(&VG::adcCore_speedLenghtAve_ram, &VG::adcCore_speedLenghtAve, sizeof(uint16_t));
// 		eeprom_read_block(&VG::adcCore_speedLenghtDlt_ram, &VG::adcCore_speedLenghtDlt, sizeof(uint8_t));
// 		adcCore = new AdcCore(
// 			// ����� ������� ���������� ��� ���������� ��������
// 			VG::adcCore_speedLenghtAve_ram,
// 			// ����� ������� ������ ��� ���������� ��������
// 			VG::adcCore_speedLenghtDlt_ram,
// 			// ����� ������� ���������� ������ ��� ��������� ���������
// 			eeprom_read_byte(&VG::adcCore_distanceLenghtAve),
// 			// ����� ������� ���������� ������ ��� ��������� �������
// 			eeprom_read_byte(&VG::adcCore_forceLenghtAve)
// 		);
		// ����� ����
//		core = new Core();
		// ����� ������
//		ns_ModeWorks::Init();
		// =====================================================
		// ���� ������������ ���������, ����� ��� ������� �������

// #if defined(CONF_MENU)
// 		ns_menu::Init();
// #endif
		// ���
//		ns_watchdog::Init(WDTO_2S);
	}
// ========================================	
	void Timer_lcd()
	{	// �� ���������� ����������
#if defined(CONF_LCD)
		scr->Interrupt();
#endif
#if defined(CONF_KEY4)
		key->Interrupt();
#endif
// #if defined(CONF_MENU)
// 		ns_menu::Interrupt();
// #endif
		// ����� ������������� ������ ������ (flNewSw = 1 - ����� ������ ; switchPos = switchMode->Status();)
//		switchMode->Interrupt();
//		adcCore->start();
//		ns_ModeWorks::Interrupt();
	}
// =======================================
	void Timer_Usr()
	{
// 		if (--preDiv == 0)
// 		{
// 			preDiv = PREDIV_ARCHIVE;
// 			// ���� ������ � ���, �������� �� ���������
// 			if (core) core->Interrupt();
// 		}
		// ���� ���������� (�����) �� ���������
//		ns_ModeWorks::InterruptTick();
	}
	void MainCicle()
	{
#if defined(CONF_MENU)
		ns_menu::Main();
#endif
		// ��������
// 		switchMode->MainCicle();
// 		ns_watchdog::ResetCount();
	}
}
