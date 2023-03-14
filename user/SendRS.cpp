/* 
* SendRS.cpp
*
* Created: 13.03.2023 13:09:12
* Author: Gumerovmn
*/


#include "SendRS.h"
// #include "../system/communication/rs232.h"

//  #define rs	ns_rs232

const char nextLine[] PROGMEM = { 0x0d, 0x0a };

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
// 	rs::String_P(PSTR("L="));
// 	rs::Digit(5, lenght);
// 	rs::String_P(nextLine);
}