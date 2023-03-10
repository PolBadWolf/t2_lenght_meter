/* 
* MenuEdit_Uint8T.cpp
*
* Created: 10.03.2023 14:06:05
* Author: Gumerovmn
*/


#include "MenuEdit_Uint8T.h"
#include "../../device.h"
// *********** режимы меню **************************
#include "menu_mode.h"

namespace ns_menu
{
	extern	uint16_t	timeOut;
 	extern	uint8_t		mode;
}

#define  mode	ns_menu::mode
#define  key4	ns_device::key
#define  scr	ns_device::scr

static MenuEdit_Uint8T		*obj;

// default constructor
MenuEdit_Uint8T::MenuEdit_Uint8T()
{
	obj = this;
} //MenuEdit_Uint8T

// default destructor
// MenuEdit_Uint8T::~MenuEdit_Uint8T()
// {
// } //~MenuEdit_Uint8T

void MenuEdit_Uint8T::Init(uint8_t datE, uint8_t minE, uint8_t maxE, char *strHead, char *string, CallBackEndEdit_u8 callBack)
{
	dat = datE;
	min = minE;
	max = maxE;
	endEdit = callBack;
	//
	mode = MODE_EDIT_UINT8T;
	key4->setAutoRepeatOff();
	scr->Clear();
	scr->pos = scr->SetPosition(0, 0);
	scr->String_P(strHead);
	scr->String_P(PSTR(" :"));
	// 		scr->String_P(scr->SetPosition(0, 1), PSTR("    м—ек."));
	scr->String_P(scr->SetPosition(4, 1), string);
	viewL();
	CRITICAL_SECTION { ns_menu::timeOut = TIMEOUT_EDIT; }
}

void MenuEdit_Uint8T::viewL()
{
	scr->Digit(scr->SetPosition(0, 1), 3, dat);
}

void MenuEdit_Uint8T::minusL()
{
		key4->setAutoRepeatOn();
		CRITICAL_SECTION { ns_menu::timeOut = TIMEOUT_EDIT; }
		if (dat > min)	dat--;
		else			dat = min;
		scr->Hex(scr->SetPosition(12, 1), dat);
		scr->Hex(scr->SetPosition(14, 1), min);
		viewL();
}

void MenuEdit_Uint8T::plusL()
{
	key4->setAutoRepeatOn();
	CRITICAL_SECTION { ns_menu::timeOut = TIMEOUT_EDIT; }
	if (dat < max)	dat++;
	else			dat = max;
	viewL();
}

void MenuEdit_Uint8T::enterL()
{
	key4->setAutoRepeatOff();
	CRITICAL_SECTION { ns_menu::timeOut = 0; }
	if (endEdit != nullptr)		endEdit(dat);
	endEdit = nullptr;
}

void MenuEdit_Uint8T::minus() { obj->minusL(); }
void MenuEdit_Uint8T::plus()  { obj->plusL (); }
void MenuEdit_Uint8T::enter() { obj->enterL(); }
