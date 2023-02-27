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

#ifdef CONF_TIMER_MAIN
		ns_timerMain::Init();
#endif // CONF_TIMER_MAIN

// 		strmScr = scr::Init(16);
		
// 		rs232StdOut = ns_rs232::Init(baud57600, DISABLE, bit8, size128, size256);
// 		ns_rs232::String_P(PSTR(" Start programm"));
		// ====== users ======
		// переключатель режима
// 		switchMode = new SwitchMode(
// 			&DDRC, &PORTC, &PINC, 0,
// 			&DDRC, &PORTC, &PINC, 1,
// 			// время переключеня
// 			eeprom_read_word(&VG::switch1_delay) , eeprom_read_word(&VG::switch2_delay),
// 			(TypeFunct1 *)&Sw_Event
//		);
		// ======================
		// АЦП
// 		eeprom_read_block(&VG::adcCore_speedLenghtAve_ram, &VG::adcCore_speedLenghtAve, sizeof(uint16_t));
// 		eeprom_read_block(&VG::adcCore_speedLenghtDlt_ram, &VG::adcCore_speedLenghtDlt, sizeof(uint8_t));
// 		adcCore = new AdcCore(
// 			// длина буффера усреднения для вычисления скорости
// 			VG::adcCore_speedLenghtAve_ram,
// 			// длина буффера дельты для нахождения скорости
// 			VG::adcCore_speedLenghtDlt_ram,
// 			// длина буффера усреднения данных для архивации дистанции
// 			eeprom_read_byte(&VG::adcCore_distanceLenghtAve),
// 			// длина буффера усреднения данных для архивации услилия
// 			eeprom_read_byte(&VG::adcCore_forceLenghtAve)
// 		);
		// общее ядро
//		core = new Core();
		// режим работы
//		ns_ModeWorks::Init();
		// =====================================================
		// меню инициировать последним, когда все объекты созданы

// #if defined(CONF_MENU)
// 		ns_menu::Init();
// #endif
		// пес
//		ns_watchdog::Init(WDTO_2S);
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
// #if defined(CONF_MENU)
// 		ns_menu::Interrupt();
// #endif
		// опрос переключателя режима работы (flNewSw = 1 - новые данные ; switchPos = switchMode->Status();)
//		switchMode->Interrupt();
//		adcCore->start();
//		ns_ModeWorks::Interrupt();
	}
// =======================================
	uint16_t ss_count = 0;
	uint8_t  ss_sec = 0;
	uint8_t  ss_min = 0;
	uint8_t  ss_chs = 0;
	void Timer_Usr()
	{
		bool ss_event = false;
		if (++ss_count == 1000)
		{
			ss_count = 0;
			ss_event = true;
			if (++ss_sec == 60)
			{
				ss_sec = 0;
				if (++ss_min == 60)
				{
					ss_min = 0;
					if (++ss_chs = 24)
					{
						ss_chs = 0;
					}
				}
			}
		}
#define SCR		ns_device::scr
		if (ss_event)
		{
			SCR->DigitZ(23, 2, ss_chs);
			SCR->DigitZ(26, 2, ss_min);
			SCR->DigitZ(29, 2, ss_sec);
			if (ss_count < 500)
			{
				SCR->PutChar(25, ':'); 
				SCR->PutChar(28, ':');
			}
			else
			{
				SCR->PutChar(25, ' ');
				SCR->PutChar(28, ' ');
			}
		}
// 		if (--preDiv == 0)
// 		{
// 			preDiv = PREDIV_ARCHIVE;
// 			// сбор данных с АЦП, передача на компьютер
// 			if (core) core->Interrupt();
// 		}
		// счет прерываний (тиков) за измерение
//		ns_ModeWorks::InterruptTick();
	}
	void MainCicle()
	{
#ifdef CONF_MENU
		ns_menu::Main();
#endif // CONF_MENU
		// заглушка
// 		switchMode->MainCicle();
// 		ns_watchdog::ResetCount();
	}
}
