/* 
* Sensor.h
*
* Created: 01.03.2023 12:54:23
* Author: Gumerovmn
*/


#ifndef __SENSOR_H__
#define __SENSOR_H__

typedef void(*SensorCallBack)(unsigned char nSensor, bool stat);

class Sensor
{
//variables
public:
	bool						stat;
	bool						statInv;
protected:
	volatile unsigned char*		pin;
	unsigned char				mask;
	unsigned char				n_sensor;
	SensorCallBack				callBack;
	unsigned char				count;
	unsigned char				countMax;
	bool						triger;
private:

//functions
public:
	Sensor(unsigned char n_sensor, volatile unsigned char* s_ddr, volatile unsigned char* s_port, volatile unsigned char* s_pin, unsigned char nBit, SensorCallBack callBack, bool inv);
	~Sensor();
	void interrupt();
	void setCountMax(unsigned char max) { countMax = max; }
	unsigned char getCountMax()			{ return countMax; }
protected:
private:
	Sensor( const Sensor &c );
	Sensor& operator=( const Sensor &c );

}; //Sensor

#endif //__SENSOR_H__
