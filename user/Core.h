/* 
* Core.h
*
* Created: 06.03.2023 9:51:05
* Author: Gumerovmn
*/


#ifndef __CORE_H__
#define __CORE_H__

#define CORESTAT_NONE	0
#define CORESTAT_IZM	1
#define CORESTAT_BLOCK	2
#define CORESTAT_ERRSN	3
#define CORESTAT_ERRIZ	4
#define CORESTAT_OK		5

typedef void(*NewTubeCallBack)(unsigned int lenghtTube, unsigned char count);

#include "SendRS.h"

class Core
{
//variables
public:
	unsigned char	newData;
	// current timing
	int32_t			lastTiming[3];
	uint8_t			lastAlgoritm;
protected:
	signed char		stat;
	NewTubeCallBack	newTubeCallBack;
	unsigned int	currentLenghtTube;
	unsigned char	count99;
private:
	SendRS			sendRS;
	bool			trigMessError;
	//
	bool			flagBlockIzm;

//functions
public:
	Core(NewTubeCallBack newTubeCallBack);
//	~Core();
	void			mainCycle();
	signed char		getStat();
	void			resetStat();
	unsigned int	getCurrentLenghtTube();
	unsigned char	getCurrentCount();
protected:
private:
	Core( const Core &c );
	Core& operator=( const Core &c );

}; //Core

#endif //__CORE_H__
