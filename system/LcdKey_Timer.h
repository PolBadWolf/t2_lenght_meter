/* 
* LcdKey_Timer.h
*
* Created: 04.02.2017 11:34:09
* Author: User
*/


#ifndef __LCDKEY_TIMER_H__
#define __LCDKEY_TIMER_H__

#ifdef CONF_TIMER_LCDKEY

// частота таймера
#define FT_LcdKeyTimer 1000

namespace ns_LcdKeyTimer
{
	void Init();
}

#endif // CONF_TIMER_LCDKEY

#endif //__LCDKEY_TIMER_H__
