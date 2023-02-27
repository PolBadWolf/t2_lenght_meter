/*
 * config.h
 *
 * Created: 08.02.2023 14:49:20
 *  Author: Gumerovmn
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

// #ifdef F_CPU
// #undef F_CPU
// #endif // F_CPU
// #define F_CPU 8000000

#define CONF_LCD
//#define CONF_LCD_24

#define CONF_KEY4
// #define CONF_MENU
// 
// #if !defined(CONF_LCD) or !defined(CONF_KEY4)
// #undef CONF_MENU
// #endif

#if defined(CONF_LCD) or defined(CONF_KEY4)
#define CONF_TIMER_LCDKEY
#endif

#if !defined(CONF_LCD) or !defined(CONF_KEY4)
#undef CONF_MENU
#endif

#endif /* CONFIG_H_ */