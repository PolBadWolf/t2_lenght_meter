/* 
* StekTube.cpp
*
* Created: 06.03.2023 13:15:15
* Author: Gumerovmn
*/


#include "StekTube.h"

static StekTube *obj;

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

void StekTube::newTubeLocal(unsigned int lenghtTube)
{
	// сдвиг
	tubes[countTubesCycle].lenght = lenghtTube;
	if (++countTubesCycle == countTubesMax)		countTubesCycle = 0;
	if (countTubes < countTubesMax)		countTubes++;
}

signed int StekTube::getLenghtTube(unsigned char nTube)
{
	if (nTube > countTubes)		return -1;
	unsigned char i;
// 	i = nTube + countTubesCycle - 1;
	i = nTube  - 1;
	if (i >= countTubesMax)		i -= countTubesMax;
	return	tubes[i].lenght;
}

void StekTube::newTube(unsigned int lenghtTube)
{
	obj->newTubeLocal(lenghtTube);
}

unsigned char StekTube::getCountSteck()
{
	unsigned char nt = countTubes;
	return nt;
}