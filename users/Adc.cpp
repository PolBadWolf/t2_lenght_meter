/* 
* Adc.cpp
*
* Created: 07.12.2020 7:21:00
* Author: User
*/


#include "Adc.h"

 static Adc* adc;
 
// default constructor
Adc::Adc() {
	adc = this;
	DDRF = 0;
	PORTF = 0;
} //Adc

// default destructor
Adc::~Adc() {
} //~Adc

void Adc::start(uint8_t channel, AdcCoreCallBack* CallBack) {
	this->CallBack = CallBack;
	this->nChannel = nChannel;
	// номер канала
	ADMUX_MUX3 = 0;
	ADMUX_MUX0 = channel & (1 << 0);
	ADMUX_MUX1 = channel & (1 << 1);
	ADMUX_MUX2 = channel & (1 << 2);
	// организация бит
	ADMUX_ADLAR = 0;
	// внешнее питание ацп
	ADMUX_REFS0 = 0;
	ADMUX_REFS1 = 0;
	// делитель
	ADCSRA_ADPS0 = 1;
	ADCSRA_ADPS1 = 1;
	ADCSRA_ADPS2 = 1;
	// прерывание
	ADCSRA_ADIE = 1;
	// ??????
	ADCSRA_ADIF = 0;
	// включить ацп
	ADCSRA_ADEN = 1;
	// включить преобразование
	ADCSRA_ADSC = 1;
}

void Adc::result(uint16_t measurement) {
	//CallFunct3(CallBack, nChannel, result);
	CallMetod1(AdcCoreCallBack, CallBack, measurement);
}

ISR(ADC_vect)
{
	ADCSRA_ADIE = 0;
	ADCSRA_ADIF = 0;
	uint8_t zn_l = ADCL;
	uint8_t zn_h = ADCH;
	// текущее значение
	uint16_t measurement = zn_l | (zn_h << 8);
	adc->result(measurement);
}