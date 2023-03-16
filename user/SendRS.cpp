/* 
* SendRS.cpp
*
* Created: 13.03.2023 13:09:12
* Author: Gumerovmn
*/


#include "SendRS.h"
#include "../device.h"

#ifdef CONF_RS232
#define rs	ns_device::rs232
#endif // CONF_RS232

// default constructor
SendRS::SendRS()
{
} //SendRS

// default destructor
// SendRS::~SendRS()
// {
// } //~SendRS

void SendRS::SendLenght(int8_t nTubes, uint16_t lenght)
{
#ifdef CONF_RS232
	rs->string_P(PSTR("L"));
	rs->digit(2, nTubes);
	rs->string_P(PSTR("="));
	rs->digit(5, lenght);
	rs->endLine();
#endif // CONF_RS232
}

void SendRS::SendErrorSensor()
{
#ifdef CONF_RS232
	rs->string_P(PSTR("Error sensors\r\n"));
#endif // CONF_RS232
}

void SendRS::SendErrorRender(uint16_t lenght)
{
#ifdef CONF_RS232
	rs->string_P(PSTR("Error render lenght ("));
	rs->digit(5, lenght);
	rs->string_P(PSTR(")\r\n"));
#endif // CONF_RS232
}

void SendRS::SendChangeBlock()
{
#ifdef CONF_RS232
	rs->string_P(PSTR("The change is blocked\r\n"));
#endif // CONF_RS232
}
