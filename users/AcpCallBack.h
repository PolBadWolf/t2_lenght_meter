/* 
* AcpCallBack.h
*
* Created: 25.05.2020 9:45:04
* Author: User
*/
#ifndef __PATH_H_
#include "path.h"
#endif


#ifndef __ACPCALLBACK_H__
#define __ACPCALLBACK_H__

extern const TypeFunct2 acpEvent[1];

namespace ns_AcpCallBack
{
	//void FromAcpLenght(uint16_t dat);
	//void FromAcpVes(uint16_t dat);
	void FromAcpCallBack(uint16_t *dat);
}

#endif //__ACPCALLBACK_H__
