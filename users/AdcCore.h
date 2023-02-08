/* 
* AdcCore.h
*
* Created: 07.12.2020 9:49:19
* Author: User
*/
#ifndef __PATH_H_
#include "path.h"
#endif

//typedef void(*AdcCoreCallBack)(uint16_t measurement, uint8_t channel);

#ifndef __ADCCORE_H__
#define __ADCCORE_H__

class AdcCore
{
//variables
public:
protected:

private:
	Adc *adc;
	uint8_t nChannel;
	// ===
	uint16_t distanceSource;
	uint16_t forceSource;
	// ===
	AveragingSliding* averagingSlidingDistance;
	AveragingSliding* averagingSlidingForce;
	AveragingSliding* averagingSlidingSpeed;
	DistanceSpeed* distanceSpeed;
	// ===
	uint16_t distanceAveraging;
	uint16_t forceAveraging;
	uint16_t speedAveraging;
	uint16_t speedSub;
	// ===
//functions
public:
	AdcCore(uint8_t speedLenght_AVE, uint8_t speedLenght_DLT, uint8_t distanceLenghtAve, uint8_t forceLenghtAve);
	~AdcCore();
	// ===
	void start();
	static void callBackEvent(uint16_t measurement);
	uint16_t getDistance();
	uint16_t getForce();
	uint16_t getDistanceSpeed();
	uint16_t getSpeedSub();
	// ===
protected:
private:
	AdcCore( const AdcCore &c );
	AdcCore& operator=( const AdcCore &c );
	// ===
	void result(uint16_t measurement);
	void startLocal(uint8_t nChannel);
	// ===
}; //AdcCore

#endif //__ADCCORE_H__
