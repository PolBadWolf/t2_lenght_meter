/* 
* RingBuffer.h
*
* Created: 14.03.2023 8:05:15
* Author: Gumerovmn
*/


#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include "path/path.h"

class RingBuffer
{
//variables
public:
protected:
	volatile	uint8_t		writeOffSet;
	volatile	uint8_t		read_OffSet;
	uint8_t		size;
	uint8_t		*buffer;
private:

//functions
public:
	RingBuffer(uint8_t	*buffer, uint8_t size);
	~RingBuffer();
	bool		push(uint8_t dat);
	bool		pop(uint8_t *dat);
protected:
	inline	uint8_t		getNextIndex(uint8_t index);
private:
	RingBuffer( const RingBuffer &c );
	RingBuffer& operator=( const RingBuffer &c );

}; //RingBuffer

#endif //__RINGBUFFER_H__
