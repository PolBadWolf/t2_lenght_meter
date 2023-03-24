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
{//					view				key1					key2					key3					 key4				key5			 setmode			 timeout
	{	       Main_View,		   Main_menu,					Dupm,					Dupm,					  Dupm,				Dupm,       Main_SetMode,				Dupm },	//	0 main
	{				Dupm,		Main_SetMode,		seclectScr_minus,		 seclectScr_plus,			seclectScr_ent,				Dupm, seclectScr_SetMode,		Main_SetMode }, //  1 меню выбора
	{	  steckTube_view,		Main_SetMode,		 steckTube_minus,		  steckTube_plus,					  Dupm,				Dupm,  steckTube_SetMode,		Main_SetMode }, //  2 просмотр стека труб
	{				Dupm,	  EnterPass_Back,		 EnterPass_Minus,		  EnterPass_Plus,		   EnterPass_Enter,				Dupm,	   EnterPass_Set,		Main_SetMode }, //  3 ввод пароля
	{				Dupm,				Dupm,					Dupm,					Dupm,					  Dupm,				Dupm,				Dupm,		Main_SetMode }, //  4 delay, go main
	{				Dupm,				Dupm,					Dupm,					Dupm,					  Dupm,				Dupm,				Dupm,	 SelectParam_Set }, //  5 delay, go menu
	{				Dupm, seclectScr_SetMode,	   SelectParam_Minus,		SelectParam_Plus,		 SelectParam_Enter,				Dupm,	 SelectParam_Set,		Main_SetMode }, //  6 выбор параметра в меню
	{				Dupm,	 SelectParam_Set, MenuEdit_Uint8T::minus,  MenuEdit_Uint8T::plus,	MenuEdit_Uint8T::enter,				Dupm,				Dupm,	 SelectParam_Set }, //  7 редактирование параметров мерность : uint8_t
	{				Dupm,	 SelectParam_Set,MenuEdit_Uint16T::minus, MenuEdit_Uint16T::plus,  MenuEdit_Uint16T::enter,				Dupm,				Dupm,	 SelectParam_Set }, //  8 редактирование параметров мерность : uint16_t
	{				Dupm,		 SetPass_Bck,			 SetPass_Mns,			 SetPass_Pls,			   SetPass_Ent,				Dupm,		 SetPass_Set,				Dupm }, //  9 установка пароля
	{				Dupm,				Dupm,					Dupm,					Dupm,					  Dupm,				Dupm,		 Timeout_Set,	 SelectParam_Set }, // 10 timeOut
	{showLastTiming_view,		Main_SetMode,					Dupm,					Dupm,					  Dupm,				Dupm, showLastTiming_set,				Dupm }, // 11 show last timing

	{				Dupm,				Dupm,					Dupm,					Dupm,					  Dupm,				Dupm,				Dupm,				Dupm },
	{				Dupm,				Dupm,					Dupm,					Dupm,					  Dupm,				Dupm,				Dupm,				Dupm },
	{				Dupm,				Dupm,					Dupm,					Dupm,					  Dupm,				Dupm,				Dupm,				Dupm },
	{				Dupm,				Dupm,					Dupm,					Dupm,					  Dupm,				Dupm,				Dupm,				Dupm },
	{				Dupm,				Dupm,					Dupm,					Dupm,					  Dupm,				Dupm,				Dupm,				Dupm },
	{				Dupm,				Dupm,					Dupm,					Dupm,					  Dupm,				Dupm,				Dupm,				Dupm },
	{				Dupm,				Dupm,					Dupm,					Dupm,					  Dupm,				Dupm,				Dupm,				Dupm }
};


#endif /* MENU_TAB_FN_H_ */