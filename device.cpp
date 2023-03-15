/*
 * device.cpp
 *
 * Created: 22.09.2017 14:36:35
 *  Author: GumerovMN
 */ 

#include "config.h"
#include "device.h"

#ifdef CONF_LCD
#include "indication/Lcd_hard.h"
#define SCR		ns_device::scr
#endif // CONF_LCD

namespace ns_device
{
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
	
#ifdef CONF_RS232
	Serial *rs232;
#endif // CONF_RS232

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
#endif // CONF_RS232


#ifdef CONF_TIMER_LCDKEY
 		ns_LcdKeyTimer::Init();
#endif // CONF_TIMER_LCDKEY

#ifdef CONF_TIMER_MAIN
		ns_timerMain::Init();
#endif // CONF_TIMER_MAIN

		// =====================================================
		// меню инициировать последним, когда все объекты созданы

#ifdef CONF_MENU
 		ns_menu::Init();
#endif // CONF_MENU
		// пес
#ifdef CONF_WATHDOG
		ns_watchdog::Init(WDTO_2S);
#endif // CONF_WATHDOG
		// пользовательские
		ns_sensors::Init();
		// ********************************************** Длина СТЕКА ****************************
		steckTube = new StekTube(15);
		core = new Core(StekTube::newTube);
	}
// ========================================	
	void Timer_lcd()
	{	// НЕ РЕГУЛЯРНЫЕ ПРЕРЫВАНИЯ
#if defined(CONF_LCD)
		scr->Interrupt();
#endif
#if defined(CONF_KEY4)
		key->Interrupt();
#endif
#ifdef CONF_MENU
 		ns_menu::Interrupt();
#endif // CONF_MENU
	}
// =======================================
	void Timer_Usr()
	{
		// прерывание для измерение
		ns_sensors::interrupt();
	}
// ======================= ******************** ==================
	void MainCicle()
	{
#ifdef CONF_MENU
		ns_menu::Main();
#endif // CONF_MENU

#ifdef CONF_WATHDOG
		ns_watchdog::ResetCount();
#endif // CONF_WATHDOG

		ns_sensors::mainCicle();
		core->mainCycle();
	}
}
