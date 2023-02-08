/* 
* Core.h
*
* Created: 24.05.2020 11:21:59
* Author: User
*/
#ifndef __PATH_H_
#include "path.h"
#endif


#ifndef __CORE_H__
#define __CORE_H__

// ���� ������� �������� �� ���������
// =========================
// *** �������� ������� ������
// ������ �����, ���� ����� � ������ ������
#define	SEND2PC_MALARM			0
// ������ ���� ����� � ������ ������
#define	SEND2PC_MBACK			1
// ������ ��������� ���� (�������� ���������)
#define	SEND2PC_STOP			2
// ������ ���� ������ � ������ ������
#define	SEND2PC_MFORWARD		3
// ������ ���� �� "�����" � ������ ������
#define	SEND2PC_MSHELF			4
// ������ �����, ���� ����� � ����������� ������
#define	SEND2PC_CALARM			5
// ������ ���� ����� � ����������� ������
#define	SEND2PC_CBACK			6
// ������ ����� ����� ������� �����
#define	SEND2PC_CDELAY			7
// ������ ���� ������ � ����������� ������
#define	SEND2PC_CFORWARD		8
// ������ ���� �� "�����" � ����������� ������
#define	SEND2PC_CSHELF			9
//		�������� ������ - ���� ������
#define SEND2PC_DATA			11
//		�������� ������ - ���� �����
#define SEND2PC_FORCE			12
//		����� �������
#define SEND2PC_RESET			13
// =========================
// *** �������� ���������� ������
//		�������� ������ - ���������� � ����� � ������ ������
#define SEND2PC_MDATA			14
//		�������� ������ - ���������� � ����� � ����������� ������
#define SEND2PC_CDATA			15
//
//		�������� ������ - ���������� � ���� � ������ ����������
#define SEND2PC_CALIBR			17



class Core
{
//variables
public:
protected:
private:
	uint16_t	distance;
	uint16_t	force;
	uint16_t	speed;
//functions
public:
	Core();
	~Core();
	void Interrupt();
	// ------------------------
	void SendHead();
	// �������� ���������
	void sendToPc_Mode(uint8_t mod);
	void sendToPc_Mode_sub(uint8_t mod, uint32_t sub_tik);
	// �������� alarm � ����� ������
	void sendToPc_Alarm(uint8_t code);
	// �������� ������� ������
	void Send_CurentData(uint8_t mod);
	// �������� ������
	void sendToPc_ForceZero();
	// ------------------------
	uint16_t getDistance();
	uint16_t getForce();
	uint16_t getSpeed();
	// ------------------------
protected:
private:
	Core( const Core &c );
	Core& operator=( const Core &c );
	void SendDataToPc();
	void Delta();
	// ----
	// �������� ������ � ������ ����������
	void Send_CalibrSensors();
	//
}; //Core

#endif //__CORE_H__
