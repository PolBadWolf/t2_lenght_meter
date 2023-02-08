/* 
* DistanceSpeed.cpp
*
* Created: 07.12.2020 17:26:34
* Author: User
*/


#include "DistanceSpeed.h"

// default constructor
DistanceSpeed::DistanceSpeed(uint8_t lenght) {
	this->lenght = lenght;
	massive = new uint16_t[lenght];
	index = 0;
} //DistanceSpeed

// default destructor
DistanceSpeed::~DistanceSpeed() {
} //~DistanceSpeed

void DistanceSpeed::update(uint16_t measurement) {
	CRITICAL_SECTION {
		measurementOld = massive[index];
		measurementCur = measurement;
		massive[index] = measurement;
		index++;
		if (index >= lenght) index = 0;
	}
}

int16_t DistanceSpeed::getSpeed() {
	int16_t tmp;
	CRITICAL_SECTION {
		tmp = measurementCur - measurementOld;
	}
	return tmp;
}

uint16_t  DistanceSpeed::getSpeedAbs() {
	uint16_t tmp;
	CRITICAL_SECTION {
		if (measurementCur >= measurementOld) tmp = measurementCur - measurementOld;
		else tmp = measurementOld - measurementCur;
	}
	return tmp;
}