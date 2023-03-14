/* 
* StekTube.cpp
*
* Created: 06.03.2023 13:15:15
* Author: Gumerovmn
*/


#include "StekTube.h"

StekTube *StekTube::obj;

// default constructor
StekTube::StekTube(unsigned char nTubes)
{
	obj = this;
	tubes = StekTubeUnit::init(nTubes);
	countTubesMax = nTubes;
	countTubesCycle = 0;
	countTubes = 0;
} //StekTube

// default destructor
// StekTube::~StekTube()
// {
// } //~StekTube

void StekTube::newTubeLocal(unsigned int lenghtTube, unsigned char count)
{
	// сдвиг
	tubes[countTubesCycle].lenght = lenghtTube;
	tubes[countTubesCycle].n = count;
	if (++countTubesCycle >= countTubesMax)		countTubesCycle = 0;
	if (countTubes < countTubesMax)		countTubes++;
}

signed char StekTube::normIndxMass(signed char n)
{
	// нормализация
	while (n < 0)		n += countTubesMax;
	while (n >= countTubesMax)	n -= countTubesMax;
	return n;
}

StekTubeUnit StekTube::getLenghtTube(signed char nTube)
{
 	return	tubes[normIndxMass(nTube)];
}

void StekTube::newTube(unsigned int lenghtTube, unsigned char count)
{
	obj->newTubeLocal(lenghtTube, count);
}

unsigned char StekTube::getCountSteckFull()
{
	unsigned char nt = countTubes;
	return nt;
}

unsigned char StekTube::getCountSteckCurrent()
{
	return countTubesCycle;
}

unsigned char StekTube::getCountSteckMax()
{
	return countTubesMax;
}