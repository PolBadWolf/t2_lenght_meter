/* 
* SendRS.cpp
*
* Created: 13.03.2023 13:09:12
* Author: Gumerovmn
*/


#include "SendRS.h"
#include "../device.h"

#define rs	ns_device::rs232

// default constructor
SendRS::SendRS()
{
} //SendRS

// default destructor
// SendRS::~SendRS()
// {
// } //~SendRS

void SendRS::SendLenght(uint16_t lenght)
{
	rs->string_P(PSTR("L="));
	rs->digit(5, lenght);
	rs->endLine();
}