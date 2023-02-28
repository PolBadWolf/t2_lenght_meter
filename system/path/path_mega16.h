/*
 * path_mega16.h
 *
 * Created: 19.09.2017 14:42:42
 *  Author: GumerovMN
 */ 

// #ifndef __PATH_H_
// #include "path.h"
// #endif


#ifndef PATH_MEGA16_H_
#define PATH_MEGA16_H_

// SREG [0x3F]
#define SREG      _SFR_IO8(0x3F)


/* Registers and associated bit numbers */

#define TWSR_TWPS0		__port(TWSR).bit0
#define TWSR_TWPS1		__port(TWSR).bit1
#define TWSR_TWS3		__port(TWSR).bit3
#define TWSR_TWS4		__port(TWSR).bit4
#define TWSR_TWS5		__port(TWSR).bit5
#define TWSR_TWS6		__port(TWSR).bit6
#define TWSR_TWS7		__port(TWSR).bit7

#define TWAR_TWGCE		__port(TWAR).bit0
#define TWAR_TWA0		__port(TWAR).bit1
#define TWAR_TWA1		__port(TWAR).bit2
#define TWAR_TWA2		__port(TWAR).bit3
#define TWAR_TWA3		__port(TWAR).bit4
#define TWAR_TWA4		__port(TWAR).bit5
#define TWAR_TWA5		__port(TWAR).bit6
#define TWAR_TWA6		__port(TWAR).bit7

#define ADCSRA_ADPS0	__port(ADCSRA).bit0
#define ADCSRA_ADPS1	__port(ADCSRA).bit1
#define ADCSRA_ADPS2	__port(ADCSRA).bit2
#define ADCSRA_ADIE		__port(ADCSRA).bit3
#define ADCSRA_ADIF		__port(ADCSRA).bit4
#define ADCSRA_ADATE	__port(ADCSRA).bit5
#define ADCSRA_ADSC		__port(ADCSRA).bit6
#define ADCSRA_ADEN		__port(ADCSRA).bit7

#define ADMUX_MUX0		__port(ADMUX).bit0
#define ADMUX_MUX1		__port(ADMUX).bit1
#define ADMUX_MUX2		__port(ADMUX).bit2
#define ADMUX_MUX3		__port(ADMUX).bit3
//#define ADMUX_MUX4		__port(ADMUX).bit4
#define ADMUX_ADLAR		__port(ADMUX).bit5
#define ADMUX_REFS0		__port(ADMUX).bit6
#define ADMUX_REFS1		__port(ADMUX).bit7

#define ACSR_ACIS0		__port(ACSR).bit0
#define ACSR_ACIS1		__port(ACSR).bit1
#define ACSR_ACIC		__port(ACSR).bit2
#define ACSR_ACIE		__port(ACSR).bit3
#define ACSR_ACI		__port(ACSR).bit4
#define ACSR_ACO		__port(ACSR).bit5
#define ACSR_ACBG		__port(ACSR).bit6
#define ACSR_ACD		__port(ACSR).bit7

#define UCSRB_TXB8		__port(UCSRB).bit0
#define UCSRB_RXB8		__port(UCSRB).bit1
#define UCSRB_UCSZ2		__port(UCSRB).bit2
#define UCSRB_TXEN		__port(UCSRB).bit3
#define UCSRB_RXEN		__port(UCSRB).bit4
#define UCSRB_UDRIE		__port(UCSRB).bit5
#define UCSRB_TXCIE		__port(UCSRB).bit6
#define UCSRB_RXCIE		__port(UCSRB).bit7

#define UCSRA_MPCM		__port(UCSRA).bit0
#define UCSRA_U2X		__port(UCSRA).bit1
#define UCSRA_PE		__port(UCSRA).bit2
#define UCSRA_DOR		__port(UCSRA).bit3
#define UCSRA_FE		__port(UCSRA).bit4
#define UCSRA_UDRE		__port(UCSRA).bit5
#define UCSRA_TXC		__port(UCSRA).bit6
#define UCSRA_RXC		__port(UCSRA).bit7

#define SPCR_SPR0		__port(SPCR).bit0
#define SPCR_SPR1		__port(SPCR).bit1
#define SPCR_CPHA		__port(SPCR).bit2
#define SPCR_CPOL		__port(SPCR).bit3
#define SPCR_MSTR		__port(SPCR).bit4
#define SPCR_DORD		__port(SPCR).bit5
#define SPCR_SPE		__port(SPCR).bit6
#define SPCR_SPIE		__port(SPCR).bit7

