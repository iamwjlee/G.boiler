//======================================================
// Main program routine
// - Device name  : MC95FG308
// - Package type : 32LQFP
//======================================================

// Generated    : Tue, Jan 07, 2014 (14:20:09)

/*
	register addressing mode
		add ax,bx
		very limited address space
		very fast execution
		no memory access
	register indirect addressing mode
		ea=(r)
	
	direct addressing mode
		add ax,value
		limited address space
	indirect addressing mode
		ea=(a)        // look in a, find address (a)
		add ax,(a)  //pointed to by contents of a
		ea=(((a)))
		


*/
#include	"MC95FG308.h"
#include	"func_def.h"
#include  "my.h"



u16 g_ext0=0;
u16 g_ext1=0;
u8 g_bit=0;
u8 g_timer0=0;
u8 timer0_30msec_on=0;
u8   timer0_tick_of_10msec=0;
u8  timer0_tick_of_1msec=0;

u16 timer2_tick_of_2msec=0;
u16 timer4_tick_of_10msec=0;

//code u8 digit[11]={0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7F,0x7B};
u8 digit[12]={0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7F,0x7B,0x4f};
u8 toggle_7seg_digit=0;

u8 key;
u8 hot_cold_key_pressed=0;
u16 event_time_tick=0;
u8 g_timer10m=0;
u8 g_timer50m=0;
u8 g_timer1sec=0;


u8 reservation_time;
u8 temperature;	
//u8 status;

/* left side led */
u8 power_on=0; //power on or off
u8 heater_op=0;
u8 pump_op=0;
u8 cold_op=0;

/* right side  led */
u8 continuous_op_set=0; //cont or timer
u8 hot_op_set=0;  //hot or cold
u8 error_set=0;  //normal or error

u8 water_shortage =0;
u8 temperature_sense_defect =0;

u16 current_time;  //for time reservation

u16 error_set_time_tick=0;
u8 error_set_toggle=0;

void display_7seg(u8 c);


void wait(u8 d)
{
	
	/*--------------------
		8M/12=666666hz
		1/666666hz=1.5usec
	----------------------*/
	u16 i,j;
	for(i=0;i<d;i++)
		for(j=0;j<6666;j++); //10msec
}

void timer2_delay (u16 tick) {
  u16 timetick;

  timetick = timer2_tick_of_2msec=0;
  while ((timer2_tick_of_2msec - timetick) < tick);
}

void timer4_delay (u16 tick) {
 timer4_tick_of_10msec=0;
  while (timer4_tick_of_10msec  < tick);
}


//======================================================
// interrupt routines
//======================================================

void INT_Ext0() interrupt 0
{
	// External interrupt 0
	g_ext0++;
}

void INT_Ext1() interrupt 1
{
	// from pump sense int1/p11
	g_ext1++;
}

