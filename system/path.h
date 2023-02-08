/*
 * path.h
 *
 * Created: 27.08.2016 19:56:54
 *  Author: User
 */ 

#ifndef __PATH_H_
#define __PATH_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
//#include <util/delay.h>
#include <stddef.h>
#include <stdio.h>

#define __port(x) (*((volatile BYTE_BITFIELD*)(&x)))

#if defined (__AVR_ATmega16__)
#include "path_mega16.h"
#elif defined (__AVR_ATmega128__)
#include "path_mega128.h"
#else
#error not user path cpu
#endif

typedef struct
{
	unsigned bit0:1;
	unsigned bit1:1;
	unsigned bit2:1;
	unsigned bit3:1;
	unsigned bit4:1;
	unsigned bit5:1;
	unsigned bit6:1;
	unsigned bit7:1;
} BYTE_BITFIELD;

typedef struct
{
	unsigned Low:4;
	unsigned High:4;
} BYTE_TETRA;

typedef struct
{
	unsigned Low:8;
	unsigned High:8;
} DOUBLE_BYTE;

typedef struct
{
	unsigned Byte1:8;
	unsigned Byte2:8;
	unsigned Byte3:8;
	unsigned Byte4:8;
} TETRA_BYTE;

#ifndef __MY_DELAY_
#define __MY_DELAY_
static __inline__ void _my_delay(uint32_t __count) __attribute__((__always_inline__));
void _my_delay(uint32_t __count)
{
	__asm__ volatile (
	"1: subi %A0,1" "\n\t"
	"sbci %B0,0" "\n\t"
	"sbci %C0,0" "\n\t"
	"sbci %D0,0" "\n\t"
	"brne 1b"
	: "=r" (__count)
	: "0" (__count)
	);
}
#endif

// большие проблемы без оптимизации, да еще и не предупреждает об этом
#ifndef __OPTIMIZE__
//# warning "Compiler optimizations disabled; functions delay won't work as designed"
#define __delay_ms(__ms) _my_delay((F_CPU / 127400L) * __ms)
#define __delay_us(__us) __builtin_avr_delay_cycles((F_CPU / 1000000L) * __us)
#else

#define __delay_ms2(__ms) _my_delay((F_CPU / 16e3) * __ms)
#define __delay_ms(__ms) __builtin_avr_delay_cycles((F_CPU / 2e3) * __ms)
#define __delay_us(__us) __builtin_avr_delay_cycles((F_CPU / 1000000L) * __us)
#endif

#define  CRITICAL_SECTION ATOMIC_BLOCK(ATOMIC_RESTORESTATE)

class Mutex_Block
{
	private:
	uint8_t	save_sreg;
	public:
	Mutex_Block()
	{
		save_sreg = SREG;
		cli();
	}
	~Mutex_Block()
	{
		SREG = save_sreg;
	}
};

void *operator new(size_t);
void *operator new[](size_t);

void operator delete(void *);
void operator delete[](void *);
//*****************************
template <class ST>
class integ
{
	// Var
public:
	bool tr;
//protected:
//private:
	ST count;
	ST countMax;
	bool tr_int;
	// Fn
public:
	void SetMax(ST max)
	{
		CRITICAL_SECTION
		{
			countMax = max;
			if (count>countMax)
			{
				count = countMax;
				tr = tr_int = true;
			}
		}
	}
	integ()
	{
		count = (ST)0;
		countMax = (ST)40;
		tr = tr_int = false;
	}
	integ(ST max)
	{
		count = (ST)0;
		countMax = max;
		tr = tr_int = false;
	}
	unsigned char Cirle(unsigned char s)
	{
		CRITICAL_SECTION
		{
			if (s)
			{
				if (count>(ST)0) count--;
				if ((count==(ST)0) && tr_int)
				tr = tr_int = false;
			}
			else
			{
				if (count<countMax) count++;
				if ((count>=countMax) && !tr_int)
				{
					tr = tr_int = true;
					count = countMax;
				}
			}
			return tr_int;// ? 0 : 0xff;
		}
		return 0;
	}
protected:
private:
};
// ------------------------------------------
enum rsBaud
{
	baud300     = 300
	,baud600    = 600
	,baud1200   = 1200
	,baud2400   = 2400
	,baud4800   = 4800
	,baud9600   = 9600
	,baud19200  = 19200
	,baud38400  = 38400
	,baud57600  = 57600
	,baud115200 = 115200
	,baud250000 = 250000
};
enum rsParity
{
	DISABLE = 0,
	EVEN = 2,
	ODD = 3
};
enum rsBit
{
	bit5 = 0,
	bit6,
	bit7,
	bit8
};
enum rsBufferSize
{
	size2	= 2,
	size4	= 4,
	size8	= 8,
	size16	= 16,
	size32	= 32,
	size64	= 64,
	size128	= 128,
	size256	= 256
};
extern const char tabWin2Avr[];
#define tabWin2AvrOffset 128
// ------------------------------------------
//						sample
//		TypeFunct2 *FnLock = (TypeFunct2 *)&FnMass;
//		CallFunct2(FnLock, 1, 5);
typedef void(*TypeFunct1)();
typedef void(*TypeFunct2)(uint16_t *dat);
typedef void(*TypeFunct3)(uint16_t dat, uint8_t indx);
typedef void(*AdcCoreCallBack)(uint16_t measurement);
#define CallFunct1(Fn, indx) ((TypeFunct1)pgm_read_ptr(& Fn[indx]))()
#define CallFunct2(Fn, indx, dat) ((TypeFunct2)pgm_read_ptr(&Fn[indx]))(dat)
#define CallFunct3(Fn, indx, dat) ((TypeFunct3)pgm_read_ptr(Fn))(dat, indx)
//
#define CallMetod1(Type, Fn, data) ((Type)pgm_read_ptr(Fn))(data)
#define CallMetod2(Type, Fn, indx, data) ((Type)pgm_read_ptr(Fn))(data, indx)
//*****************************
#include "../device.h"
//#include "../Module.h"
#include "../vg.h"

#endif /* PATH_H_ */
