/*
 * StekTubeUnit.h
 *
 * Created: 13.03.2023 7:35:31
 *  Author: Gumerovmn
 */ 


#ifndef STEKTUBEUNIT_H_
#define STEKTUBEUNIT_H_

class StekTubeUnit
{
	public:
	unsigned	char	n;
	signed		int		lenght;
	static StekTubeUnit* init(unsigned char nTubes)
	{
		StekTubeUnit *tubes = new StekTubeUnit[nTubes];
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



#endif /* STEKTUBEUNIT_H_ */