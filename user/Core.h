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

typedef void(*NewTubeCallBack)(unsigned int lenghtTube);

class Core
{
//variables
public:
protected:
	signed char		stat;
	NewTubeCallBack	newTubeCallBack;
	unsigned int	currentLenghtTube;
private:

//functions
public:
	Core(NewTubeCallBack newTubeCallBack);
//	~Core();
	void	mainCycle();
	signed char	getStat();
	void	resetStat();
	unsigned int getCurrentLenghtTube();
protected:
private:
	Core( const Core &c );
	Core& operator=( const Core &c );

}; //Core

#endif //__CORE_H__
