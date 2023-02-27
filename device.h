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
#include "LcdKey_Timer.h"
#endif // CONF_TIMER_LCDKEY

//#include "timer2.h"

#ifdef CONF_LCD
#include "Lcd.h"
#endif // CONF_LCD

#ifdef CONF_KEY4
#include "key4.h"
#endif // CONF_KEY4

// 
// #if defined(CONF_MENU)
// #include "menu.h"
// #endif

// #include "rs232.h"
// #include "crc.h"
// #include "watchdog.h"
// #include "vg.h"
// users
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

	void Init();
	void Timer_lcd();
	void Timer_Usr();
	void MainCicle();
}



#endif /* DEVICE_H_ */

