/* 
* menu.h
*
* Created: 17.10.2016 15:41:32
* Author: GumerovMN
*/


#ifndef __MENU_H__
#define __MENU_H__

#include "config.h"

#ifdef CONF_MENU

namespace ns_menu
{
	void Init();
	void Main();
	void Interrupt();
}; //menu

#endif // CONF_MENU
#endif //__MENU_H__
