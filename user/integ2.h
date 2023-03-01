/*
 * integ2.h
 *
 * Created: 28.02.2023 15:07:21
 *  Author: Gumerovmn
 */ 


#ifndef INTEG2_H_
#define INTEG2_H_

typedef void(*callBack)(uint8_t nSensor, bool onOff);

template <class ST>
class integ2
{
	// var
	private:
	uint8_t		nSensor;
	callBack	xx;
	// fn
	public:
	integ2(callBack yy, uint8_t n)
	{
		xx = yy;
		xx(n, false);
	}
};



#endif /* INTEG2_H_ */