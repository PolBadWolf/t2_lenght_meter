/*
 * device.h
 *
 * Created: 22.09.2017 14:36:54
 *  Author: GumerovMN
 */ 

#ifndef __DEVICE_H_
#define __DEVICE_H_

#include "config.h"

//#ifndef __PATH_H_
//#include "path.h"
//#endif

#ifdef CONF_TIMER_LCDKEY
#include "timers/LcdKey_Timer.h"
#endif // CONF_TIMER_LCDKEY

#ifdef CONF_TIMER_MAIN
#include "timers/timer2.h"
#endif // CONF_TIMER_MAIN

#ifdef CONF_LCD
#include "indication/Lcd.h"
#endif // CONF_LCD

#ifdef CONF_KEY4
#include "keyboard/key4.h"
#endif // CONF_KEY4

// 
#ifdef CONF_MENU
#include "menu/menu.h"
#endif // CONF_MENU

#ifdef CONF_WATHDOG
#include "timers/watchdog.h"
#endif // CONF_WATHDOG

// #include "rs232.h"
// #include "crc.h"
// #include "vg.h"

// users
#include "user/Sensors.h"
// ---пользовательское
#include "user/Core.h"
#include "user/StekTube.h"
// ------------------
// #include "users\AveragingSliding.h"
// #include "users\LoadVars.h"
// #include "users\SwitchMode.h"
// #include "users\ModeWorks.h"
// #include "users\Acp.h"
// #include "users\AcpNorm.h"
// #include "users\AcpCallBack.h"
// #include "users\Core.h"
// #include "users\Adc.h"
// #include "users\DistanceSpeed.h"
// #include "users\AdcCore.h"


// 
// #if defined(CONF_LCD)
// #define scr	ns_lcd
// #endif
#define PREDIV_ARCHIVE	5

namespace ns_device
{
#ifdef CONF_LCD
	extern Lcd	*scr;
#endif // CONF_LCD

#ifdef CONF_KEY4
	extern tc_key4		*key;
#endif // CONF_KEY4

	
//	extern FILE			*strmScr;
	//extern FILE			*rs232StdOut;
	//users
//	extern SwitchMode	*switchMode;
// 	extern AcpNorm		*acpNormDistance;
// 	extern AcpNorm		*acpNormVes;
//	extern Core			*core;
	
//	extern AdcCore* adcCore;

// ======================
	// Core
	extern Core	*core;
	extern StekTube *steckTube;
// ======================

	void Init();
	void Timer_lcd();
	void Timer_Usr();
	void MainCicle();
}



#endif /* DEVICE_H_ */

