/* 
* MenuEdit_Uint16T.cpp
*
* Created: 13.03.2023 7:38:42
* Author: Gumerovmn
*/


#include "MenuEdit_Uint16T.h"
#include "../../device.h"
//
#include "menu_mode.h"

namespace	ns_menu
{
	extern	uint16_t	timeOut;
	extern	uint8_t		mode;
	// тут надо организовать функцию для аварийного выхода
	void Main_menu();
}

#define		mode		ns_menu::mode
#define		timeOut		ns_menu::timeOut
#define		Main_menu	ns_menu::Main_menu
#define		key4		ns_device::key
#define		scr			ns_device::scr

MenuEdit_Uint16T	*MenuEdit_Uint16T::obj;

// default constructor
MenuEdit_Uint16T::MenuEdit_Uint16T()
{
	obj = this;
} //MenuEdit_Uint16T

// default destructor
// MenuEdit_Uint16T::~MenuEdit_Uint16T()
// {
// } //~MenuEdit_Uint16T

void MenuEdit_Uint16T::init(uint16_t dat, uint16_t min, uint16_t max, char *strHead, char *string, CallBackEndEdit_u16 callBack)
{
	this->dat = dat;
	this->min = min;
	this->max = max;
	endEdit = callBack;
	//
	mode = MODE_EDIT_UINT16T;
	key4->setAutoRepeatOff();
	scr->Clear();
	scr->pos = scr->SetPosition(0, 0);
	scr->String_P(strHead);
	scr->String_P(PSTR(" :"));
	scr->String_P(scr->SetPosition(6, 1), string);
	viewL();
	CRITICAL_SECTION	{ timeOut = TIMEOUT_EDIT; }
}

void	MenuEdit_Uint16T::viewL()
{
	scr->Digit(scr->SetPosition(0, 1), 5, dat);
}

void	MenuEdit_Uint16T::minusL()
{
	key4->setAutoRepeatOn();
	CRITICAL_SECTION	{	timeOut = TIMEOUT_EDIT;		}
	if (dat > min)		dat--;
	else				dat = min;
	viewL();
}

void	MenuEdit_Uint16T::plusL()
{
	key4->setAutoRepeatOn();
	CRITICAL_SECTION	{	timeOut = TIMEOUT_EDIT;		}
	if (dat < max)		dat++;
	else				dat = max;
	viewL();
}

void	MenuEdit_Uint16T::enterL()
{
	key4->setAutoRepeatOff();
	CRITICAL_SECTION	{	timeOut = 0;	}
	if (endEdit != nullptr)		endEdit(dat);
	// тут надо использовать функцию для аварийного выхода
	else						Main_menu();
	endEdit = nullptr;
}

void	MenuEdit_Uint16T::minus()
{
	if (obj != nullptr)		obj->minusL();
	// тут надо использовать функцию для аварийного выхода
	else						Main_menu();
}

void	MenuEdit_Uint16T::plus()
{
	if (obj != nullptr)		obj->plusL();
	// тут надо использовать функцию для аварийного выхода
	else						Main_menu();
}

void	MenuEdit_Uint16T::enter()
{
	if (obj != nullptr)		obj->enterL();
	// тут надо использовать функцию для аварийного выхода
	else						Main_menu();
}