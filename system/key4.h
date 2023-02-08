/* 
* key4.h
*
* Created: 13.10.2016 15:08:42
* Author: GumerovMN
*/

#ifndef __PATH_H_
#include "path.h"
#endif

#ifndef __KEY4_H__
#define __KEY4_H__

#define KEY4_KEY1 1
#define KEY4_KEY2 2
#define KEY4_KEY3 4
#define KEY4_KEY4 8
class tc_key4
{
//variables
public:
	unsigned char keyX;
protected:
	bool autoRepeat;
	bool autoRepeatTrue;
private:
	integ<unsigned char>* line1;
	integ<unsigned char>* line2;
	integ<unsigned char>* line3;
	integ<unsigned char>* line4;
	unsigned int  countL1;
	unsigned char oldKey1;
	unsigned char oldKey2;
	unsigned int  countL2;
	uint16_t countAut2;
	unsigned char xx;
	unsigned char* buffer;
	unsigned char head;
	unsigned char tail;
//functions
public:
	tc_key4();
	~tc_key4();
	void Interrupt();
	unsigned char InKey();
	unsigned char Read(unsigned char* key);
	void setAutoRepeatOn();
	void setAutoRepeatOff();
protected:
private:
	//tc_key4( const tc_key4 &c );
	//tc_key4& operator=( const tc_key4 &c );
	//void static (* const FN[][2])(unsigned char par);
	void static (* const FN[])(unsigned char par);
	unsigned char norm(unsigned char idx);
	void Push(unsigned char dat);
	void Select();
	unsigned char Nbit(unsigned char dat);
}; //key4

#endif //__KEY4_H__
