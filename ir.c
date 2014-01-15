//======================================================
// Main program routine
// - Device name  : MC95FG308
// - Package type : 32LQFP
//======================================================


/*

	Decoding NEC Infrared Ray remote  Protocol

	Leader code : 9m on + 4.5m off + custom code +!custom code + datacode +!datacode

	Normal Leader code (13.5m)
	Repeat Leader code (11.25m)
	
	Data bit 1 (2.25m ): 0.56m  + sapce
	Data bit 0(1.125m): 0.56m  + sapce

	Falling edge interrupt 
     _	      	     ____        _______
		|      	     |     |||  |
		|      	     |	  |||  |
		|______ |	  |||_|
		


	
The IRrecv library consists of two parts. An interrupt routine is called every 50 microseconds, measures the length of the marks and spaces, and saves the durations in a buffer. The user calls a decoding routine to decode the buffered measurements into the code value that was sent (typically 11 to 32 bits).
For decoding, the MATCH macro determine if the measured mark or space time is approximately equal to the expected time.

The RC5/6 decoding is a bit different from the others because RC5/6 encode bits with mark + space or space + mark, rather than by durations of marks and spaces. The getRClevel helper method splits up the durations and gets the mark/space level of a single time interval.

n more detail, the receiver's interrupt code is called every time the TIMER1 overflows, which is set to happen after 50 microseconds. At each interrupt, the input status is checked and the timer counter is incremented. The interrupt routine times the durations of marks (receiving a modulated signal) and spaces (no signal received), and records the durations in a buffer. The first duration is the length of the gap before the transmission starts. This is followed by alternating mark and space measurements. All measurements are in "ticks" of 50 microseconds.


http://stuff.mit.edu/afs/sipb/contrib/linux/drivers/media/rc/ir-rc6-decoder.c

http://www.ustr.net/infrared/index.shtml


You don't need to decode those first  two bits, not even the CHK bit (except if you want to control as the TV do and described above), so you can skip those 3 bits and start to receive the ADDRESS bits.  To do that, you need to skip 2.75 bits time, and you will be exactly at the middle of the right level of the first ADDRESS bit to be read (non inverted level).

So, upon sensing the first low level, your software should wait 4.752 milliseconds and then start to read the next 11 bits spaced 1.728ms each.   The first 5 bits are Address and the next 6 bits are  Command, logic correct level, LOW = 0, HIGH = 1.

To make sure your software is waiting the correct timing, you need to use a dual channel oscilloscope, and this procedure to adjust your software:



is HIGH to LOW ?


*/
#include	"MC95FG308.h"
#include	"func_def.h"
#include "def.h"
#include  "my.h"
//#include <intrins.h>

enum ir_status
{
	INIT,
	LEADER_DETECT,
	CUSTOM_DETECT,
	DATA_DETECT
	
};

u8 ir_state;
u8 ir_key_data;
u8 ir_repeat_key;

u8 tick1m_for_ir =0;
u16 ir_data=0;
u16 ir_data_mask=0;
void timer_interrupt(void)
{
	// 1msec
	// This is called by every time the timer overflows, which is set to happen after 1msecond. 
	// at each interrupt, the input status is checked and the timer counter is incremented.
	// 
	tick1m_for_ir++;
	//tick_of_1msec ++;
	//tick_of_500usec ++;


}

/*
	use falling edge 
	


*/
void ir_interrupt(void)
{
	u8 interval;
	u8 code8; 
	u8 edoc8; 

	
	interval=tick1m_for_ir;
	tick1m_for_ir=0;

	if(ir_state == INIT)
	{
		ir_data=0;
		ir_state= LEADER_DETECT;
	}	
	else if(ir_state == LEADER_DETECT)
	{
		if(interval>12 && interval <14)  /* reference 13.5 ms */
		{
			ir_data=0;
			ir_data_mask=0x0001;
			ir_state = CUSTOM_DETECT;
		}
		
		if(interval>10 && interval <13)  /* reference 11.25 ms for Repeat code*/
		{  
			ir_data=0;
			ir_data_mask=0x0001;
			ir_state = CUSTOM_DETECT;
			ir_key_data =ir_repeat_key;
		}
	}	

	else if(ir_state == CUSTOM_DETECT)
	{
		if(interval >= 2 && interval <4) 	/* reference 2.25 ms */
			ir_data |= ir_data_mask;  
		else if(interval > 0  && interval < 2) /* reference 1.125 ms */
			//ir_data &= ~(ir_data_mask);
			//NOP;  
		 {
		 }
		else 
			ir_state = LEADER_DETECT;

		ir_data_mask <<= 1;
		if(ir_data_mask==0)
		{
			code8 = ir_data;
			edoc8= ~(ir_data>>8); 
			if(code8 == edoc8)
			{
				ir_data=0;
				ir_data_mask=0x0001;
				ir_state = DATA_DETECT;
			}

		}
		
	}
	else if(ir_state == DATA_DETECT)
	{
		if(interval >= 2 && interval <4) 	/* reference 2.25 ms */
			ir_data |= ir_data_mask;  
		else if(interval > 0  && interval < 2) /* reference 1.125 ms */
			//ir_data &= ~(ir_data_mask);
			//NOP;    
			{
				
				_nop_();
			}		
		else 
			ir_state = LEADER_DETECT;

		ir_data_mask <<= 1;
		if(ir_data_mask==0)
		{
			
			code8 = ir_data;
			edoc8= ~(ir_data>>8); 
			if(code8 == edoc8)
			{
			   ir_key_data = code8;	
			   ir_repeat_key = code8;
			}
			ir_state = INIT;
		}


	}


}