#define SPSR_SPI2X		__port(SPSR).bit0
#define SPSR_WCOL		__port(SPSR).bit6
#define SPSR_SPIF		__port(SPSR).bit7

#define PIND_PIND0		__port(PIND).bit0
#define PIND_PIND1		__port(PIND).bit1
#define PIND_PIND2		__port(PIND).bit2
#define PIND_PIND3		__port(PIND).bit3
#define PIND_PIND4		__port(PIND).bit4
#define PIND_PIND5		__port(PIND).bit5
#define PIND_PIND6		__port(PIND).bit6
#define PIND_PIND7		__port(PIND).bit7

#define DDRD_DDD0		__port(DDRD).bit0
#define DDRD_DDD1		__port(DDRD).bit1
#define DDRD_DDD2		__port(DDRD).bit2
#define DDRD_DDD3		__port(DDRD).bit3
#define DDRD_DDD4		__port(DDRD).bit4
#define DDRD_DDD5		__port(DDRD).bit5
#define DDRD_DDD6		__port(DDRD).bit6
#define DDRD_DDD7		__port(DDRD).bit7

#define PORTD_PD0		__port(PORTD).bit0
#define PORTD_PD1		__port(PORTD).bit1
#define PORTD_PD2		__port(PORTD).bit2
#define PORTD_PD3		__port(PORTD).bit3
#define PORTD_PD4		__port(PORTD).bit4
#define PORTD_PD5		__port(PORTD).bit5
#define PORTD_PD6		__port(PORTD).bit6
#define PORTD_PD7		__port(PORTD).bit7

#define PINC_PINC0		__port(PINC).bit0
#define PINC_PINC1		__port(PINC).bit1
#define PINC_PINC2		__port(PINC).bit2
#define PINC_PINC3		__port(PINC).bit3
#define PINC_PINC4		__port(PINC).bit4
#define PINC_PINC5		__port(PINC).bit5
#define PINC_PINC6		__port(PINC).bit6
#define PINC_PINC7		__port(PINC).bit7

#define DDRC_DDC0		__port(DDRC).bit0
#define DDRC_DDC1		__port(DDRC).bit1
#define DDRC_DDC2		__port(DDRC).bit2
#define DDRC_DDC3		__port(DDRC).bit3
#define DDRC_DDC4		__port(DDRC).bit4
#define DDRC_DDC5		__port(DDRC).bit5
#define DDRC_DDC6		__port(DDRC).bit6
#define DDRC_DDC7		__port(DDRC).bit7

#define PORTC_PC0		__port(PORTC).bit0
#define PORTC_PC1		__port(PORTC).bit1
#define PORTC_PC2		__port(PORTC).bit2
#define PORTC_PC3		__port(PORTC).bit3
#define PORTC_PC4		__port(PORTC).bit4
#define PORTC_PC5		__port(PORTC).bit5
#define PORTC_PC6		__port(PORTC).bit6
#define PORTC_PC7		__port(PORTC).bit7

#define PINB_PINB0		__port(PINB).bit0
#define PINB_PINB1		__port(PINB).bit1
#define PINB_PINB2		__port(PINB).bit2
#define PINB_PINB3		__port(PINB).bit3
#define PINB_PINB4		__port(PINB).bit4
#define PINB_PINB5		__port(PINB).bit5
#define PINB_PINB6		__port(PINB).bit6
#define PINB_PINB7		__port(PINB).bit7

#define DDRB_DDB0		__port(DDRB).bit0
#define DDRB_DDB1		__port(DDRB).bit1
#define DDRB_DDB2		__port(DDRB).bit2
#define DDRB_DDB3		__port(DDRB).bit3
#define DDRB_DDB4		__port(DDRB).bit4
#define DDRB_DDB5		__port(DDRB).bit5
#define DDRB_DDB6		__port(DDRB).bit6
#define DDRB_DDB7		__port(DDRB).bit7

#define PORTB_PB0		__port(PORTB).bit0
#define PORTB_PB1		__port(PORTB).bit1
#define PORTB_PB2		__port(PORTB).bit2
#define PORTB_PB3		__port(PORTB).bit3
#define PORTB_PB4		__port(PORTB).bit4
#define PORTB_PB5		__port(PORTB).bit5
#define PORTB_PB6		__port(PORTB).bit6
#define PORTB_PB7		__port(PORTB).bit7

