/*
 * menu_tab_fn.h
 *
 * Created: 28.02.2023 13:31:10
 *  Author: Gumerovmn
 */ 


#ifndef MENU_TAB_FN_H_
#define MENU_TAB_FN_H_

// ============================================
// 0 : no key (view)
// 1-4 : main key
// 5 : add key
// 6 : timeout
const TyFnMn MassFnMn[][8] PROGMEM =
{//					view				key1				key2				key3				key4				key5			 setmode			 timeout
	{	       Main_View,		   Main_menu,				Dupm,				Dupm,				Dupm,				Dupm,       Main_SetMode,				Dupm },	//	0 main
	{	  EnterPass_View,	  EnterPass_Back,	 EnterPass_Minus,	  EnterPass_Plus,	 EnterPass_Enter,				Dupm,	   EnterPass_Set,		Main_SetMode }, //	1 enter to menu
	{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,		Main_SetMode }, //	2 delay, go main
	{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,	 SelectParam_Set }, //	3 delay, go menu
	{				Dupm,		Main_SetMode,  SelectParam_Minus,	SelectParam_Plus,  SelectParam_Enter,				Dupm,	 SelectParam_Set,		Main_SetMode }, //	4 выбор параметра в меню
	{				Dupm,	 SelectParam_Set,	  EditMsecU8_Mns,	  EditMsecU8_Pls,	  EditMsecU8_Ent,				Dupm,	  EditMsecU8_Set,	 SelectParam_Set }, //  5 MODE_EDIT_MSEC_U8
	{				Dupm,	 SelectParam_Set,	 EditMsecU16_Mns,	 EditMsecU16_Pls,	 EditMsecU16_Ent,				Dupm,	 EditMsecU16_Set,	 SelectParam_Set }, //  6 MODE_EDIT_MSEC_U16
	{				Dupm,	 SelectParam_Set,	EditUmSecU16_Mns,	EditUmSecU16_Pls,	EditUmSecU16_Ent,				Dupm,	EditUmSecU16_Set,	 SelectParam_Set }, //  7 MODE_EDIT_UMSEC_U16
	{				Dupm,	 SelectParam_Set,	  EditUnitU8_Mns,	  EditUnitU8_Pls,	  EditUnitU8_Ent,				Dupm,	  EditUnitU8_Set,	 SelectParam_Set }, //  8 MODE_EDIT_UNIT_U8
	{		 Reboot_None,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,		  Reboot_Set,				Dupm }, //  9 перезагрузка
	{				Dupm,		 SetPass_Bck,		 SetPass_Mns,		 SetPass_Pls,		 SetPass_Ent,				Dupm,		 SetPass_Set,				Dupm }, // 10 установка пароля
	{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,		 Timeout_Set,	 SelectParam_Set },
	{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm },
	{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm },
	{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm },
	{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm },
	{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm },
	{				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm,				Dupm }
};


#endif /* MENU_TAB_FN_H_ */