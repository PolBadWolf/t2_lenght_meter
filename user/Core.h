/* 
* Core.h
*
* Created: 06.03.2023 9:51:05
* Author: Gumerovmn
*/


#ifndef __CORE_H__
#define __CORE_H__

#define CORESTAT_NONE	0
#define CORESTAT_ERROR	1
#define CORESTAT_OK		2

typedef void(*NewTubeCallBack)(unsigned int lenghtTube, unsigned char count);

class Core
{
//variables
public:
	unsigned char	newData;
protected:
	signed char		stat;
	NewTubeCallBack	newTubeCallBack;
	unsigned int	currentLenghtTube;
	unsigned char	count99;
private:

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
