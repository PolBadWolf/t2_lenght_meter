/* 
* DeB.cpp
*
* Created: 07.03.2023 14:16:29
* Author: Gumerovmn
*/


#include "DeB.h"
#include "device.h"

// default constructor
DeB::DeB()
{
} //DeB

// default destructor
DeB::~DeB()
{
} //~DeB

void DeB::main()
{
	ns_device::rs232->digitZ(3, - 123);
}