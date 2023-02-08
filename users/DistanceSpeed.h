/* 
* DistanceSpeed.h
*
* Created: 07.12.2020 17:26:34
* Author: User
*/
#ifndef __PATH_H_
#include "path.h"
#endif


#ifndef __DISTANCESPEED_H__
#define __DISTANCESPEED_H__


class DistanceSpeed
{
//variables
public:
protected:
private:
	uint16_t *massive;
	uint8_t lenght;
	uint8_t index;
	// ====
	uint16_t measurementOld;
	uint16_t measurementCur;
//functions
public:
	DistanceSpeed(uint8_t lenght);
	~DistanceSpeed();
	void update(uint16_t measurement);
	int16_t getSpeed();
	uint16_t  getSpeedAbs();
protected:
private:
	DistanceSpeed( const DistanceSpeed &c );
	DistanceSpeed& operator=( const DistanceSpeed &c );

}; //DistanceSpeed

#endif //__DISTANCESPEED_H__
