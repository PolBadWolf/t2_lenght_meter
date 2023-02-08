/* 
* Adc.h
*
* Created: 07.12.2020 7:21:00
* Author: User
*/
#ifndef __PATH_H_
#include "path.h"
#endif


#ifndef __ADC_H__
#define __ADC_H__


class Adc {
//variables
public:
protected:
private:
	AdcCoreCallBack* CallBack;
	uint8_t nChannel;
//functions
public:
	Adc();
	~Adc();
	// ====
	void start(uint8_t nChannel, AdcCoreCallBack* CallBack);
	void result(uint16_t result);
	// ====
protected:
private:
	Adc( const Adc &c );
	Adc& operator=( const Adc &c );

}; //Adc

#endif //__ADC_H__
