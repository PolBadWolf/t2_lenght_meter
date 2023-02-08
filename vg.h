/* 
* vg.h
*
* Created: 07.11.2017 13:19:00
* Author: GumerovMN
*/


#ifndef __PATH_H_
#include "path.h"
#endif

#ifndef __VG_H__
#define __VG_H__

#include <avr/eeprom.h>

namespace VG
{
	// *************** adc core **********************
	// длина буффера усреднения для вычисления скорости
	extern uint16_t EEMEM adcCore_speedLenghtAve;
	extern uint16_t adcCore_speedLenghtAve_ram;
	// длина буффера дельты для нахождения скорости
	extern uint8_t EEMEM adcCore_speedLenghtDlt;
	extern uint8_t adcCore_speedLenghtDlt_ram;
	// длина буффера усреднения данных для архивации дистанции
	extern uint8_t EEMEM adcCore_distanceLenghtAve;
	// длина буффера усреднения данных для архивации услилия
	extern uint8_t EEMEM adcCore_forceLenghtAve;
	// ************* mode work ***********************
	// защитный интервал при старте двигателя (тиках)
	extern uint16_t EEMEM safeIntervalEngineOn_set;
	// -------------------------------------------------------------------------------------
	// защитный интервал при останове двигателя (тиках)
	extern uint16_t EEMEM safeIntervalEngineOff_set;
	// -------------------------------------------------------------------------------------
	// время отстрочки регистрации начало выдвижения штока
	extern uint16_t EEMEM thresholdSpeedStockTimeForwardStart_set;
	// время отстрочки регистрации окончания выдвижения штока
	extern uint16_t EEMEM thresholdSpeedStockTimeForwardStop_set;
	// время отстрочки регистрации начало задвижения штока
	extern uint16_t EEMEM thresholdSpeedStockTimeBackStart_set;
	// время отстрочки регистрации окончания задвижения штока
	extern uint16_t EEMEM thresholdSpeedStockTimeBackStop_set;
	// ===========================================================================
	// порог скорости старта выдвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	extern uint16_t EEMEM thresholdSpeedStockForward_start;
	// порог скорости остановки выдвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	extern uint16_t EEMEM thresholdSpeedStockForward_stop;
	// -------------------------------------------------------------------------------------
	// порог скорости старта задвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	extern uint16_t EEMEM thresholdSpeedStockBack_start;
	// порог скорости остановки задвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	extern uint16_t EEMEM thresholdSpeedStockBack_stop;
	// ===========================================================================
	// время нахождения штока на полке (тиках)
	extern uint16_t EEMEM timeStockShelf_set;
	// -------------------------------------------------------------------------------------
	// время нахождения штока в исходном положении (тиках)
	extern uint16_t EEMEM timeStockDelay_set;
	// -------------------------------------------------------------------------------------
	// минимальное время alarm
	extern uint16_t EEMEM timeAlarm_set;
	// ===========================================================================
	// максимальное число измерений на автомате
	extern uint8_t EEMEM n_cycle_max;
	// ===========================================================================
	// время переключения контактов переключателя****
	extern uint16_t EEMEM switch1_delay;
	extern uint16_t EEMEM switch2_delay;
}

#endif //__VG_H__
