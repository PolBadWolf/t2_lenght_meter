/* 
* MenuEdit_Uint16T.h
*
* Created: 13.03.2023 7:38:42
* Author: Gumerovmn
*/


#ifndef __MENUEDIT_UINT16T_H__
#define __MENUEDIT_UINT16T_H__

#include "path/path.h"
typedef void(*CallBackEndEdit_u16)(uint16_t dat);

class MenuEdit_Uint16T
{
//variables
public:
	uint16_t		min;
	uint16_t		max;
	uint16_t		dat;
protected:
	CallBackEndEdit_u16		endEdit;
	static MenuEdit_Uint16T	*obj;
private:

//functions
public:
	MenuEdit_Uint16T();
// 	~MenuEdit_Uint16T();
	//
	void	init(uint16_t dat, uint16_t min, uint16_t max, char *strHead, char *string, CallBackEndEdit_u16 callBack);
	static	void	minus();
	static	void	plus();
	static	void	enter();
protected:
	void	viewL();
	void	minusL();
	void	plusL();
	void	enterL();
private:
	MenuEdit_Uint16T( const MenuEdit_Uint16T &c );
	MenuEdit_Uint16T& operator=( const MenuEdit_Uint16T &c );
	
}; //MenuEdit_Uint16T

#endif //__MENUEDIT_UINT16T_H__
