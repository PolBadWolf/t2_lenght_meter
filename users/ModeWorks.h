/* 
* ModeWorks.h
*
* Created: 25.05.2020 10:39:30
* Author: User
*/
#ifndef __PATH_H_
#include "path.h"
#endif


#ifndef __MODEWORKS_H__
#define __MODEWORKS_H__

// режимы работы
#define		WMODE_M_ALARM		0
#define		WMODE_M_BACK		1
#define		WMODE_M_STOP		2
#define		WMODE_M_PUSK		3
#define		WMODE_M_SHELF		4
//#define		WMODE_C_ALARM		5
#define		WMODE_C_BACK		6
#define		WMODE_C_DELAY		7
#define		WMODE_C_PUSK		8
#define		WMODE_C_SHELF		9
#define		WMODE_I_INIT		10
#define		WMODE_RESET			11

// коды ошибок alarm
#define		ALARM_CODE_NONE				0
// прерывание режима измерения оператором
#define		ALARM_CODE_ABORT			1
// ошибка переключателя
#define		ALARM_CODE_SWITCH			2
// защитный интервал при пуске выдвижения штока
#define		ALARM_CODE_SAFE_ENGINE_ON	3
// защитный интервал при пуске задвижения штока
#define		ALARM_CODE_SAFE_ENGINE_OFF	4

// -----------------------------
// **************** команды из компьютера *****************
// завершение автоматического режима работы
#define		_FromPcCommand_Stop			0x80
// количество циклов в автоматическом режиме
#define		_FromPcCommand_CycleMax		0x81
// режим калибровки
#define		_FromPcCommand_Calibration	0x82

extern const TypeFunct1 Sw_Event[1];

namespace ns_ModeWorks
{
	// инициация
	void Init();
	// прерывание
	void InterruptTick();
	void Interrupt();
	// =====================
 	extern uint32_t		ticker;
	// =====================
	// режим работы
	extern uint8_t		mode;
	extern uint8_t		flBackWork;
	// =====================
	// флуктуация т отскок
	extern uint16_t speedMoveStock;
	// =====================
	extern const char *Message_Status[];
	// ---------------------- 0x81 ------------------------------
	//                 режим калибровки
	// флаг калибровки
	extern uint8_t flag_calibrovka;
	// счетчик времени разрешения передачи данных
	extern uint8_t calibrovkaBackTimeSendToPc;
	// время задержки разрешения передачи данных
	extern const uint8_t calibrovkaBackTimeSendToPc_SET;
	// счетчик времени разрешения режима калибровки
	extern uint16_t calibrovkaBackTimeFlag_mode;
	// время разрешения режима калибровки от момента команды
	extern const uint16_t calibrovkaBackTimeFlag_SET;
	// ********************** параметры настройки ***************************
	// защитный интервал при старте двигателя (тиках)
	extern uint16_t safeIntervalEngineOn_set;
	// -------------------------------------------------------------------------------------
	// защитный интервал при останове двигателя (тиках)
	extern uint16_t safeIntervalEngineOff_set;
	// -------------------------------------------------------------------------------------
	// время отстрочки регистрации начало выдвижения штока
	extern uint16_t thresholdSpeedStockTimeForwardStart_set;
	// время отстрочки регистрации окончания выдвижения штока
	extern uint16_t thresholdSpeedStockTimeForwardStop_set;
	// время отстрочки регистрации начало задвижения штока
	extern uint16_t thresholdSpeedStockTimeBackStart_set;
	// время отстрочки регистрации окончания задвижения штока
	extern uint16_t thresholdSpeedStockTimeBackStop_set;
	// ===========================================================================
	// порог скорости старта выдвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	extern uint16_t thresholdSpeedStockForward_start;
	// порог скорости остановки выдвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	extern uint16_t thresholdSpeedStockForward_stop;
	// -------------------------------------------------------------------------------------
	// порог скорости старта задвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	extern uint16_t thresholdSpeedStockBack_start;
	// порог скорости остановки задвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	extern uint16_t thresholdSpeedStockBack_stop;
	// ===========================================================================
	// время нахождения штока на полке (тиках)
	extern uint16_t timeStockShelf_set;
	// -------------------------------------------------------------------------------------
	// время нахождения штока в исходном положении (тиках)
	extern uint16_t timeStockBegin_set;
	// -------------------------------------------------------------------------------------
	// минимальное время alarm
	extern uint16_t timeAlarm_set;
	// код ошибки аларм
	extern uint8_t alarmCode;
	// ===========================================================================
	// максимальное число измерений на автомате
	extern uint8_t n_cycle_max;
}

#endif //__MODEWORKS_H__
