/* 
* Acp.cpp
*
* Created: 23.05.2020 9:11:14
* Author: User
*/


#include "Acp.h"

namespace ns_acp
{
	// public vars
	// private vars
	// всего каналов
	uint8_t		nCh = 1;
	// номера инспользуемых каналов
	uint8_t*	nChs;
	// массив функция обратного вызова
	TypeFunct2*	CallBack;
	// текщий канал
	uint8_t		cChn;
	// ----------------------------------
	// текущее значение
	uint16_t	*measurementAdc;
	uint8_t		measurementAdcReady = 0;
	// ----------------------------------
	// functions
	//		пуск преобразования
	void StartAcpLock(uint8_t channel)
	{
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
		measurementAdcReady = 0;
	}
	void StartAcp()
	{
		cChn = 0;
		StartAcpLock(nChs[cChn]);
	}
	//		инициация
	void Init(uint8_t nCh_, uint8_t* nChs_, TypeFunct2* CallBack_)
	{
		// количество используемых каналов
		nCh = nCh_;
		measurementAdc = new uint16_t[nCh];
		// номера инпользуемых каналов
		nChs = new uint8_t[nCh];
		for (uint8_t i=0; i<nCh; i++)
		{
			nChs[i] = nChs_[i];
		}
		// массив указателей обратного вызова
		CallBack = CallBack_;
		// канал
		cChn = 0;
	}
// 	ISR(ADC_vect)
// 	{
// 		ADCSRA_ADIE = 0;
// 		ADCSRA_ADIF = 0;
// 		uint8_t zn_l = ADCL;
// 		uint8_t zn_h = ADCH;
// 		// текущее значение
// 		measurementAdc[cChn] = zn_l | (zn_h << 8);
// 		if (++cChn < nCh)
// 		{
// 			__delay_us(20);
// 			StartAcpLock(nChs[cChn]);
// 		}
// 		else
// 		{
// 			measurementAdcReady = 1;
// 			sei();
// 			// обратный вызов
// 			if (CallBack != NULL)
// 			{
// 				CallFunct2(CallBack, 0, measurementAdc);
// 			}
// 		}
// 	}
}

