/* 
* DeB.cpp
*
* Created: 07.03.2023 14:16:29
* Author: Gumerovmn
*/


#include "DeB.h"

// default constructor
DeB::DeB()
{
} //DeB

// default destructor
DeB::~DeB()
{
} //~DeB

void DeB::main()
{
	StekTube *steck = new StekTube(3);
	
	steck->tubes[0] = *(new StekTubeUnit(11, 7));
	steck->tubes[1] = *(new StekTubeUnit(22, 8));
	steck->tubes[2] = *(new StekTubeUnit(33, 9));
	
	x = steck->getCountSteckCurrent();
	steck->newTube( 9500, 1);
	x = steck->getCountSteckCurrent();
	{
		unit = steck->getLenghtTube(x);
		units[0].lenght = unit.lenght;
		units[0].n = unit.n;
	}
	steck->newTube(10000, 2);
	x = steck->getCountSteckCurrent();
	steck->newTube(10500, 3);
	x = steck->getCountSteckCurrent();
	steck->newTube(11000, 4);
	x = steck->getCountSteckCurrent();
	steck->newTube(11500, 5);
	x = steck->getCountSteckCurrent();
	steck->newTube(11506, 6);
	x = steck->getCountSteckCurrent();
	steck->newTube(11507, 7);
	x = steck->getCountSteckCurrent();
	steck->newTube(11508, 8);
	x = steck->getCountSteckCurrent();
}