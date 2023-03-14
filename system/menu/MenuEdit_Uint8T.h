/* 
* MenuEdit_Uint8T.h
*
* Created: 10.03.2023 14:06:06
* Author: Gumerovmn
*/


#ifndef __MENUEDIT_UINT8T_H__
#define __MENUEDIT_UINT8T_H__

#include "path/path.h"
typedef void(*CallBackEndEdit_u8)(uint8_t dat);

class MenuEdit_Uint8T
{
//variables
public:
	uint8_t				dat;
	uint8_t				min;
	uint8_t				max;
protected:
	CallBackEndEdit_u8	endEdit;
	static MenuEdit_Uint8T		*obj;
private:

//functions
public:
	MenuEdit_Uint8T();
// 	~MenuEdit_Uint8T();
	void init(uint8_t dat, uint8_t min, uint8_t max, char *strHead, char *string, CallBackEndEdit_u8 callBack);
	static void minus();
	static void plus();
	static void enter();
protected:
private:
	MenuEdit_Uint8T( const MenuEdit_Uint8T &c );
	MenuEdit_Uint8T& operator=( const MenuEdit_Uint8T &c );
	void viewL();
	void minusL();
	void plusL();
	void enterL();

}; //MenuEdit_Uint8T

#endif //__MENUEDIT_UINT8T_H__