#define PINA_PINA0		__port(PINA).bit0
#define PINA_PINA1		__port(PINA).bit1
#define PINA_PINA2		__port(PINA).bit2
#define PINA_PINA3		__port(PINA).bit3
#define PINA_PINA4		__port(PINA).bit4
#define PINA_PINA5		__port(PINA).bit5
#define PINA_PINA6		__port(PINA).bit6
#define PINA_PINA7		__port(PINA).bit7

#define DDRA_DDA0		__port(DDRA).bit0
#define DDRA_DDA1		__port(DDRA).bit1
#define DDRA_DDA2		__port(DDRA).bit2
#define DDRA_DDA3		__port(DDRA).bit3
#define DDRA_DDA4		__port(DDRA).bit4
#define DDRA_DDA5		__port(DDRA).bit5
#define DDRA_DDA6		__port(DDRA).bit6
#define DDRA_DDA7		__port(DDRA).bit7

#define PORTA_PA0		__port(PORTA).bit0
#define PORTA_PA1		__port(PORTA).bit1
#define PORTA_PA2		__port(PORTA).bit2
#define PORTA_PA3		__port(PORTA).bit3
#define PORTA_PA4		__port(PORTA).bit4
#define PORTA_PA5		__port(PORTA).bit5
#define PORTA_PA6		__port(PORTA).bit6
#define PORTA_PA7		__port(PORTA).bit7

// EEPROM Control Register
#define EECR_EERE		__port(EECR).bit0
#define EECR_EEWE		__port(EECR).bit1
#define EECR_EEMWE		__port(EECR).bit2
#define EECR_EERIE		__port(EECR).bit3

#define UCSRC_UCPOL		__port(UCSRC).bit0
#define UCSRC_UCSZ0		__port(UCSRC).bit1
#define UCSRC_UCSZ1		__port(UCSRC).bit2
#define UCSRC_USBS		__port(UCSRC).bit3
#define UCSRC_UPM0		__port(UCSRC).bit4
#define UCSRC_UPM1		__port(UCSRC).bit5
#define UCSRC_UMSEL		__port(UCSRC).bit6
#define UCSRC_URSEL		__port(UCSRC).bit7

#define UBRRH_URSEL		__port(UBRRH).bit7

#define WDTCR_WDP0		__port(WDTCR).bit0
#define WDTCR_WDP1		__port(WDTCR).bit1
#define WDTCR_WDP2		__port(WDTCR).bit2
#define WDTCR_WDE		__port(WDTCR).bit3
#define WDTCR_WDTOE		__port(WDTCR).bit4

#define ASSR_TCR2UB		__port(ASSR).bit0
#define ASSR_OCR2UB		__port(ASSR).bit1
#define ASSR_TCN2UB		__port(ASSR).bit2
#define ASSR_AS2		__port(ASSR).bit3

#define TCCR2_CS20		__port(TCCR2).bit0
#define TCCR2_CS21		__port(TCCR2).bit1
#define TCCR2_CS22		__port(TCCR2).bit2
#define TCCR2_WGM21		__port(TCCR2).bit3
#define TCCR2_COM20		__port(TCCR2).bit4
#define TCCR2_COM21		__port(TCCR2).bit5
#define TCCR2_WGM20		__port(TCCR2).bit6
#define TCCR2_FOC2		__port(TCCR2).bit7

#define TCCR1B_CS10		__port(TCCR1B).bit0
#define TCCR1B_CS11		__port(TCCR1B).bit1
#define TCCR1B_CS12		__port(TCCR1B).bit2
#define TCCR1B_WGM12	__port(TCCR1B).bit3
#define TCCR1B_WGM13	__port(TCCR1B).bit4
#define TCCR1B_ICES1	__port(TCCR1B).bit6
#define TCCR1B_ICNC1	__port(TCCR1B).bit7

#define TCCR1A_WGM10	__port(TCCR1A).bit0
#define TCCR1A_WGM11	__port(TCCR1A).bit1
#define TCCR1A_FOC1B	__port(TCCR1A).bit2
#define TCCR1A_FOC1A	__port(TCCR1A).bit3
#define TCCR1A_COM1B0	__port(TCCR1A).bit4
#define TCCR1A_COM1B1	__port(TCCR1A).bit5
#define TCCR1A_COM1A0	__port(TCCR1A).bit6
#define TCCR1A_COM1A1	__port(TCCR1A).bit7

/* 
   The ADHSM bit has been removed from all documentation, 
   as being not needed at all since the comparator has proven 
   to be fast enough even without feeding it more power.
*/

