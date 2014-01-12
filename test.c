

#include	"MC95FG308.h"
#include	"func_def.h"
#include  "my.h"


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


