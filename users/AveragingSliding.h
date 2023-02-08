/* 
* AveragingSliding.h
*
* Created: 07.12.2020 13:12:50
* Author: User
*/
#ifndef __PATH_H_
#include "path.h"
#endif


#ifndef __AVERAGINGSLIDING_H__
#define __AVERAGINGSLIDING_H__


class AveragingSliding
{
//variables
public:
protected:
private:
	uint16_t* massive;
	uint32_t sumWork;
	uint32_t sumCorc;
	uint8_t lenght;
	uint8_t currentLenght;
	uint8_t index;

//functions
public:
//	AveragingSliding();
	AveragingSliding(uint8_t lenght);
	~AveragingSliding();
	uint16_t averaging(uint16_t data);
protected:
private:
	AveragingSliding( const AveragingSliding &c );
	AveragingSliding& operator=( const AveragingSliding &c );

}; //AveragingSliding

#endif //__AVERAGINGSLIDING_H__
