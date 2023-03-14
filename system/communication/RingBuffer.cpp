/* 
* RingBuffer.cpp
*
* Created: 14.03.2023 8:05:15
* Author: Gumerovmn
*/


#include "RingBuffer.h"
// #include "IncFile1.h"

// default constructor
RingBuffer::RingBuffer(uint8_t		*buffer, uint8_t size)
{
	this->buffer = buffer;
	this->size = size;
	read_OffSet = writeOffSet = 0;
} //RingBuffer

// default destructor
RingBuffer::~RingBuffer()
{
	delete[]	buffer;
} //~RingBuffer

uint8_t	RingBuffer::getNextIndex(uint8_t index)
{
	if (++index >= size) index = 0;
	return index;
}

bool RingBuffer::push(uint8_t dat)
{
	uint8_t nextIndx = getNextIndex(writeOffSet);
	if (nextIndx == read_OffSet)	return	true;
	buffer[writeOffSet] = dat;
	writeOffSet = nextIndx;
	return false;
}

bool RingBuffer::pop(uint8_t *dat)
{
	if (read_OffSet == writeOffSet)	return	true;
	*dat = buffer[read_OffSet];
	read_OffSet = getNextIndex(read_OffSet);
	return	false;
}