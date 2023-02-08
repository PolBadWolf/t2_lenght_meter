/* 
* Acp.h
*
* Created: 23.05.2020 9:11:15
* Author: User
*/
#ifndef __PATH_H_
#include "path.h"
#endif

#ifndef __TACP_H__
#define __TACP_H__

namespace ns_acp
{
	// vars
	// текущее значение
	extern uint16_t	*measurementAdc;
	extern uint8_t	measurementAdcReady;
	
	// functions
	void Init(uint8_t nCh, uint8_t* nChs, TypeFunct2* CallBack);
	void StartAcp();
}

#endif //__ACP_H__
