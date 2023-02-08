/*
 * device.cpp
 *
 * Created: 22.09.2017 14:36:35
 *  Author: GumerovMN
 */ 

#include "device.h"

uint16_t acp_div = 0;

namespace ns_device
{
	FILE		*strmScr;
	FILE		*rs232StdOut;
	tc_key4		*key;
	// users
	SwitchMode	*switchMode;
	Core		*core;
	AdcCore		*adcCore; // = new AdcCore();
	// -
	uint16_t preDiv = PREDIV_ARCHIVE;
	
	void Init()
	{
		ns_LcdKeyTimer::Init();
		ns_timerMain::Init();
		strmScr = scr::Init(16);
		rs232StdOut = ns_rs232::Init(baud57600, DISABLE, bit8, size128, size256);
		ns_rs232::String_P(PSTR(" Start programm"));
		key = new tc_key4;
		// ====== users ======
		// ������������� ������
		switchMode = new SwitchMode(
			&DDRC, &PORTC, &PINC, 0,
			&DDRC, &PORTC, &PINC, 1,
			// ����� �����������
			eeprom_read_word(&VG::switch1_delay) , eeprom_read_word(&VG::switch2_delay),
			(TypeFunct1 *)&Sw_Event
		);
		// ======================
		// ���
		eeprom_read_block(&VG::adcCore_speedLenghtAve_ram, &VG::adcCore_speedLenghtAve, sizeof(uint16_t));
		eeprom_read_block(&VG::adcCore_speedLenghtDlt_ram, &VG::adcCore_speedLenghtDlt, sizeof(uint8_t));
		adcCore = new AdcCore(
			// ����� ������� ���������� ��� ���������� ��������
			VG::adcCore_speedLenghtAve_ram,
			// ����� ������� ������ ��� ���������� ��������
			VG::adcCore_speedLenghtDlt_ram,
			// ����� ������� ���������� ������ ��� ��������� ���������
			eeprom_read_byte(&VG::adcCore_distanceLenghtAve),
			// ����� ������� ���������� ������ ��� ��������� �������
			eeprom_read_byte(&VG::adcCore_forceLenghtAve)
		);
		// ����� ����
		core = new Core();
		// ����� ������
		ns_ModeWorks::Init();
		// =====================================================
		// ���� ������������ ���������, ����� ��� ������� �������
		ns_menu::Init();
		// ���
		ns_watchdog::Init(WDTO_2S);
	}
	void Timer_lcd()
	{	// �� ���������� ����������
		scr::Interrupt();
		key->Interrupt();
		ns_menu::Interrupt();
		// ����� ������������� ������ ������ (flNewSw = 1 - ����� ������ ; switchPos = switchMode->Status();)
		switchMode->Interrupt();
		adcCore->start();
		ns_ModeWorks::Interrupt();
	}
	void Timer_Usr()
	{
		if (--preDiv == 0)
		{
			preDiv = PREDIV_ARCHIVE;
			// ���� ������ � ���, �������� �� ���������
			if (core) core->Interrupt();
		}
		// ���� ���������� (�����) �� ���������
		ns_ModeWorks::InterruptTick();
	}
	void MainCicle()
	{
		ns_menu::Main();
		// ��������
		switchMode->MainCicle();
		ns_watchdog::ResetCount();
	}
}
