/* 
* watchdog.h
*
* Created: 13.02.2017 15:19:12
* Author: gumerovmn
*/
#ifndef __PATH_H_
#include "path.h"
#endif


#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#define 	WDTO_15MS   0
#define 	WDTO_30MS   1
#define 	WDTO_60MS   2
#define 	WDTO_120MS   3
#define 	WDTO_250MS   4
#define 	WDTO_500MS   5
#define 	WDTO_1S   6
#define 	WDTO_2S   7
// #define 	WDTO_4S   8
// #define 	WDTO_8S   9

namespace ns_watchdog
{
    void Init(uint8_t timeCont);
    void DisabledCount();
    void ResetCount();
} //watchdog

#endif //__WATCHDOG_H__
