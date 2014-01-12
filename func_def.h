//======================================================
// Function and global variables definition
//======================================================

void port_init();             	// initialize ports
void clock_init();            	// initialize operation clock
void ADC_init();              	// initialize A/D convertor
void ADC_start(unsigned char ch);	// start A/D convertor
unsigned int ADC_read();      	// read A/D convertor

