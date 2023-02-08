/* 
* AcpNorm.cpp
*
* Created: 25.05.2020 8:37:43
* Author: User
*/


#include "AcpNorm.h"

// default constructor
AcpNorm::AcpNorm(uint8_t lenghtUsr)
{
	flagSend = 0;
	massUsrLenght = lenghtUsr;
	massUsr = new uint16_t[massUsrLenght];
	for (uint8_t i=0; i<massUsrLenght; i++)
	{
		massUsr[i] = 0;
	}
	summ = 0;
	summCor = 0;
	//
// 	offset = 0;
// 	k_acp = 512;
// 	k_izm = 512;
} //AcpNorm

// default destructor
AcpNorm::~AcpNorm()
{
} //~AcpNorm

// void AcpNorm::Init(int16_t offset, uint16_t k_acp, uint16_t k_izm)
// {
// 	this->offset = offset;
// 	this->k_acp = k_acp;
// 	this->k_izm = k_izm;
//}

void AcpNorm::FromAcp(uint16_t dat)
{
	// усреднение
	uint32_t tmp1;
	summ = summ - massUsr[massUsrIndex];
	massUsr[massUsrIndex] = dat;
	if (massUsrIndex == 0)
	{
		summ = summCor;
		summCor = dat;
	}
	else
	{
		summ = summ + dat;
		summCor = summCor + dat;
	}
	if (++massUsrIndex >= massUsrLenght)
	{
		massUsrIndex = 0;
	}
	tmp1 = summ / massUsrLenght;
	//tmp1 = ( (tmp * (uint32_t)k_izm) / ((uint32_t)k_acp) ) + offset;
	CRITICAL_SECTION
	{
		znach = (uint16_t)tmp1;
		flagSend = 1;
	}
}
