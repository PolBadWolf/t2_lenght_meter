/* 
* SwitchMode.h
*
* Created: 21.05.2020 15:17:22
* Author: User
*/

#ifndef __PATH_H_
#include "path.h"
#endif

#ifndef __SWITCHMODE_H__
#define __SWITCHMODE_H__

#define SWITCH_POS_STOP		0
#define SWITCH_POS_PUSK		1
#define SWITCH_POS_CICL		2
#define SWITCH_POS_INIT		255

class SwitchMode
{
//variables
public:
protected:
private:
	volatile uint8_t	*sw_ddr[2];
	volatile uint8_t	*sw_port[2];
	volatile uint8_t	*sw_pin[2];
	uint8_t				sw_mask[2];
	// статус (состояние)
	uint8_t		sw_stat[2];
	uint16_t	sw_count[2];
	uint16_t	sw_level[2];
	//
	uint8_t		flagEvent;
	//
	TypeFunct1	*LinkFunction;
//functions
public:
	SwitchMode(
		volatile uint8_t *sw1_ddr, volatile uint8_t *sw1_port, volatile uint8_t *sw1_pin, uint8_t sw1_bit,
		volatile uint8_t *sw2_ddr, volatile uint8_t *sw2_port, volatile uint8_t *sw2_pin, uint8_t sw2_bit,
		uint16_t sw1_levelIntegr, uint16_t sw2_levelIntegr
	);
	SwitchMode(
		volatile uint8_t *sw1_ddr, volatile uint8_t *sw1_port, volatile uint8_t *sw1_pin, uint8_t sw1_bit,
		volatile uint8_t *sw2_ddr, volatile uint8_t *sw2_port, volatile uint8_t *sw2_pin, uint8_t sw2_bit,
		uint16_t sw1_levelIntegr, uint16_t sw2_levelIntegr,
		TypeFunct1 *CallBack
	);
	~SwitchMode();
	//
	void MainCicle(); // пустая
	void Interrupt(); // точность не требуется
	uint8_t Status(); // статус: 0 - стоп, 1 - пуск, 2 - цикл
protected:
private:
	SwitchMode( const SwitchMode &c );
	SwitchMode& operator=( const SwitchMode &c );
	void Init(
	volatile uint8_t *sw1_ddr, volatile uint8_t *sw1_port, volatile uint8_t *sw1_pin, uint8_t sw1_bit,
	volatile uint8_t *sw2_ddr, volatile uint8_t *sw2_port, volatile uint8_t *sw2_pin, uint8_t sw2_bit,
	uint16_t sw1_levelIntegr, uint16_t sw2_levelIntegr,
	TypeFunct1 *LinkFunction
	);
	//
	void CallBack();

}; //SwitchMode

#endif //__SWITCHMODE_H__
