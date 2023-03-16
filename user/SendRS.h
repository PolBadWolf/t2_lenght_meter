/* 
* SendRS.h
*
* Created: 13.03.2023 13:09:12
* Author: Gumerovmn
*/


#ifndef __SENDRS_H__
#define __SENDRS_H__

#include "path/path.h"

class SendRS
{
//variables
public:
protected:
private:

//functions
public:
	SendRS();
// 	~SendRS();
//
	void	SendLenght(int8_t nTubes, uint16_t		lenght);
	void	SendErrorSensor();
	void	SendErrorRender(uint16_t lenght);
	void	SendChangeBlock(uint16_t lenght);
protected:
private:
	SendRS( const SendRS &c );
	SendRS& operator=( const SendRS &c );

}; //SendRS

#endif //__SENDRS_H__
