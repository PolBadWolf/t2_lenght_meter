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
	// ����� �������
	uint8_t		nCh = 1;
	// ������ ������������� �������
	uint8_t*	nChs;
	// ������ ������� ��������� ������
	TypeFunct2*	CallBack;
	// ������ �����
	uint8_t		cChn;
	// ----------------------------------
	// ������� ��������
	uint16_t	*measurementAdc;
	uint8_t		measurementAdcReady = 0;
	// ----------------------------------
	// functions
	//		���� ��������������
	void StartAcpLock(uint8_t channel)
	{
		// ����� ������
		ADMUX_MUX3 = 0;
		ADMUX_MUX0 = channel & (1 << 0);
		ADMUX_MUX1 = channel & (1 << 1);
		ADMUX_MUX2 = channel & (1 << 2);
		// ����������� ���
		ADMUX_ADLAR = 0;
		// ������� ������� ���
		ADMUX_REFS0 = 0;
		ADMUX_REFS1 = 0;
		// ��������
		ADCSRA_ADPS0 = 1;
		ADCSRA_ADPS1 = 1;
		ADCSRA_ADPS2 = 1;
		// ����������
		ADCSRA_ADIE = 1;
		// ??????
		ADCSRA_ADIF = 0;
		// �������� ���
		ADCSRA_ADEN = 1;
		// �������� ��������������
		ADCSRA_ADSC = 1;
		measurementAdcReady = 0;
	}
	void StartAcp()
	{
		cChn = 0;
		StartAcpLock(nChs[cChn]);
	}
	//		���������
	void Init(uint8_t nCh_, uint8_t* nChs_, TypeFunct2* CallBack_)
	{
		// ���������� ������������ �������
		nCh = nCh_;
		measurementAdc = new uint16_t[nCh];
		// ������ ������������ �������
		nChs = new uint8_t[nCh];
		for (uint8_t i=0; i<nCh; i++)
		{
			nChs[i] = nChs_[i];
		}
		// ������ ���������� ��������� ������
		CallBack = CallBack_;
		// �����
		cChn = 0;
	}
// 	ISR(ADC_vect)
// 	{
// 		ADCSRA_ADIE = 0;
// 		ADCSRA_ADIF = 0;
// 		uint8_t zn_l = ADCL;
// 		uint8_t zn_h = ADCH;
// 		// ������� ��������
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
// 			// �������� �����
// 			if (CallBack != NULL)
// 			{
// 				CallFunct2(CallBack, 0, measurementAdc);
// 			}
// 		}
// 	}
}

