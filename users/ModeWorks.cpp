/*
* ModeWorks.cpp
*
* Created: 25.05.2020 10:39:30
* Author: User
*/


#include "ModeWorks.h"
#include <avr/eeprom.h>

// ==============================
// переключатель режима работы на пульте, обратный вызов
namespace ns_ModeWorks { void ModeCallBack(); }
const TypeFunct1 Sw_Event[1] PROGMEM = {ns_ModeWorks::ModeCallBack};
// ==============================
// двигатель
#define ENGINE_INIT		DDRC	|=	(1 << 6)
#define ENGINE_ON		PORTC	&= ~(1 << 6)//; 
//scr::PutChar(13, '1')
#define ENGINE_OFF		PORTC	|=  (1 << 6)//; 
//scr::PutChar(13, '0')

#define switchMode	ns_device::switchMode
#define core		ns_device::core

// -----------------------------------------------
// в массиве
#define M_STOP	0
#define M_PUSK	1
#define M_CICL	2
#define M_NONE	3
#define M_SETM	4

typedef void(*TypeFnMode)();
#define FnMode(mode, stat)	((TypeFnMode)pgm_read_ptr(& MassFnMn[mode][stat]))()
#define SetMode(Mode)		FnMode(Mode, M_SETM)

namespace ns_ModeWorks
{
	// *********************** внутренние переменные *****************************
	extern const TypeFnMode MassFnMn[][5];
	// режим работы
	uint8_t mode = WMODE_M_ALARM;
	// флаг нового положения переключателя
	uint8_t flNewSw = 0;
	// положение выключателя
	uint8_t switchPos = 0;
	// скорость движения штока
	uint16_t speedMoveStock = 0;
	// флаг движение штока
	uint8_t flagMoveStock;
	// ***************** переменные читаемые/записываемые из вне *****************
 	uint32_t	ticker;
	// ************************* параметры настройки *****************************
	// защитный интервал при старте двигателя (тиках)
	uint16_t safeIntervalEngineOn_set			= 1000;
	uint16_t safeIntervalEngineOn_value			= safeIntervalEngineOn_set;
	// -------------------------------------------------------------------------------------
	// защитный интервал при останове двигателя (тиках)
	uint16_t safeIntervalEngineOff_set			= 1000;
	uint16_t safeIntervalEngineOff_value		= safeIntervalEngineOff_set;
	// -------------------------------------------------------------------------------------
	// время отстрочки регистрации начало выдвижения штока
	uint16_t thresholdSpeedStockTimeForwardStart_set	= 200;
	uint16_t thresholdSpeedStockTimeForwardStart_value	= thresholdSpeedStockTimeForwardStart_set;
	// время отстрочки регистрации окончания выдвижения штока
	uint16_t thresholdSpeedStockTimeForwardStop_set		= 200;
	uint16_t thresholdSpeedStockTimeForwardStop_value	= thresholdSpeedStockTimeForwardStop_set;
	// время отстрочки регистрации начало задвижения штока
	uint16_t thresholdSpeedStockTimeBackStart_set		= 200;
	uint16_t thresholdSpeedStockTimeBackStart_value		= thresholdSpeedStockTimeBackStart_set;
	// время отстрочки регистрации окончания задвижения штока
	uint16_t thresholdSpeedStockTimeBackStop_set		= 200;
	uint16_t thresholdSpeedStockTimeBackStop_value		= thresholdSpeedStockTimeBackStop_set;
	// ===========================================================================
	// порог скорости старта выдвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	uint16_t thresholdSpeedStockForward_start	= 15;
	// порог скорости остановки выдвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	uint16_t thresholdSpeedStockForward_stop	= 10;
	// -------------------------------------------------------------------------------------
	// порог скорости старта задвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	uint16_t thresholdSpeedStockBack_start		= 15;
	// порог скорости остановки задвижения штока ( (единица ацп / тик) * длина массива нахождения дельты )
	uint16_t thresholdSpeedStockBack_stop		= 10;
	// ===========================================================================
	// время нахождения штока на полке (тиках)
	uint16_t timeStockShelf_set					= 3000;
	uint16_t timeStockShelf_value				= timeStockShelf_set;
	// -------------------------------------------------------------------------------------
	// время нахождения штока в исходном положении (тиках)
	uint16_t timeStockBegin_set					= 3000;
	uint16_t timeStockBegin_value				= timeStockBegin_set;
	// -------------------------------------------------------------------------------------
	// минимальное время alarm
	uint16_t timeAlarm_set						= 5000;	// в секундвх
	uint16_t timeAlarm_value					= timeAlarm_set;
	// -------------------------------------------------------------------------------------
	// код ошибки аларм
	uint8_t alarmCode							= ALARM_CODE_NONE;
	uint16_t autoRepeatSendCout					= 0;
	#define  autoRepeatSendCoutMax				  1000
	// ===========================================================================
	// счетчик циклов измерений на автомате
	uint8_t n_cycle_value;
	// максимальное число измерений на автомате
	uint8_t n_cycle_max = 15;
	// ===========================================================================
	// ********** прием  команд с компьютера **********
	#define READPCMODE_SYNHRO		0
	#define READPCMODE_LENGHT		1
	#define READPCMODE_BODY			2
	void readFromPC();
	void readFromPC_fillSynhroBuf(uint8_t byte);
	void readPC_CheckedSynhro();
	// ---------------------- 0x80 ------------------------------
	//                стоп из компьютера
	uint8_t flag_StopFromPc = 0;
	// ---------------------- 0x81 ------------------------------
	//                 режим калибровки
	// флаг калибровки
	uint8_t flag_calibrovka = 0;
	// счетчик времени разрешения передачи данных
	uint8_t calibrovkaBackTimeSendToPc = 0;
	// время задержки разрешения передачи данных
	const uint8_t calibrovkaBackTimeSendToPc_SET = 10;
	// счетчик времени разрешения режима калибровки
	uint16_t calibrovkaBackTimeFlag_mode = 0;
	// время разрешения режима калибровки от момента команды
	const uint16_t calibrovkaBackTimeFlag_SET = 2000;
	//
	void calibrovka_checkSetFlags();
	void InitModeSet();
	// ===========================================================================
	// обратный вызов от модуля переключателя
	void ModeCallBack()
	{
		CRITICAL_SECTION
		{
			flNewSw = 1;
			switchPos = switchMode->Status();
		}
	}
	// =====================
	void setModeAlarm(uint8_t code) {
		alarmCode = code;
		SetMode(WMODE_M_ALARM);
	}
	// =====================
	// инициация
	void Init()
	{
		flag_StopFromPc = 0;
		ENGINE_INIT;
		ticker = 0;
		LoadVars();
		InitModeSet();
		switchPos = SWITCH_POS_INIT;
	}
	// прерывание стабильное
	void InterruptTick()
	{
		ticker++;
		if (calibrovkaBackTimeFlag_mode > 0) calibrovkaBackTimeFlag_mode--;
		if (calibrovkaBackTimeSendToPc > 0) calibrovkaBackTimeSendToPc--;
	}
	// прерывание не нормированное
	void Interrupt()
	{
//		scr::Digit (scr::SetPosition(11, 1), 4, calibrovkaBackTimeFlag_mode);
		// чтение команд от компьютера
		readFromPC();
		// установка флагов калибровки
		calibrovka_checkSetFlags();
		// в режиме калибровка
		if (flag_calibrovka != 0) {
			if (switchPos == SWITCH_POS_PUSK) {
				// включить двигатель
				ENGINE_ON;
			} else {
				// отключить двигатель
				ENGINE_OFF;
			}
			return;
		}
		// чтение успокоения и отскока
		speedMoveStock = core->getSpeed();
		uint8_t stat = M_NONE;
		if (flNewSw)
		{
			flNewSw = 0;
			switch (switchPos)
			{
				// sop
				case SWITCH_POS_STOP:
				stat = M_STOP;
				break;
				case SWITCH_POS_PUSK:
				stat = M_PUSK;
				break;
				case SWITCH_POS_CICL:
				stat = M_CICL;
				break;
			}
		}
		FnMode(mode, stat);
	}
	void Dupm() {}
	// ===================================================
	// ожидание успокоения и ключа в режим "стоп"
	void ManualAlarmSet()
	{
		// отключить двигатель
		ENGINE_OFF;
		// режим работы в компьютер
		//core->sendToPc_Mode(SEND2PC_MALARM);
		core->sendToPc_Alarm(alarmCode);
		// режим работы
		mode = WMODE_M_ALARM;
		// минимальное время alarm
		timeAlarm_value = timeAlarm_set;
		//
		autoRepeatSendCout = autoRepeatSendCoutMax;
	}
	void ManualAlarmNone()
	{
		if (autoRepeatSendCout == 0) {
			core->sendToPc_Alarm(alarmCode);
			autoRepeatSendCout = autoRepeatSendCoutMax;
		} else {
			autoRepeatSendCout--;
		}
		// ожидание остановки штока
		if ( speedMoveStock > thresholdSpeedStockBack_stop ) {
			timeAlarm_value = timeAlarm_set;
			return;
		}
		// время в режиме аларм
		if (timeAlarm_value > 0) {
			timeAlarm_value--;
			return;
		}
		// режим работы стоп
		if (switchPos == SWITCH_POS_STOP){
			// сброс в pc
			core->sendToPc_Mode(SEND2PC_RESET);
			SetMode(WMODE_M_STOP);
		}
	}
	// ===================================================
	void ManualBackSet()
	{
		mode = WMODE_M_BACK;
		// режим работы в компьютер
		core->sendToPc_Mode(SEND2PC_MBACK);
		// отключить двигатель
		ENGINE_OFF;
		// флаг движение штока
		flagMoveStock = 0;
		// защитный интервал при останове двигателя (тиках)
		safeIntervalEngineOff_value = safeIntervalEngineOff_set;
		// время отстрочки регистрации начало задвижения штока
		thresholdSpeedStockTimeBackStart_value = thresholdSpeedStockTimeBackStart_set;
	}
	void ManualBackNone()
	{
		// защитный интервал при останове двигателя (тиках)
		if (--safeIntervalEngineOff_value == 0) {
			// шток заклинило - авария
			//SetMode(WMODE_M_ALARM);
			setModeAlarm(ALARM_CODE_SAFE_ENGINE_OFF);
			return;
		}
		// флаг движение штока
		if (flagMoveStock == 0) {
			// ожидание начало задвижение штока
			if (speedMoveStock > thresholdSpeedStockBack_start) {
				if (thresholdSpeedStockTimeBackStart_value > 0) thresholdSpeedStockTimeBackStart_value--;
				else {
					// время отстрочки регистрации окончания задвижения штока
					thresholdSpeedStockTimeBackStop_value = thresholdSpeedStockTimeBackStop_set;
					flagMoveStock = 1;
				}
			} else {
				thresholdSpeedStockTimeBackStart_value = thresholdSpeedStockTimeBackStart_set;
			}
		} else {
			// ожидание остановки штока
			if (speedMoveStock < thresholdSpeedStockBack_stop) {
				if (thresholdSpeedStockTimeBackStop_value > 0) thresholdSpeedStockTimeBackStop_value--;
				else {
					// завершение цикла
					SetMode(WMODE_M_STOP);
				}
			} else {
				thresholdSpeedStockTimeBackStop_value = thresholdSpeedStockTimeBackStop_set;
			}
		}
	}
	// ===================================================
	void ManualStopSet()
	{
		// отключить двигатель
		ENGINE_OFF;
		uint8_t old_mod = mode;
		mode = WMODE_M_STOP;
		// режим работы в компьютер
		if (old_mod == WMODE_C_BACK) {
			core->sendToPc_Mode_sub(SEND2PC_STOP, thresholdSpeedStockTimeForwardStop_set + VG::adcCore_speedLenghtAve_ram + VG::adcCore_speedLenghtDlt_ram);
		} else {
			core->sendToPc_Mode(SEND2PC_STOP);
		}
		//
		autoRepeatSendCout = autoRepeatSendCoutMax;
	}
	void ManualStopNone() {
		if (autoRepeatSendCout == 0) {
			core->sendToPc_Mode(SEND2PC_RESET);
			core->sendToPc_Mode(SEND2PC_STOP);
			autoRepeatSendCout = autoRepeatSendCoutMax;
			} else {
			autoRepeatSendCout--;
		}
	}
	// ===================================================
	// шток вперед, начало измерений
	void ManualPuskSet()
	{
		// загрузка начальных значений
		LoadVars();
		// время начало измерения
		CRITICAL_SECTION { ticker = 0; }
		// режим работы
		mode = WMODE_M_PUSK;
		// режим работы в компьютер
		core->sendToPc_Mode(SEND2PC_MFORWARD);
		// начальное усилие в компьютер
		core->sendToPc_ForceZero();
		// флаг движение штока
		flagMoveStock = 0;
		// защитный интервал при старте двигателя (тиках)
		safeIntervalEngineOn_value = safeIntervalEngineOn_set;
		// время отстрочки регистрации начало выдвижения штока
		thresholdSpeedStockTimeForwardStart_value = thresholdSpeedStockTimeForwardStart_set;
		// включить двигатель
		ENGINE_ON;
	}
	// шток вперед
	void ManualPuskNone() {
		// защитный интервал при старте двигателя (тиках)
		if (--safeIntervalEngineOn_value == 0) {
			// режим работы авария
			//SetMode(WMODE_M_ALARM);
			setModeAlarm(ALARM_CODE_SAFE_ENGINE_ON);
			return;
		}
		if (flagMoveStock == 0) {
			// ожидание движение штока
			if (speedMoveStock > thresholdSpeedStockForward_start) {
				if (thresholdSpeedStockTimeForwardStart_value > 0) thresholdSpeedStockTimeForwardStart_value--;
				else {
					// время отсрочки регистрации окончания выдвижения штока
					thresholdSpeedStockTimeForwardStop_value = thresholdSpeedStockTimeForwardStop_value;
					flagMoveStock = 1;
				}
			} else {
				thresholdSpeedStockTimeForwardStart_value = thresholdSpeedStockTimeForwardStart_set;
			}
		} else {
			// ожидание торможения штока
			if (speedMoveStock < thresholdSpeedStockForward_stop) {
				if (thresholdSpeedStockTimeForwardStop_value > 0) thresholdSpeedStockTimeForwardStop_value--;
				else {
					// режим работы полка
					SetMode(WMODE_M_SHELF);
				}
			} else {
				thresholdSpeedStockTimeForwardStop_value = thresholdSpeedStockTimeForwardStop_set;
			}
		}
	}
	// переключатель в стоп
	void ManualPuskStop() {
		setModeAlarm(ALARM_CODE_ABORT);
	}
	// перекл.чатель в режим авто
	void ManualPuskCycl() {
		setModeAlarm(ALARM_CODE_SWITCH);
	}
	// ===================================================
	// двигатель работает, измерения вперед завершены, выход из режима через возврат - переключатель в стоп
	void ManualShelfSet() {
		mode = WMODE_M_SHELF;
		// двигатель включен
		ENGINE_ON;
		// режим работы в компьютер
		//core->sendToPc_Mode(SEND2PC_MSHELF);
		core->sendToPc_Mode_sub(SEND2PC_MSHELF, thresholdSpeedStockTimeForwardStop_set + VG::adcCore_speedLenghtAve_ram + VG::adcCore_speedLenghtDlt_ram);
		// время нахождения штока на полке (тиках)
//		timeStockShelf_value = timeStockShelf_set;
	}
// 	void ManualShelfNone() {
// 		// ожидание окончания времени полки
// 		if (timeStockShelf_value > 0) {
// 			timeStockShelf_value--;
// 			return;
// 		}
// 		// перейти в режим обратный ход
// 		SetMode(WMODE_M_BACK);
// 	}
	// ===================================================
	void CicleBackSet() {
		// режим работы
		mode = WMODE_C_BACK;
		// режим работы в компьютер
		core->sendToPc_Mode(SEND2PC_CBACK);
		// отключить двигатель
		ENGINE_OFF;
		// флаг движения штока
		flagMoveStock = 0;
		// защитный интервал при останове двигателя (тиках)
		safeIntervalEngineOff_value = safeIntervalEngineOff_set;
		// время отстрочки регистрации начало задвижения штока
		thresholdSpeedStockTimeBackStart_value = thresholdSpeedStockTimeBackStart_set;
	}
	void CicleBackNone() {
		// защитный интервал при останове двигателя
		if (--safeIntervalEngineOff_value == 0) {
			// шток заклинело - авария
			//SetMode(WMODE_M_ALARM);
			setModeAlarm(ALARM_CODE_SAFE_ENGINE_OFF);
			return;
		}
		// флаг движения штока
		if (flagMoveStock == 0) {
			// ожидание начало движения штока
			if (speedMoveStock > thresholdSpeedStockBack_start) {
				if (thresholdSpeedStockTimeBackStart_value > 0) thresholdSpeedStockTimeBackStart_value--;
				else {
					flagMoveStock = 1;
					// время отстрочки регистрации окончания задвижения штока
					thresholdSpeedStockTimeBackStop_value = thresholdSpeedStockTimeBackStop_set;
				}
			} else {
				thresholdSpeedStockTimeBackStart_value = thresholdSpeedStockTimeBackStart_set;
			}
		} else {
			// ожидание остановки штока
			if (speedMoveStock < thresholdSpeedStockBack_stop) {
				if (thresholdSpeedStockTimeBackStop_value > 0) thresholdSpeedStockTimeBackStop_value--;
				else {
					switch (switchPos) {
						// останов замеров
						case SWITCH_POS_STOP:
//							SetMode(WMODE_RESET);
							SetMode(WMODE_M_STOP);
						break;
						// невозможный вариант
						case SWITCH_POS_PUSK:
							setModeAlarm(ALARM_CODE_SWITCH);
						break;
						default:
							// пауза между циклами
							SetMode(WMODE_C_DELAY);
					}
				}
			} else {
				thresholdSpeedStockTimeBackStop_value = thresholdSpeedStockTimeBackStop_set;
			}
		}
	}
	void CicleBackStop() {
		setModeAlarm(ALARM_CODE_ABORT);
	}
	// ===================================================
	void CicleDelaySet() {
		// отключить двигатель
		ENGINE_OFF;
		// проверка количество пройденных циклов
		if (n_cycle_value >= n_cycle_max) {
			// конец циклов - режим стоп
			SetMode(WMODE_M_STOP);
			return;
		}
		// режим работы
		mode = WMODE_C_DELAY;
		// режим работы в компьютер
		//core->sendToPc_Mode(SEND2PC_CDELAY);
		core->sendToPc_Mode_sub(SEND2PC_CDELAY, thresholdSpeedStockTimeForwardStop_set + VG::adcCore_speedLenghtAve_ram + VG::adcCore_speedLenghtDlt_ram);
		// время нахождения штока в исходном положении (тиках)
		timeStockBegin_value = timeStockBegin_set;
	}
	void CicleDelayNone() {
		// проверка флага прерывания цикла из компьютера
		if (flag_StopFromPc != 0) {
			// в режим стоп
			SetMode(WMODE_M_STOP);
			return;
		}
		// время нахождения штока в исходном положении
		if (timeStockBegin_value > 0) {
			timeStockBegin_value--;
			return;
		}
		// в начало цикла
		SetMode(WMODE_C_PUSK);
	}
	// ===================================================
	// начало цикла
	void CiclePuskSet()
	{
		// режим работы
		if (mode == WMODE_M_STOP) {
			// первый цикл
			n_cycle_value = 1;
			// сброс флага прерывание циклов с компьютера
			flag_StopFromPc = 0;
			// загрузка начальных значений
			LoadVars();
		} else {
			// последующие циклы
			n_cycle_value++;
		}
		// режим работы
		mode = WMODE_C_PUSK;
		CRITICAL_SECTION { ticker = 0; }
		// режим работы в компьютер
		core->sendToPc_Mode(SEND2PC_CFORWARD);
		// начальное усилие
		core->sendToPc_ForceZero();
		// флаг движения штока
		flagMoveStock = 0;
		// защитный интервал при старте двигателя штока
		safeIntervalEngineOn_value = safeIntervalEngineOn_set;
		// время отстрочки регистрации начало выдвижения штока
		thresholdSpeedStockTimeForwardStart_value = thresholdSpeedStockTimeForwardStart_set;
		// включить двигатель
		ENGINE_ON;
	}
	// шток вперед
	void CiclePuskNone() {
		// защитный интервал при старте двигателя штока
		if (--safeIntervalEngineOn_value == 0) {
			// режим работы - авария
			//SetMode(WMODE_M_ALARM);
			setModeAlarm(ALARM_CODE_SAFE_ENGINE_ON);
			return;
		}
		// флаг движения штока
		if (flagMoveStock == 0) {
			// ожидание движения штока
			if (speedMoveStock > thresholdSpeedStockForward_start) {
				if (thresholdSpeedStockTimeForwardStart_value > 0) thresholdSpeedStockTimeForwardStart_value--;
				else {
					flagMoveStock = 1;
					// время отстрочки регистрации окончания выдвижения штока
					thresholdSpeedStockTimeForwardStop_value = thresholdSpeedStockTimeForwardStop_set;
				}
			} else {
				thresholdSpeedStockTimeForwardStart_value = thresholdSpeedStockTimeForwardStart_set;
			}
		} else {
			// ожидание остановки штока
			if (speedMoveStock < thresholdSpeedStockForward_stop) {
				if (thresholdSpeedStockTimeForwardStop_value > 0) thresholdSpeedStockTimeForwardStop_value--;
				else {
					 // режим работы полка
					SetMode(WMODE_C_SHELF);
				}
			} else {
				thresholdSpeedStockTimeForwardStop_value = thresholdSpeedStockTimeForwardStop_set;
			}
		}
	}
	// ключ на "стоп"
	void CiclePuskStop() {
		// номер текущего цикла
		if (n_cycle_value == 1) {
			// режим авария
			//SetMode(WMODE_M_ALARM);
			setModeAlarm(ALARM_CODE_ABORT);
			return;
		}
		// режим сброс
		SetMode(WMODE_RESET);
	}
	// невозможный вариант:
	// аварийный режим - ключ на "пуск"
	void CiclePuskPusk() {
		// режим авария
		//SetMode(WMODE_M_ALARM);
		setModeAlarm(ALARM_CODE_SWITCH);
	}
	// ===================================================
	// режим полки
	void CicleShelfSet()
	{
		// режим работы
		mode = WMODE_C_SHELF;
		// включить двигатель
		ENGINE_ON;
		// режим работы в компьютер
		//core->sendToPc_Mode(SEND2PC_CSHELF);
		core->sendToPc_Mode_sub(SEND2PC_CSHELF, thresholdSpeedStockTimeForwardStop_set + VG::adcCore_speedLenghtAve_ram + VG::adcCore_speedLenghtDlt_ram);
		// время нахождения штока на полке
		timeStockShelf_value = timeStockShelf_set;
	}
	void CicleShelfNone() {
		// ожидание окончания времени полки
		if (timeStockShelf_value > 0) {
			timeStockShelf_value--;
			return;
		}
		// перейти в режим обратный ход
		SetMode(WMODE_C_BACK);
	}
	void CicleShelfStop() {
		setModeAlarm(ALARM_CODE_ABORT);
	}
	// ===================================================
	uint16_t delayInitMode;
	void InitModeSet()
	{
		// отключить двигатель
		ENGINE_OFF;
		mode = WMODE_I_INIT;
		delayInitMode = 1000;
	}
	void InitModeNone() {
		if (delayInitMode > 0) {
			delayInitMode--;
			return;
		}
		if (switchMode->Status() == SWITCH_POS_STOP) {
			SetMode(WMODE_M_STOP);
			return;
		}
		setModeAlarm(ALARM_CODE_NONE);
	}
	// ===================================================
	void ResetModeSet() {
		// отключить двигатель
		ENGINE_OFF;
		// в компьютер сброс
		core->sendToPc_Mode(SEND2PC_RESET);
		if (switchPos == SWITCH_POS_STOP) {
			SetMode(WMODE_M_STOP);
		}
	}
	// ===================================================
	// прием команд от компьютера
	uint8_t readPC_mode = READPCMODE_SYNHRO;
	#define readPC_synhroBufLenght 4
	uint8_t readPC_synhroBuf[readPC_synhroBufLenght];
	uint8_t readPC_lenghtBlock;
	#define readPC_blockMax 16
	uint8_t readPC_block[readPC_blockMax];
	uint8_t readPC_lenghtBlockIndx;
	void readFromPC() {
		uint8_t byte;
		while(ns_rs232::ReadByte(&byte) == 1) {
			readFromPC_fillSynhroBuf(byte);
			if (readPC_mode == READPCMODE_SYNHRO) {
				readPC_CheckedSynhro();
				continue;
			}
			if (readPC_mode == READPCMODE_LENGHT) {
				if (byte > readPC_blockMax) {
					readPC_mode = READPCMODE_SYNHRO;
					readPC_CheckedSynhro();
					continue;
				}
				readPC_lenghtBlock = byte;
				readPC_lenghtBlockIndx = 0;
				readPC_mode = READPCMODE_BODY;
				continue;
			}
			if (readPC_mode == READPCMODE_BODY) {
				readPC_block[readPC_lenghtBlockIndx] = byte;
				readPC_lenghtBlockIndx++;
				if (readPC_lenghtBlockIndx < readPC_lenghtBlock) continue;
				// проверка к.с.
				if (readPC_block[readPC_lenghtBlock - 1] != crc8_buf(readPC_block, readPC_lenghtBlock - 1)) {
					// ошибка передачи
					readPC_mode = READPCMODE_SYNHRO;
					readPC_CheckedSynhro();
					continue;
				}
				readPC_mode = READPCMODE_SYNHRO;
				if (readPC_block[0] == _FromPcCommand_Stop) {
					// стоп из компьютера
					flag_StopFromPc = 1;
					continue;
				}
				if (readPC_block[0] == _FromPcCommand_CycleMax) {
					// число циклов в автоматическом режиме
					n_cycle_max = readPC_block[1];
					eeprom_update_block(&n_cycle_max, &VG::n_cycle_max, sizeof(uint8_t));
					continue;
				}
				if (readPC_block[0] == _FromPcCommand_Calibration) {
					// режим калибровки
					calibrovkaBackTimeFlag_mode = calibrovkaBackTimeFlag_SET;
				}
			}
		}
	}
	void readFromPC_fillSynhroBuf(uint8_t byte) {
		for (uint8_t i = 0; i < readPC_synhroBufLenght - 1; i++) {
			readPC_synhroBuf[i] = readPC_synhroBuf[i + 1];
		}
		readPC_synhroBuf[readPC_synhroBufLenght - 1] = byte;
	}
	void readPC_CheckedSynhro() {
		if (readPC_synhroBuf[0] != 0xe6) return;
		if (readPC_synhroBuf[1] != 0x19) return;
		if (readPC_synhroBuf[2] != 0x55) return;
		if (readPC_synhroBuf[3] != 0xaa) return;
		readPC_mode = READPCMODE_LENGHT;
	}
	// ===================================================
	// установка флагов калибровки
	void calibrovka_checkSetFlags() {
		CRITICAL_SECTION {
			if (calibrovkaBackTimeFlag_mode > 0) {
				flag_calibrovka = 1;
			} else {
				flag_calibrovka = 0;
				calibrovkaBackTimeSendToPc = calibrovkaBackTimeSendToPc_SET;
			}
		}
	}
	// ===================================================
	const TypeFnMode MassFnMn[][5] PROGMEM =
	{//					stop				pusk				cycl				none			     setmode
		{				Dupm,				Dupm,				Dupm,	 ManualAlarmNone,		  ManualAlarmSet	},	// 0  ручной режим: аларм
		{				Dupm,				Dupm,				Dupm,	  ManualBackNone,		   ManualBackSet	},	// 1  ручной режим: назад
		{				Dupm,	   ManualPuskSet,		CiclePuskSet,	  ManualStopNone,		   ManualStopSet	},	// 2  ручной режим: стоп
		{	  ManualPuskStop,				Dupm,	  ManualPuskCycl,	  ManualPuskNone,		   ManualPuskSet	},	// 3  ручной режим: пуск
		{	   ManualBackSet,				Dupm,				Dupm,				Dupm,		  ManualShelfSet	},	// 4  ручной режим: полка
		{				Dupm,				Dupm,				Dupm,				Dupm,					Dupm	},	// 5  цикличный режим: аларм - убран
		{	   CicleBackStop,				Dupm,				Dupm,	   CicleBackNone,			CicleBackSet	},	// 6  цикличный режим: назад
		{	   ManualStopSet,	  ManualAlarmSet,				Dupm,	  CicleDelayNone,		   CicleDelaySet	},	// 7  цикличный режим: задержка
		{	   CiclePuskStop,	   CiclePuskPusk,				Dupm,	   CiclePuskNone,			CiclePuskSet	},	// 8  цикличный режим: пуск
		{	  CicleShelfStop,				Dupm,				Dupm,	  CicleShelfNone,		   CicleShelfSet	},	// 9  цикличный режим: полка
		{	   ManualStopSet,				Dupm,				Dupm,		InitModeNone,			 InitModeSet	},	// 10 начальный режим после сброса
		{	   ManualStopSet,				Dupm,				Dupm,				Dupm,			ResetModeSet	},	// 11 RESET
		{				Dupm,				Dupm,				Dupm,				Dupm,					Dupm	}	// 12
	};
	const char Message_ManualAlarm[]	PROGMEM = "Manual Alarm";
	const char Message_ManualBack[]		PROGMEM = "Manual Back";
	const char Message_Stop[]			PROGMEM = "Stop";
	const char Message_ManualForward[]	PROGMEM = "Manual Forward";
	const char Message_ManualShelf[]	PROGMEM = "Manual Shelf";
	const char Message_CicleAlarm[]		PROGMEM = "Cicle Alarm";
	const char Message_CicleBack[]		PROGMEM = "Cicle Back";
	const char Message_CicleDelay[]		PROGMEM = "Cicle Delay";
	const char Message_CicleForward[]	PROGMEM = "Cicle Forward";
	const char Message_CicleShelf[]		PROGMEM = "Cicle Shelf";
	const char *Message_Status[] =
	{
		Message_ManualAlarm,
		Message_ManualBack,
		Message_Stop,
		Message_ManualForward,
		Message_ManualShelf,
		Message_CicleAlarm,
		Message_CicleBack,
		Message_CicleDelay,
		Message_CicleForward,
		Message_CicleShelf
	};
}