void INT_Timer0() interrupt 12
{
	// 10 msec
	// 1msec
	g_timer0++;
	g_timer10m++;
	timer0_tick_of_1msec++;
	if(timer0_tick_of_1msec >= 10)
	{
		timer0_tick_of_1msec=0;
		if(++timer0_tick_of_10msec >= 3)
		{
			timer0_tick_of_10msec=0;
			timer0_30msec_on=1;

		}

	}

	if(power_on==1)
	{

		if(heater_op) 		HEATER_CONTROL_LED=1;
		if( cold_op )  COLD_CONTROL_LED=1;
		if( pump_op) PUMP_CONTROL_LED=1;

		
		FND_LED=0;
		if(continuous_op_set) FND_A = 0; //cont_operation
		else FND_B = 0;  //time_operation
		if(hot_op_set) FND_C = 0; //hot set
		else FND_D = 0; //cold set
		if(error_set==1) 
		{

			if(!error_set_toggle)
			FND_E = 0;  // Error
			//error_set_time_tick=g_timer1sec+100;
			//g_timer1sec++;
				
			
		}	
		NOP;
		FND_LED=1;
		FND_A=FND_B=FND_C=FND_D=FND_E=1;

		if(water_shortage)
			display_7seg(0xe1);
		if(pump_op==0)
			display_7seg(0xe0);
		if(temperature_sense_defect==1)
			display_7seg(0xe2);
		if(water_shortage==0 && pump_op==1 &&temperature_sense_defect==0)
		{
			if(continuous_op_set)
				display_7seg(temperature);
			else
				display_7seg(reservation_time);
		}	
	}
	
	/* check key input */
	if(g_timer10m >=5)  //50 msec
	{
		g_timer50m++;
		g_timer10m=0;

		if(POWER_SW_PIN==0)	key= KEY_POWER;
		else if(HOT_COLD_SW_PIN==0)	{
			if(hot_cold_key_pressed==0)
			{
				event_time_tick=g_timer0+200;   //pb!
				hot_cold_key_pressed=1;
			}	
			else
			{
				if(g_timer0 > event_time_tick)
				{
					hot_cold_key_pressed=0;
					key= KEY_HOT_COLD;
				}
			}
		}	
		else if(CONT_OP_SW_PIN==0)	key= KEY_CONT_OP;
		else if(TIME_OP_SW_PIN==0)	 key= KEY_TIME_OP;
		else if(DOWN_SW_PIN==0)	key= KEY_DOWN;
		else if(UP_SW_PIN==0)	key= KEY_UP;
		else {
		hot_cold_key_pressed=0;
		key= KEY_IDLE;
		}
	}

	HEATER_CONTROL_LED=0;
	COLD_CONTROL_LED=0;
	PUMP_CONTROL_LED=0;
}

void INT_Timer2() interrupt 14
{
	// Timer2 interrupt
	timer2_tick_of_2msec++;

	//FND_LED=0;
	//FND_A = 0;
}

void INT_Timer4() interrupt 16
{
	// Timer4 interrupt
	
	timer4_tick_of_10msec++;
}
void INT_BIT() interrupt 22
{
	/*  
			5: 16.384msec
			6: 32.768msec
			7: 65.536msec
	*/
	
	g_bit++;
}


//======================================================
// peripheral setting routines
//======================================================


void clock_init()
{
	// internal RC clock (8.000000MHz)
	// Nothing to do for the default clock
}

void Timer0_init()
{
	// initialize Timer0
	// 8bit timer, period = 1.000000mS
	T0CR = 0x8E;    	// stop & timer setting
	T0DR = 0xF9;    	// period count
	IEN2 |= 0x01;   	// Enable Timer0 interrupt
	T0CR |= 0x01;   	// start
}

void Timer2_init()
{
	// initialize Timer2
	// 16bit timer, period = 2.000000mS
	T2CR = 0x8E;    	// stop & timer setting
	T3CR = 0x4F;    	// 16bit
	T3DR = 0x00;    	// period count High
	T2DR = 0xF9;    	// period count Low
	IEN2 |= 0x04;   	// Enable Timer2 interrupt
	T2CR |= 0x01;   	// start
}


void Timer4_init()
{
	// initialize Timer4
	// 16bit timer, period = 10.000000mS
	T4CR = 0x92;    	// stop & timer setting
	T4HDR = 0x04;   	// period count High
	T4LDR = 0xE1;   	// period count Low
	IEN2 |= 0x10;   	// Enable Timer4 interrupt
	T4CR |= 0x01;   	// start
}

void BIT_init()
{
	// initialize Basic interval timer 
	/*  
			5: 16.384msec
			6: 32.768msec
			7: 65.536msec
	*/
	BCCR = 0x05;    	// setting
	IEN3 |= 0x10;   	// Enable BIT interrupt
}


void ExINT_init()
{
	// initialize external interrupt
	EIEDGE = 0x03;  	// level / edge
	EIPOLA = 0x03;  	// polarity
	EIBOTH = 0x00;  	// both polarity
	EIFLAG = 0x00;  	// clear all flags
	IEN0 |= 0x03;   	// INT. 3,2,1,0
	EIENAB = 0x03;  	// enable INT pin
}

