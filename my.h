

#ifndef _MY_H_
#define _MY_H_

/* includes ------------------------------------------------------------------- */
 
#ifdef __cplusplus
extern "C" {
#endif

/* constants ------------------------------------------------------------------ */


/* enumerations --------------------------------------------------------------- */


/* types ---------------------------------------------------------------------- */




typedef int BOOL;
#define TRUE 1
#define FALSE 0
#ifndef true
#define  true TRUE
#endif
#ifndef false
#define  false FALSE
#endif

#ifndef null
#define null NULL
#endif

typedef unsigned int	U32;
typedef signed short S16;
typedef unsigned short U16;
typedef unsigned char  U8;
//typedef int partition_t;


//#define dprint(...)  //printf(__VA_ARGS__)


typedef  unsigned char u8;
typedef unsigned short u16;

#define ON 1
#define OFF 0


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
