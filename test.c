

#include	"MC95FG308.h"
#include	"func_def.h"
#include  "my.h"

#define MAX_Q 5
u8 q_buf[MAX_Q];  
u8 q_head=0;
u8 q_tail =0;

void q_send(u8 val)
{
	if((q_tail + 1) %MAX_Q ==q_head)
	{
		//buffer_full , real buffer size =MAX_Q -1
		return;
	}
	q_buf[q_tail]=val;
	//q_tail++;
	//if(q_tail ==MAX_Q) q_tail=0;
	q_tail = ++q_tail % MAX_Q;

}
u8 q_receive()
{
	u8 tmp;
	if(q_head == q_tail) 
	{   //No data 
		return 0;
	}	
	tmp=q_buf[q_head];
	q_head= ++q_head %MAX_Q;
	//q_head++;
	//if(q_head==MAX_Q) q_head=0;
	return tmp;
}

u8 get_key_action()
{
	if(POWER_SW_PIN==0)	
		HEATER_CONTROL_LED ^= 0x1;
	else if(HOT_COLD_SW_PIN==0)	
		PUMP_CONTROL_LED ^= 0x01;
	else if(CONT_OP_SW_PIN==0)
		COLD_CONTROL_LED ^= 0x01;
	else if(TIME_OP_SW_PIN==0)	
		HEATER_CONTROL_LED ^= 0x1;
	
	else if(DOWN_SW_PIN==0)
		PUMP_CONTROL_LED ^= 0x01;
	
	else if(UP_SW_PIN==0)	
		COLD_CONTROL_LED ^= 0x01;
	
	else {
		return KEY_IDLE;
	}
}

void pio_test_code()
{
	u8 key;
	while(1)
	{
		wait(100);
		HEATER_CONTROL_LED ^= 0x1;
		PUMP_CONTROL_LED ^= 0x01;
		COLD_CONTROL_LED ^= 0x01;
	}

	while(1)
	{
		wait(1);
		key=get_key_action();

	}

}

void timer_test_code()
{
	u8 cnt=0;
	HEATER_CONTROL_LED = 0x0;
	PUMP_CONTROL_LED = 0x00;
	COLD_CONTROL_LED = 0x00;
	
	while(1)
	{
	
		timer2_delay (500);  // 2m timer * x
		//timer4_delay(100 ) ;  //10msec timer;
		HEATER_CONTROL_LED ^= 0x1;
		//g_timer4
		if(++cnt>3) break;
	}

	while(1)
	{
		timer4_delay(100*2); //10msec * x
		PUMP_CONTROL_LED ^= 0x01;

	}


}

extern u16 timer2_2msec;

void fnd_test_code()
{
	u8 a=0;
	u8 b=1;
	/* Disable Timer 0 and 4 */
	IEN2 &= ~0x01;   	// ~Enable Timer0 interrupt
	T0CR &= ~0x01;   	// ~start

	IEN2 &= ~0x10;   	// ~Enable Timer4 interrupt
	T4CR &= ~0x01;   	// ~start

	timer2_2msec=0;
	while(1)
	{
		if(timer2_2msec > 1000)
		{
			timer2_2msec=0;
			a ^= 0x01;
			b ^=0x01;
			
		}
		timer2_delay (5);  // 2m timer * x

		FND_1=0;
		FND_10=1;

		FND_G=a; 
		FND_F=b;
		FND_E=1;
		FND_D=a;
		FND_C=1;
		FND_B=b;
		FND_A=a;

		NOP;
		NOP;
		FND_A=FND_B=FND_C=FND_D=FND_E=FND_F=FND_G=1;

		FND_1=1;
		FND_10=1;

	}

}