u8 ADC_read2(unsigned int *adcVal)
{
	// read A/D convertor
	//unsigned int adcVal;
	
	//while(!(ADCM & 0x10));	// wait ADC busy
	if(ADCM & 0x10)
	{
		*adcVal = (ADCRH << 8) | ADCRL;	// read ADC
		*adcVal >>= 4;   	// data alignment
		ADCM &= ~0x40;  	// stop ADC
		return	0;
	}
	return	1;
}

void ADC_init()
{
	// initialize A/D convertor
	ADCM = 0x00;    	// setting
	ADCM2 = 0x00;   	// trigger source, alignment, frequency
	/* fx/4   3v~5v*/
	//ADCM2 = 0x01;   	// trigger source, alignment, frequency
}

void ADC_start(unsigned char ch)
{
	// start A/D convertor
	ADCM |= (ADCM & 0xf0) | (ch & 0xf);	// select channel
	ADCM |= 0x40;   	// start ADC
}

void port_init()
{
	// initialize ports
	//  23 : P00      o 
	/* 
		input : p32,p32,p33,p44,p04,p05  for  s/w0 -5

			p01 : temp sence 
			p10/int0 : remocon
			p11/int1 : pump sence
			p12 : water sence
	 	out  :  
			p25,p26,p03
			p02 :buzzer
			
			p23: fnd_1
			p24: fnd_10
			p16 :fnd_led

			p37: fnd_a
			p36: b
			p35: c
			p34: d
			p15: e
			p14: f
			p13: g
	*/		
	P0IO = 0xcF;    	// direction
	P0PU = 0xff;    	// pullup
	P0OD = 0x00;    	// open drain
	P0DB = 0x00;    	// debounce
	P0   = 0x00;    	// port initial value
	
	P1IO = 0xF9;    	// direction
	P1PU = 0xff;    	// pullup
	P1OD = 0x00;    	// open drain
	P1DB = 0x00;    	// debounce
	P1   = 0x00;    	// port initial value
	
	P2IO = 0xFF;    	// direction
	P2PU = 0xff;    	// pullup
	P2OD = 0x00;    	// open drain
	P2DB = 0x00;    	// debounce
	P2   = 0x00;    	// port initial value
	
	P3IO = 0xF0;    	// direction
	P3PU = 0xff;    	// pullup
	P3OD = 0x00;    	// open drain
	P3DB = 0x00;    	// debounce
	P3   = 0x00;    	// port initial value
	
	// Set port function
	PSR0 = 0x00;    	// AN7 ~ AN0
	PSR1 = 0x00;    	// I2C, AN14 ~ AN8
}

void display_led(u8 cont_op,u8 time_op, u8 hot_set,u8 cold_set,u8 err_set)
{
	
	FND_LED=0;
	if(cont_op) FND_A = 0;
	if(time_op) FND_B = 0;
	if(hot_set) FND_C = 0;
	if(cold_set) FND_D = 0;
	if(err_set) FND_E = 0;
	NOP;
	NOP;
	FND_LED=1;
	FND_A=FND_B=FND_C=FND_D=FND_E=1;
}

/*
Digit	gfedcba	abcdefg	a		b		c		d		e		f		g
0			0x3F		0x7E		on	on	on	on	on	on	off
1			0x06		0x30		off	on	on	off	off	off	off
2			0x5B		0x6D		on	on	off	on	on	off	on
3			0x4F		0x79		on	on	on	on	off	off	on
4			0x66		0x33		off	on	on	off	off	on	on
5			0x6D		0x5B		on	off	on	on	off	on	on
6			0x7D		0x5F		on	off	on	on	on	on	on
7			0x07		0x70		on	on	on	off	off	off	off
8			0x7F		0x7F		on	on	on	on	on	on	on
9			0x6F		0x7B		on	on	on	on	off	on	on
a                     0x7D     on  on   on   on  on  off   on
E                     0x4F       1     0   0    1    1     1    1
*/
void display_7seg(u8 c)
{
	u8 val;
	u8 seg10,seg1;

	if(c == 0xe0)
	{
		seg10=10;
		seg1=0;
	}
	else if(c==0xe1)
	{
		seg10=10;
		seg1=1;
	}
	else if(c==0xe2)
	{
		seg10=10;
		seg1=2;
	}
	else
	{
		seg10 = c/10;
		seg1 = c%10;
	}
	
	if(toggle_7seg_digit==0)
	{
		FND_1=0;
		val=seg1;
		toggle_7seg_digit=1;
	}	
	else
	{
		
		FND_10=0;
		val=seg10;
		toggle_7seg_digit=0;
	}	
	
	FND_G=~digit[val]&0x01;	
	FND_F=~digit[val]>>1& 0x01;	
	FND_E=~digit[val]>>1& 0x01;	
	FND_D=~digit[val]>>1&0x01;	
	FND_C=~digit[val]>>1&0x01;	
	FND_B=~digit[val]>>1&0x01;	
	FND_A=~digit[val]>>1&0x01;	
	
	NOP;
	NOP;
	FND_A=FND_B=FND_C=FND_D=FND_E=FND_F=FND_G=1;

	FND_1=1;
	FND_10=1;
}

