/* 
* DeB.h
*
* Created: 07.03.2023 14:16:29
* Author: Gumerovmn
*/


#ifndef __DEB_H__
#define __DEB_H__

#include "user/StekTube.h"


class DeB
{
//variables
public:
	signed char x;
	StekTubeUnit units[2];
	StekTubeUnit unit;
protected:
private:

//functions
public:
	DeB();
	~DeB();
	void main();
protected:
private:
	DeB( const DeB &c );
	DeB& operator=( const DeB &c );

}; //DeB

#endif //__DEB_H__