#define SFIOR_PSR10		__port(SFIOR).bit0
#define SFIOR_PSR2		__port(SFIOR).bit1
#define SFIOR_PUD		__port(SFIOR).bit2
#define SFIOR_ACME		__port(SFIOR).bit3
#define SFIOR_ADTS0		__port(SFIOR).bit5
#define SFIOR_ADTS1		__port(SFIOR).bit6
#define SFIOR_ADTS2		__port(SFIOR).bit7

//#define TCCR0   _SFR_IO8(0x33)
#define TCCR0_CS00		__port(TCCR0).bit0
#define TCCR0_CS01		__port(TCCR0).bit1
#define TCCR0_CS02		__port(TCCR0).bit2
#define TCCR0_WGM01		__port(TCCR0).bit3
#define TCCR0_COM00		__port(TCCR0).bit4
#define TCCR0_COM01		__port(TCCR0).bit5
#define TCCR0_WGM00		__port(TCCR0).bit6
#define TCCR0_FOC0		__port(TCCR0).bit7

#define MCUCSR_PORF		__port(MCUCSR).bit0
#define MCUCSR_EXTRF	__port(MCUCSR).bit1
#define MCUCSR_BORF		__port(MCUCSR).bit2
#define MCUCSR_WDRF		__port(MCUCSR).bit3
#define MCUCSR_JTRF		__port(MCUCSR).bit4
#define MCUCSR_ISC2		__port(MCUCSR).bit6
#define MCUCSR_JTD		__port(MCUCSR).bit7

#define MCUCR_ISC00		__port(MCUCR).bit0
#define MCUCR_ISC01		__port(MCUCR).bit1
#define MCUCR_ISC10		__port(MCUCR).bit2
#define MCUCR_ISC11		__port(MCUCR).bit3
#define MCUCR_SM0		__port(MCUCR).bit4
#define MCUCR_SM1		__port(MCUCR).bit5
#define MCUCR_SE		__port(MCUCR).bit6
#define MCUCR_SM2		__port(MCUCR).bit7

#define TWCR_TWIE		__port(TWCR).bit0
#define TWCR_TWEN		__port(TWCR).bit2
#define TWCR_TWWC		__port(TWCR).bit3
#define TWCR_TWSTO		__port(TWCR).bit4
#define TWCR_TWSTA		__port(TWCR).bit5
#define TWCR_TWEA		__port(TWCR).bit6
#define TWCR_TWINT		__port(TWCR).bit7

#define SPMCR_SPMEN		__port(SPMCR).bit0
#define SPMCR_PGERS		__port(SPMCR).bit1
#define SPMCR_PGWRT		__port(SPMCR).bit2
#define SPMCR_BLBSET	__port(SPMCR).bit3
#define SPMCR_RWWSRE	__port(SPMCR).bit4
#define SPMCR_RWWSB		__port(SPMCR).bit6
#define SPMCR_SPMIE		__port(SPMCR).bit7

#define TIFR_TOV0		__port(TIFR).bit0
#define TIFR_OCF0		__port(TIFR).bit1
#define TIFR_TOV1		__port(TIFR).bit2
#define TIFR_OCF1B		__port(TIFR).bit3
#define TIFR_OCF1A		__port(TIFR).bit4
#define TIFR_ICF1		__port(TIFR).bit5
#define TIFR_TOV2		__port(TIFR).bit6
#define TIFR_OCF2		__port(TIFR).bit7

#define TIMSK_TOIE0		__port(TIMSK).bit0
#define TIMSK_OCIE0		__port(TIMSK).bit1
#define TIMSK_TOIE1		__port(TIMSK).bit2
#define TIMSK_OCIE1B	__port(TIMSK).bit3
#define TIMSK_OCIE1A	__port(TIMSK).bit4
#define TIMSK_TICIE1	__port(TIMSK).bit5
#define TIMSK_TOIE2		__port(TIMSK).bit6
#define TIMSK_OCIE2		__port(TIMSK).bit7

#define GIFR_INTF2		__port(GIFR).bit5
#define GIFR_INTF0		__port(GIFR).bit6
#define GIFR_INTF1		__port(GIFR).bit7

#define GICR_IVCE		__port(GICR).bit0
#define GICR_IVSEL		__port(GICR).bit1
#define GICR_INT2		__port(GICR).bit5
#define GICR_INT0		__port(GICR).bit6
#define GICR_INT1		__port(GICR).bit7

#endif /* PATH_MEGA16_H_ */