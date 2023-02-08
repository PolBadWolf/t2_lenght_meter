/* 
* Core.cpp
*
* Created: 24.05.2020 11:21:58
* Author: User
*/


#include "Core.h"

// default constructor
Core::Core() {
} //Core

// default destructor
Core::~Core() {
} //~Core

void Core::Interrupt()
{
	distance = ns_device::adcCore->getDistance();
	force = ns_device::adcCore->getForce();
	speed = ns_device::adcCore->getSpeedSub();
	// �������� ������ � ���������
	SendDataToPc();
}

uint16_t Core::getDistance() {
	uint16_t tmp;
	CRITICAL_SECTION {
		tmp = distance;
	}
	return tmp;
}

uint16_t Core::getForce() {
	uint16_t tmp;
	CRITICAL_SECTION {
		tmp = force;
	}
	return tmp;
}

uint16_t Core::getSpeed() {
	uint16_t tmp;
	CRITICAL_SECTION {
		tmp = speed;
	}
	return tmp;
}

// �������� �� ���������
void Core::SendDataToPc() {
	// ����� ����������
	if (ns_ModeWorks::flag_calibrovka != 0 ) {
		
		if (ns_ModeWorks::calibrovkaBackTimeSendToPc == 0) {
			ns_ModeWorks::calibrovkaBackTimeSendToPc = ns_ModeWorks::calibrovkaBackTimeSendToPc_SET;
			Send_CalibrSensors();
		}
		return;
	}
	// ������� ������� � ��������� ������� �������� ���
	switch (ns_ModeWorks::mode)
	{
		case WMODE_M_PUSK:
		case WMODE_M_SHELF:
		case WMODE_M_BACK:
		case WMODE_C_PUSK:
		case WMODE_C_SHELF:
		case WMODE_C_BACK:
			Send_CurentData(SEND2PC_DATA);
			break;
		break;
	}
}

// �������� ������ � ������ ����������
void Core::Send_CalibrSensors() {
	// �������� ������� ����� � ���� � �����
	Send_CurentData(SEND2PC_CALIBR);
	
}

// ==================================================================================================
const uint8_t headMass[] PROGMEM = {0x00, 0xe6, 0x19, 0x55, 0xaa};
void Core::SendHead()
{
	uint8_t i;
	for (i=0; i<sizeof(headMass); i++)
		ns_rs232::SendByte(pgm_read_byte_near(&headMass[i]));
}
// ==================================================================================================
// �������� ���������
void Core::sendToPc_Mode(uint8_t mod)
{
	SendHead();
	ns_rs232::SendByte(6);									// ����� ������� + ���� �.�.
	ns_rs232::CrcInit();									// ������ ������� ����������� �����
	ns_rs232::SendByte(mod);								// ��� ��������
	ns_rs232::SendTetraByte(ns_ModeWorks::ticker);			// ������� ���
	ns_rs232::SendByte( ns_rs232::CrcRead() );				// ����������� �����
}
void Core::sendToPc_Mode_sub(uint8_t mod, uint32_t sub_tik)
{
	SendHead();
	ns_rs232::SendByte(6);									// ����� ������� + ���� �.�.
	ns_rs232::CrcInit();									// ������ ������� ����������� �����
	ns_rs232::SendByte(mod);								// ��� ��������
	ns_rs232::SendTetraByte(ns_ModeWorks::ticker - sub_tik);// ������� ���
	ns_rs232::SendByte( ns_rs232::CrcRead() );				// ����������� �����
}
// �������� alarm � ����� ������
void Core::sendToPc_Alarm(uint8_t code) {
	SendHead();
	ns_rs232::SendByte(1 + 4 + 1 + 1);						// ����� ������� = ����� + ��� + ��� ������ + �.�.
	ns_rs232::CrcInit();									// ������ ������� ����������� �����
	ns_rs232::SendByte((uint8_t)WMODE_M_ALARM);				// ��� ��������
	ns_rs232::SendTetraByte(ns_ModeWorks::ticker);			// ������� ���
	ns_rs232::SendByte(code);								// ��� ������
	ns_rs232::SendByte( ns_rs232::CrcRead() );				// ����������� �����
}
// �������� ������� ������
void Core::Send_CurentData(uint8_t mod)
{

	SendHead();
	ns_rs232::SendByte(1+4+2+2+1);						// ����� �������
	ns_rs232::CrcInit();
	ns_rs232::SendByte(mod);							// ��� ��������
	ns_rs232::SendTetraByte(ns_ModeWorks::ticker);		// ������� ���
	ns_rs232::SendDoubleByte(distance);
	ns_rs232::SendDoubleByte(force);
	ns_rs232::SendByte( ns_rs232::CrcRead() );				// ����������� �����
}
// �������� ����
void Core::sendToPc_ForceZero() {
	SendHead();
	ns_rs232::SendByte(1+4+2+1);
	ns_rs232::CrcInit();
	ns_rs232::SendByte((uint8_t)SEND2PC_FORCE);
	ns_rs232::SendTetraByte(ns_ModeWorks::ticker);			// ������� ���
	ns_rs232::SendDoubleByte(force);
	ns_rs232::SendByte( ns_rs232::CrcRead() );				// ����������� �����
}
