/* 
* StekTube.h
*
* Created: 06.03.2023 13:15:16
* Author: Gumerovmn
*/


#ifndef __STEKTUBE_H__
#define __STEKTUBE_H__

class StekTubeUnit
{
public:
 	unsigned	char	n;
	signed		int		lenght;
	static StekTubeUnit* init(unsigned char nTubes)
	{
		StekTubeUnit *tubes = new StekTubeUnit[nTubes];
// 		for (unsigned char i = 0; i< nTubes; i++)
// 		{
// 			tubes[i] = new StekTubeUnit();
// 		}
		return tubes;
	}
	StekTubeUnit()
	{
		lenght = 0;
		n = 0;
	}
	StekTubeUnit(unsigned int lenght, unsigned char n)
	{
		this->lenght	= lenght;
		this->n			= n;
	}
protected:
private:
};

class StekTube
{
//variables
public:
	StekTubeUnit	*tubes;
protected:
// 	StekTubeUnit	*tubes;
	unsigned char	countTubesMax;
	unsigned char	countTubes;
	unsigned char	countTubesCycle;
private:

//functions
public:
	StekTube(unsigned char nTubes);
// 	~StekTube();
	void newTubeLocal(unsigned int lenghtTube, unsigned char count);
	static void newTube(unsigned int lenghtTube, unsigned char count);
	unsigned char getCountSteckFull();
	unsigned char getCountSteckCurrent();
	unsigned char getCountSteckMax();
	StekTubeUnit  getLenghtTube(signed char nTube);
protected:
private:
	StekTube( const StekTube &c );
	StekTube& operator=( const StekTube &c );

}; //StekTube

#endif //__STEKTUBE_H__
