/* 
* AveragingSliding.cpp
*
* Created: 07.12.2020 13:12:50
* Author: User
*/


#include "AveragingSliding.h"

// default constructor
// AveragingSliding::AveragingSliding() {
// }
AveragingSliding::AveragingSliding(uint8_t lenght) {
	this->lenght = lenght;
	index = 0;
	currentLenght = 0;
	sumWork = sumCorc = 0;
	massive = new uint16_t[lenght];
	for (uint8_t i = 0; i < lenght; i++) {
		massive[i] = 0;
	}
} //AveragingSliding

// default destructor
AveragingSliding::~AveragingSliding() {
} //~AveragingSliding

uint16_t AveragingSliding::averaging(uint16_t data) {
	sumWork = sumWork - massive[index];
	massive[index] = data;
	sumWork = sumWork + data;
	sumCorc = sumCorc + data;
	index++;
	if (index >= lenght) {
		index = 0;
		sumWork = sumCorc;
		sumCorc = 0;
	}
	if (currentLenght < lenght) currentLenght++;
	return sumWork / currentLenght;
}