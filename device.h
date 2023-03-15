/*
 * device.h
 *
 * Created: 22.09.2017 14:36:54
 *  Author: GumerovMN
 */ 

#ifndef __DEVICE_H_
#define __DEVICE_H_

#include "config.h"

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

#ifdef CONF_RS232
#include "system/communication/RS_232.h"
#endif // CONF_RS232

// users
#include "user/Sensors.h"
// ---пользовательское
#include "user/Core.h"
#include "user/StekTube.h"

namespace ns_device
{
#ifdef CONF_LCD
	extern Lcd	*scr;
#endif // CONF_LCD

#ifdef CONF_KEY4
	extern tc_key4		*key;
#endif // CONF_KEY4

#ifdef CONF_RS232
	extern	Serial *rs232;
#endif // CONF_RS232
	
	
	//users
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