#define MR_EEPROM 0x40
#define MR_PROGRAM_ERASE 0x01
#define MR_PBUFF 0x08
#define MR_PROGRAM_VFY 0x20
//#define MR_ERASE_VFY 0x10
#define MR_ERASE 0x10
#define MR_PGM 0x20

#define CR_RESET 0x02
#define CR_EXIT 0x30
#define CR_READ 0x04

#define SR_FLAG 0x80


void enable_program_mode(u8 option)
{
	if(option==1)
	{
		FEDR = 0xa5;
		FEDR = 0x5a;

	}
	else
	{
		FECR=CR_EXIT;
	}

}
void ee_page_read()
{
	u8 i;
	u8 d[8];
	enable_program_mode(1);
	
	/* select page buffer */
	FEMR =MR_EEPROM|MR_PROGRAM_ERASE| MR_PBUFF;

	for(i=0;i<8;i++)
	{
		d[i]=FEDR;
		FEMR |=CR_READ;
		NOP;
	}	
	enable_program_mode(0);

}
void ee_page_erase()
{
	enable_program_mode(1);

	/* reset page buffer */
	FEMR = MR_EEPROM|MR_PROGRAM_ERASE;
	FECR = CR_RESET;

	
	/* select page buffer */
	FEMR =MR_EEPROM|MR_PROGRAM_ERASE| MR_PBUFF;

	/* write something to page buffer */
	FEDR = 0;
	/* set erase mode */
	FEMR = MR_EEPROM | MR_ERASE|MR_PROGRAM_ERASE;
	/* set page address. one page=16bytes */
	FEARH=0x0;	//?
	FEARM=0x00;
	FEARL=0x00;
	/* set Timer Control Register */
	FETCR =0x1f;
	/* start erase */
	FECR = 0x0b;
	NOP;
	
	/* check status flag==1 */
	while(!FESR&SR_FLAG);
	enable_program_mode(0);

}

void ee_page_write()
{
	enable_program_mode(1);
	
	/* reset page buffer */
	FEMR = MR_EEPROM|MR_PROGRAM_ERASE;
	FECR = CR_RESET;
	/* select page buffer */
	FEMR =MR_EEPROM|MR_PROGRAM_ERASE| MR_PBUFF;
	/* write something to page buffer */
	FEDR = 0x77;  //??
	/* set write mode */
	FEMR= MR_EEPROM|MR_PGM|MR_PROGRAM_ERASE;


	/* set page address. one page=16bytes */
	FEARH=0x0;	//?
	FEARM=0x00;
	FEARL=0x00;
	/* set Timer Control Register */
	FETCR =0x1f;
	/* start program or erase */
	FECR = 0x0b;
	NOP;
	
	/* check status flag==1 */
	while(!FESR&SR_FLAG);
	enable_program_mode(0);

}


