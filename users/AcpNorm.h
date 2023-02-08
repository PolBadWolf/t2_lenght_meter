/* 
* AcpNorm.h
*
* Created: 25.05.2020 8:37:43
* Author: User
*/
#ifndef __PATH_H_
#include "path.h"
#endif


#ifndef __ACPNORM_H__
#define __ACPNORM_H__


class AcpNorm
{
//variables
public:
	uint8_t		flagSend;
	uint16_t	znach;
protected:
private:
	uint8_t		massUsrLenght;
	uint8_t		massUsrIndex;
	uint16_t	*massUsr;
// 	int16_t		offset;
// 	uint16_t	k_acp;
// 	uint16_t	k_izm;
	// усреднение
	uint32_t	summ;
	uint32_t	summCor;

//functions
public:
	AcpNorm(uint8_t lenghtUsr);
	~AcpNorm();
//	void Init(int16_t offset, uint16_t k_acp, uint16_t k_izm);
	void FromAcp(uint16_t dat);
protected:
private:
	AcpNorm( const AcpNorm &c );
	AcpNorm& operator=( const AcpNorm &c );

}; //AcpNorm

#endif //__ACPNORM_H__
