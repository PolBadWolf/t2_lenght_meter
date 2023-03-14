/* 
* StekTube.h
*
* Created: 06.03.2023 13:15:16
* Author: Gumerovmn
*/


#ifndef __STEKTUBE_H__
#define __STEKTUBE_H__

#include "StekTubeUnit.h"

class StekTube
{
//variables
public:
	StekTubeUnit	*tubes;
protected:
	unsigned char	countTubesMax;
	unsigned char	countTubes;
	unsigned char	countTubesCycle;
	static StekTube *obj;
private:

//functions
public:
	StekTube(unsigned char nTubes);
// 	~StekTube();
	void newTubeLocal(unsigned int lenghtTube,   unsigned char count);
	static void newTube(unsigned int lenghtTube, unsigned char count);
	unsigned char getCountSteckFull();
	unsigned char getCountSteckCurrent();
	  signed char normIndxMass(signed char n);
	unsigned char getCountSteckMax();
	StekTubeUnit  getLenghtTube(signed char nTube);
protected:
private:
	StekTube( const StekTube &c );
	StekTube& operator=( const StekTube &c );

}; //StekTube

#endif //__STEKTUBE_H__