void main_loop(u8 key)
{
	u16 adc_temperature;
	switch(key)
	{
		case KEY_POWER:
	
			/* power_off */
			power_on=0;
		
			break;
		case KEY_HOT_COLD:
			if(hot_op_set==0)
			{
				/*hot */
				
				hot_op_set=1;
			}
			else
			{
				/* cold  */

				hot_op_set=0;
			}
			break;

		case KEY_CONT_OP:
			continuous_op_set=1;
			break;
		case KEY_TIME_OP:
			continuous_op_set=0;

			break;
		case KEY_DOWN:
			if(continuous_op_set==1)
			{
				if(temperature > 10 && temperature <51)
					temperature--;
			}
			else
			{
				if(reservation_time > 0 && temperature <13)
					reservation_time--;
			}
			break;
		case KEY_UP:
			if(continuous_op_set==1)
			{
				if(temperature > 10 && temperature <51)
					temperature++;

			}
			else
			{
				if(reservation_time > 0 && temperature <13)
					reservation_time++;

			}
			break;
		case KEY_IDLE:

			//if(1sec )
			//error_set_toggle=~error_set_toggle;
			error_set_toggle^=0x01;
			if(g_timer50m>40)  //2sec
			{
				g_timer50m=0;

				/* check if pump is working well */
				if( g_ext1 > 50)   	
				{
					g_ext1=0;
					pump_op=1;	
				}	
				else 
				{
					g_ext1=0;
					pump_op=0;
				}	

				/* check if water shortage */
				if(WATER_SENSE_PIN==1)
				{
					water_shortage= 1;
				}
				else
				{
					water_shortage= 0;
				}

				/* buzzer set */
				if(pump_op==0 || water_shortage==1||temperature_sense_defect==1)
				{
					BUZZER_PIN=1;
					error_set=1;
				}
				else
				{
					
					error_set=0;
					BUZZER_PIN=0;
				}
				

				if(!ADC_read2(&adc_temperature))
				{
					if(adc_temperature == 0x01 || adc_temperature > 0xfff )
					{
						temperature_sense_defect=1;
					}
					else 
					{
						temperature_sense_defect=0;
						if(hot_op_set==ON)
						{
							if(adc_temperature  > temperature ) 	
							{
								heater_op=0;
								//HEATER_CONTROL_LED=OFF;
							}	
							else if(adc_temperature < temperature ) 
							{
								heater_op=1;
								//HEATER_CONTROL_LED=ON;
							}	
						}
						else
						{
							if(adc_temperature  > temperature ) 	
							{
								cold_op=1;    //COLD_CONTROL_LED=ON;
							}	
							else if(adc_temperature < temperature ) 	
							{
								cold_op=0;   //COLD_CONTROL_LED=OFF;
							}	
						}
					}
					
				}
			}

			if(reservation_time * 60 < current_time )
			{
				power_on=0;
			}
				
			break;
	}

}


//unsigned char xdata my_xdata[50];

void fnd_test_code()
{

}

void interrupt_test_code()
{

}
void adc_test_code()
{


}
void eeprom_test_code()
{

}
void remocon_test_code()
{

}
void main()
{

	/* form eeprom */
	temperature=28;
	reservation_time=8;
	continuous_op_set=1;
	hot_op_set=1;

	/* temp status        */
	heater_op=1;
	cold_op=0;
	pump_op=1;
	/* alwayes */
	power_on=0;
	cli();          	// disable INT. during peripheral setting
	port_init();    	// initialize ports
	clock_init();   	// initialize operation clock
	ADC_init();     	// initialize A/D convertor
	BIT_init();     	// initialize Basic interval timer
	ExINT_init();   	// initialize external interrupt
	Timer0_init();  	// initialize Timer0
	Timer2_init();  	// initialize Timer0
	Timer4_init();  	// initialize Timer4
	sei();          	// enable INT.

	
	
	FND_1=1;
	FND_10=1;
	FND_LED=1;
	
	wait(100); //10msec * x

	while(1)
	{

#ifdef _timer0_okay
		if(power_on)
		{
			main_loop(key);
		}
		else
		{
			if(key==KEY_POWER) 
			{
				current_time=0;
				power_on=1;
			}	
			/*  check temperature < 2 dgree for freezing    */
			
		}
#else
		wait(2); //10msec * x
		get_key_action();
#endif
	}
	
}

