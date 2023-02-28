/* 
* timer0.h
*
* Created: 19.10.2016 8:06:57
* Author: GumerovMN
*/

#include "config.h"

#ifdef CONF_TIMER_MAIN

#ifndef __TIMER2_H__
#define __TIMER2_H__

// частота таймера ( таймер0 )
#define FtTimerMain 1000

namespace ns_timerMain
{
	void Init();
}

#endif //__TIMER2_H__

#endif // CONF_TIMER_MAIN
