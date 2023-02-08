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

// коды посылок передачи на компьютер
// =========================
// *** передача текущих данных
// статус аларм, шток назад в ручном режиме
#define	SEND2PC_MALARM			0
// статус шток назад в ручном режиме
#define	SEND2PC_MBACK			1
// статус состо€ние —“ќѕ (исходное состо€ние)
#define	SEND2PC_STOP			2
// статус шток вперед в ручном режиме
#define	SEND2PC_MFORWARD		3
// статус шток на "полке" в ручном режиме
#define	SEND2PC_MSHELF			4
// статус аларм, шток назад в циклическом режиме
#define	SEND2PC_CALARM			5
// статус шток назад в циклическом режиме
#define	SEND2PC_CBACK			6
// статус пауза перед началом цикла
#define	SEND2PC_CDELAY			7
// статус шток вперед в циклическом режиме
#define	SEND2PC_CFORWARD		8
// статус шток на "полке" в циклическом режиме
#define	SEND2PC_CSHELF			9
//		передача данных - шток вперед
#define SEND2PC_DATA			11
//		передача данных - шток назад
#define SEND2PC_FORCE			12
//		сброс режимов
#define SEND2PC_RESET			13
// =========================
// *** передача измеренных данных
//		передача данных - рассто€ние и врем€ в ручном режиме
#define SEND2PC_MDATA			14
//		передача данных - рассто€ние и врем€ в циклическом режиме
#define SEND2PC_CDATA			15
//
//		передача данных - рассто€ни€ и веса в режиме калибровки
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
	// передача состо€ний
	void sendToPc_Mode(uint8_t mod);
	void sendToPc_Mode_sub(uint8_t mod, uint32_t sub_tik);
	// передача alarm с кодом ошибки
	void sendToPc_Alarm(uint8_t code);
	// передача текущих данных
	void Send_CurentData(uint8_t mod);
	// передача усили€
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
	// передача данных в режиме калибровки
	void Send_CalibrSensors();
	//
}; //Core

#endif //__CORE_H__
