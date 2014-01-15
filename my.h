

#ifndef _MY_H_
#define _MY_H_

/* includes ------------------------------------------------------------------- */
 
#ifdef __cplusplus
extern "C" {
#endif

/* constants ------------------------------------------------------------------ */


/* enumerations --------------------------------------------------------------- */


/* types ---------------------------------------------------------------------- */





#define		NOP			_nop_()


//#define dprint(...)  //printf(__VA_ARGS__)



#define MAX_TEMPERATURE 60
#define TEMPERATURE_RATIO  (0xfff/MAX_TEMPERATURE)



/* Center side  FND */
#define FND_10 P24
#define FND_1 P23
#define  FND_LED P16
#define  FND_A  P37     
#define  FND_B  P36
#define  FND_C  P35
#define  FND_D  P34
#define  FND_E  P15
#define  FND_F  P14
#define  FND_G  P13

/* Right side LED */

#define CONTINUOUS_OP_LED FND_A
#define TIME_OP_LED FND_B
#define HOT_SET_LED FND_C
#define COLD_SET_LDE FND_D
#define ERROR_LED FND_E

/* Left side LED */
#define POWER_LED 0
#define HEATER_CONTROL_LED P25
#define PUMP_CONTROL_LED P26
#define COLD_CONTROL_LED P03


/* Bottom side SW */
#define  POWER_SW_PIN P30
#define  HOT_COLD_SW_PIN P31
#define  CONT_OP_SW_PIN P32
#define  TIME_OP_SW_PIN P33
#define  DOWN_SW_PIN P04
#define  UP_SW_PIN P05

/* Other pins */
#define  WATER_SENSE_PIN P12
#define BUZZER_PIN P02


enum input_key
{
	
	KEY_IDLE,
	KEY_POWER,
	KEY_HOT_COLD,
	KEY_CONT_OP,
	KEY_TIME_OP,
	KEY_DOWN,
	KEY_UP
};

enum STATE
{
	POWER_ON,
	POWER_OFF,
	HOT,
	COLD,
	CONTINUOUS_MODE,
	TIMER_MODE,
	IDLE
};

#if 0
typedef struct 
{
	union
	{
		struct {
			unsigned char bit0:1;
			unsigned char bit1:1;
			unsigned char bit2:1;
			unsigned char bit3:1;
			unsigned char bit4:1;
			unsigned char bit5:1;
			unsigned char bit6:1;
			unsigned char bit7:1;
		} ;
		unsigned char byte;
	};
} EightBits;
//EightBits my_bytes;
#endif

#if 0
//
u8bdata aaa;
	sbit aaa0 = aaa ^ 0;
	sbit aaa1 = aaa ^ 1;
	sbit aaa2 = aaa ^ 2;
	sbit aaa3 = aaa ^ 3;
	sbit aaa4 = aaa ^ 4;
	sbit aaa5 = aaa ^ 5;
	sbit aaa6 = aaa ^ 6;
	sbit aaa7 = aaa ^ 7;

#endif
/*  2 byte
bit bit_test0;
bit bit_test1;
bit bit_test2;
bit bit_test3;
*/
// *** ERROR L104: MULTIPLE PUBLIC DEFINITIONS
//bit bit_test0;
//bit bit_test1;
	
typedef struct
{
	u8	hour;
	u8	min;
	u8	sec;
	u8	days;
} rtc_t;


/* macros --------------------------------------------------------------------- */



/* variables ------------------------------------------------------------------ */


/* functions ------------------------------------------------------------------ */
void wait(u8 d);
void timer2_delay (u16 tick) ;
void timer4_delay (u16 tick) ;


#ifdef __cplusplus
}
#endif
#endif 
/* end of file ---------------------------------------------------------------- */
