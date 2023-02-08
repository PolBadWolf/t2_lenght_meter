/* 
* AdcCore.cpp
*
* Created: 07.12.2020 9:49:19
* Author: User
*/


#include "AdcCore.h"

static AdcCore* adcCore;
const AdcCoreCallBack adcCoreCallBack[] PROGMEM = {AdcCore::callBackEvent};
#define	ADC_DISTANCE	0
#define ADC_FORCE		1
	
// default constructor
AdcCore::AdcCore(uint8_t speedLenght_AVE, uint8_t speedLenght_DLT, uint8_t distanceLenghtAve, uint8_t forceLenghtAve) {
	adcCore = this;
	adc = new Adc();
	// ===
	// длина усреднения данных для архивирования
	averagingSlidingDistance = new AveragingSliding(distanceLenghtAve);
	averagingSlidingForce    = new AveragingSliding(forceLenghtAve);
	// длина усреднения данных для определения скорости
	averagingSlidingSpeed    = new AveragingSliding(speedLenght_AVE);
	// длина ма
	distanceSpeed = new DistanceSpeed(speedLenght_DLT);
} //AdcCore

// default destructor
AdcCore::~AdcCore() {
} //~AdcCore

void AdcCore::start() {
	startLocal(ADC_DISTANCE);
}

uint8_t nk = 0;

void AdcCore::result(uint16_t measurement) {
	uint16_t tmpDistance, tmpForce, tmpSpeed;
 	switch (nChannel) {
 		case ADC_DISTANCE:
			distanceSource = measurement;
 			startLocal(ADC_FORCE);
 			break;
 		case ADC_FORCE:
 			forceSource = measurement;
  			tmpDistance = averagingSlidingDistance->averaging(distanceSource);
			tmpForce = averagingSlidingForce->averaging(forceSource);
			tmpSpeed = averagingSlidingSpeed->averaging(distanceSource);
			distanceSpeed->update(tmpSpeed);
			CRITICAL_SECTION {
				distanceAveraging = tmpDistance;
				forceAveraging = tmpForce;
				speedAveraging = tmpSpeed;
				speedSub = distanceSpeed->getSpeedAbs();
			}
			// ==============
			break;
	}
	//
}

void AdcCore::callBackEvent(uint16_t measurement) {
	adcCore->result(measurement);
}

void AdcCore::startLocal(uint8_t nChannel) {
	this->nChannel = nChannel;
	adc->start(nChannel, (AdcCoreCallBack *)&adcCoreCallBack );
}

uint16_t AdcCore::getDistance() {
	uint16_t tmp;
	CRITICAL_SECTION {
		tmp = distanceAveraging;
	}
	return tmp;
}

uint16_t AdcCore::getForce() {
	uint16_t tmp;
	CRITICAL_SECTION {
		tmp = forceAveraging;
	}
	return tmp;
}

uint16_t AdcCore::getDistanceSpeed() {
	uint16_t tmp;
	CRITICAL_SECTION {
		tmp = speedAveraging;
	}
	return tmp;
}

uint16_t AdcCore::getSpeedSub() {
	uint16_t tmp;
	CRITICAL_SECTION {
		tmp = speedSub;
	}
	return tmp;
}