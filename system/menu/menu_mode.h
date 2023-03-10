/*
 * menu_mode.h
 *
 * Created: 09.03.2023 15:43:03
 *  Author: Gumerovmn
 */ 


#ifndef MENU_MODE_H_
#define MENU_MODE_H_

// *********** режимы меню **************************
#define MODE_MAIN				0				// главный экран
#define MODE_SELECT_SCR			1				// выбор экрана отображения или настройки
#define MODE_STECK_TUBE			2				// просмотр стека труб
#define MODE_ENTER_PASS			3				// ввод пароля
#define MODE_TIMEOUT_TO_MAIN	4				// delay, go main
#define MODE_TIMEOUT_TO_MENU	5				// delay, go menu
#define MODE_SELECT_PARAM		6				// выбор параметра в меню
#define MODE_EDIT_UINT8T		7				// редактирование параметров мерность : uint8_t
#define MODE_EDIT_UINT16T		8				// редактирование параметров мерность : uint16_t


// #define MODE_EDIT_UMSEC_U16		7				// редактирование параметров в unit/mSec  uint16_t
// #define MODE_EDIT_UNIT_U8		8				// редактирование параметров в разах uint8_t
// #define MODE_REBOOT				9				// перезагрузка
// #define MODE_SET_PASS			10				// установка пароля
// #define MODE_TIMEOUT			11

// ==================================================
#define TIMEOUT_ENTER_PASS		30000
#define TIMEOUT_EDIT			30000



#endif /* MENU_MODE_H_ */
