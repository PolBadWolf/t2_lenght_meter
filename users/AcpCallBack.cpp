/* 
* AcpCallBack.cpp
*
* Created: 25.05.2020 9:45:04
* Author: User
*/


#include "AcpCallBack.h"

// ==============================
// датчики на ацп- обратный вызов
//const TypeFunct2 acpEvent[2] PROGMEM = {ns_AcpCallBack::FromAcpLenght, ns_AcpCallBack::FromAcpVes};
const TypeFunct2 acpEvent[1] PROGMEM = {ns_AcpCallBack::FromAcpCallBack};
// ==============================

namespace ns_AcpCallBack
{
	
	void FromAcpCallBack(uint16_t *dat)
	{
//		ns_device::acpNormDistance->FromAcp(dat[0]);
//		ns_device::acpNormVes->FromAcp(dat[1]);
	}
}
