/*
 * device.cpp
 *
 * Created: 22.09.2017 14:36:35
 *  Author: GumerovMN
 */ 

#include "config.h"
//#include "path.h"
#include "device.h"
// #include "communication/rs232.h"

#include "system/communication/RS_232.h"

// ---????????????????
// #include "user/Core.h"
// #include "user/StekTube.h"
// ------------------


#ifdef CONF_LCD
#include "indication/Lcd_hard.h"
#endif // CONF_LCD
#define SCR		ns_device::scr

//uint16_t acp_div = 0;

// char xxx = 0;
// int xxx_c = 2;

namespace ns_device
{
// 	FILE		*strmScr = nullptr;
// 	FILE		*rs232StdOut = nullptr;
	// users
//	
#ifdef CONF_LCD
 	Lcd_hard *scr_vn = nullptr;
 	Lcd	*scr = nullptr;
#endif // CONF_LCD
#ifdef CONF_KEY4
	tc_key4		*key;
#endif // CONF_KEY4

	// Core
	Core	*core;
	StekTube *steckTube;
	
	Serial *rs232;

	void Init()
	{
#ifdef CONF_LCD
		scr_vn = new Lcd_hard();
		scr = (Lcd *)scr_vn;
#ifdef CONF_LCD_24
// 		strmScr = scr_vn->Init(scr_vn, 24);
		scr_vn->Init(scr_vn, 24);
#else
// 		strmScr = scr_vn->Init(scr_vn, 16);
		scr_vn->Init(scr_vn, 16);
#endif // CONF_LCD_24
#endif // CONF_LCD

#ifdef CONF_KEY4
 		key = new tc_key4;
#endif


#ifdef CONF_RS232
 		rs232 = RS_232::init(baud19200, DISABLE, bit8, 8, 32);
		rs232->string_P(PSTR(" Start programm\r\n"));
		
// 		ns_rs232::String_P(PSTR(" Start programm\n"));
#endif // CONF_RS232


#ifdef CONF_TIMER_LCDKEY
 		ns_LcdKeyTimer::Init();
#endif // CONF_TIMER_LCDKEY

#ifdef CONF_TIMER_MAIN
		ns_timerMain::Init();
#endif // CONF_TIMER_MAIN

	
// 		ns_rs232::String_P(PSTR(" Start programm"));
		// ====== users ======
		// ????????????? ??????
// 		switchMode = new SwitchMode(
// 			&DDRC, &PORTC, &PINC, 0,
// 			&DDRC, &PORTC, &PINC, 1,
// 			// ????? ???????????
// 			eeprom_read_word(&VG::switch1_delay) , eeprom_read_word(&VG::switch2_delay),
// 			(TypeFunct1 *)&Sw_Event
//		);
		// ======================
		// ???
// 		eeprom_read_block(&VG::adcCore_speedLenghtAve_ram, &VG::adcCore_speedLenghtAve, sizeof(uint16_t));
// 		eeprom_read_block(&VG::adcCore_speedLenghtDlt_ram, &VG::adcCore_speedLenghtDlt, sizeof(uint8_t));
// 		adcCore = new AdcCore(
// 			// ????? ??????? ?????????? ??? ?????????? ????????
// 			VG::adcCore_speedLenghtAve_ram,
// 			// ????? ??????? ?????? ??? ?????????? ????????
// 			VG::adcCore_speedLenghtDlt_ram,
// 			// ????? ??????? ?????????? ?????? ??? ????????? ?????????
// 			eeprom_read_byte(&VG::adcCore_distanceLenghtAve),
// 			// ????? ??????? ?????????? ?????? ??? ????????? ???????
// 			eeprom_read_byte(&VG::adcCore_forceLenghtAve)
// 		);
		// ????? ????
//		core = new Core();
		// ????? ??????
//		ns_ModeWorks::Init();
		// =====================================================
		// ???? ???????????? ?????????, ????? ??? ??????? ???????

#ifdef CONF_MENU
 		ns_menu::Init();
#endif // CONF_MENU
		// ???
#ifdef CONF_WATHDOG
//		ns_watchdog::Init(WDTO_2S);
#endif // CONF_WATHDOG
		// ????????????????
		ns_sensors::Init();
		// ********************************************** ????? ????? ****************************
		steckTube = new StekTube(15);
// 		{
// 			ns_device::steckTube->newTube( 9300, 1);
// 			ns_device::steckTube->newTube(11825, 2);
// 			ns_device::steckTube->newTube(10786, 3);
// 			ns_device::steckTube->newTube(10252, 4);
// 			ns_device::steckTube->newTube(10988, 5);
// 		}
		core = new Core(StekTube::newTube);
	}
// ========================================	
	void Timer_lcd()
	{	// ?? ?????????? ??????????
#if defined(CONF_LCD)
		scr->Interrupt();
#endif
#if defined(CONF_KEY4)
		key->Interrupt();
#endif
#ifdef CONF_MENU
 		ns_menu::Interrupt();
#endif // CONF_MENU
		// ????? ????????????? ?????? ?????? (flNewSw = 1 - ????? ?????? ; switchPos = switchMode->Status();)
//		switchMode->Interrupt();
//		adcCore->start();
//		ns_ModeWorks::Interrupt();
	}
// =======================================
// 	uint16_t ss_count = 0;
// 	uint8_t  ss_sec = 0;
// 	uint8_t  ss_min = 0;
// 	uint8_t  ss_chs = 0;
	void Timer_Usr()
	{
#ifndef CONF_MENU
// 		bool ss_event = false;
// 		if (++ss_count == 1000)
// 		{
// 			ss_count = 0;
// 			ss_event = true;
// 			if (++ss_sec == 60)
// 			{
// 				ss_sec = 0;
// 				if (++ss_min == 60)
// 				{
// 					ss_min = 0;
// 					if (++ss_chs == 24)
// 					{
// 						ss_chs = 0;
// 					}
// 				}
// 			}
// 		}
// 		if (ss_event)
// 		{
// 			SCR->DigitZ(23, 2, ss_chs);
// 			SCR->DigitZ(26, 2, ss_min);
// 			SCR->DigitZ(29, 2, ss_sec);
// 			SCR->flicker = true;
// 			SCR->PutChar(25, ':');
// 			SCR->PutChar(28, ':');
// 			SCR->flicker = false;
// 		}
#endif // CONF_MENU
#ifdef CONF_MENU
#endif // CONF_MENU
// 		if (--preDiv == 0)
// 		{
// 			preDiv = PREDIV_ARCHIVE;
// 			// ???? ?????? ? ???, ???????? ?? ?????????
// 			if (core) core->Interrupt();
// 		}
		// ???? ?????????? (?????) ?? ?????????
//		ns_ModeWorks::InterruptTick();
		// 
		ns_sensors::interrupt();
	}
// ======================= ******************** ==================
	void MainCicle()
	{
#ifdef CONF_MENU
		ns_menu::Main();
#endif // CONF_MENU
		// ????????
// 		switchMode->MainCicle();
// 		ns_watchdog::ResetCount();
		ns_sensors::mainCicle();
		core->mainCycle();
	}
}
