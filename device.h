/*
 * device.h
 *
 * Created: 22.09.2017 14:36:54
 *  Author: GumerovMN
 */ 

#ifndef __PATH_H_
#include "path.h"
#endif

#include "LcdKey_Timer.h"
#include "timer2.h"
#include "Lcd.h"
#include "key4.h"
#include "menu.h"
#include "rs232.h"
#include "crc.h"
#include "watchdog.h"
#include "vg.h"
// users
#include "users\AveragingSliding.h"
#include "users\LoadVars.h"
#include "users\SwitchMode.h"
#include "users\ModeWorks.h"
// #include "users\Acp.h"
// #include "users\AcpNorm.h"
// #include "users\AcpCallBack.h"
#include "users\Core.h"
#include "users\Adc.h"
#include "users\DistanceSpeed.h"
#include "users\AdcCore.h"

#ifndef DEVICE_H_
#define DEVICE_H_


#define scr	ns_lcd
#define PREDIV_ARCHIVE	5

namespace ns_device
{
	extern FILE			*strmScr;
	extern FILE			*rs232StdOut;
	extern tc_key4		*key;
	//users
	extern SwitchMode	*switchMode;
// 	extern AcpNorm		*acpNormDistance;
// 	extern AcpNorm		*acpNormVes;
	extern Core			*core;
	
	extern AdcCore* adcCore;

	void Init();
	void Timer_lcd();
	void Timer_Usr();
	void MainCicle();
}



#endif /* DEVICE_H_ */