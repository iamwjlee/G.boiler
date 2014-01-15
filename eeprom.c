/*

	EEPROM data memory
	512 bytes at 3000h-31ffh

	it is read by byte and written by byte or page(16-byte)
	it is mapped to external data memory of 8051

	How to read external memory of 8051 ?
	Addressing of data EEPROM ?
	Page address[15:4] + word address[3:0]

	FEARL->DPTR -> xdata memory(000h-ffffh)

	eeprom INT17,  IE2.5 
	Enable or Disable EEPROM interrupt
	INT17E of IE2  


	To access eeprom  --------------------?????

	1. enter OCD(isp) mode
	2. set ENBDM bit of BCR
	3. enable debug and request debug mode

	4. pabe buffer/pabe address FEARH:FEARM:FEARL=20¡¯hx_xxxx 
*/

#include	"MC95FG308.h"
#include	"func_def.h"
#include  "def.h"
#include  "my.h"
#include "eeprom.h"

// FEMR
#define FSEL	7
#define ESEL	6
#define PGM		5
#define ERASE	4
#define PBUFF	3
#define OTPE	2
#define VFY		1
#define FEEN	0

// FECR 
#define AEF		7
#define AEE		6
#define EXIT0 	4
#define WRITE	3
#define READ	2
#define FERST	1
#define PBRST	0
#define Eeprom ((volatile unsigned char xdata *) 0)

void eeprom_entry(void)
{
	FEDR = 0xA5;
	FEDR = 0x5A;
}

void eeprom_exit(void)
{
	FEMR  = 0x00;                 //
	FECR  = 0x03;                 // nFERST[1], nPBRST[0]
	FESR  = 0x80;                 // PEVBSY[7] = completed 
	FETCR = 0x00;                 //
	FEARL = (unsigned char)(EEPROM_BASE);
	FEARM = (unsigned char)(EEPROM_BASE>>8);
	FEARH = 0;
	FEDR  = 0x00;                 //
	FETR  = 0x00;                 //
}

unsigned char eeprom_read_byte(unsigned int addr)
{
	
	eeprom_exit();
    
   	return Eeprom[addr];

}

char eeprom_erase_byte(unsigned int addr)
{
	eeprom_entry();
	// 1. Reset page buffer
	FEMR = 0x41;
    FECR = 0x02;
	// 2. Select page buffer
    FEMR = 0x49;
    // 3. Erase data to page buffer
    Eeprom[addr] = 0x00;
    // 4. Set page address.
    FEARL = (unsigned char)addr;
    FEARM = (unsigned char)(addr>>8);
    FEARH=0;
    // 5. Set erase mode
    FEMR = 0x51;
    // 6. Set erase time.
    FETCR = ERSTIME;
    // 7. Start erase.
    FECR = 0x0B;

	while( (FESR & 0x80) != 0);	
	while( (FESR & 0x80) == 0);
    
    eeprom_exit();
    
    if( Eeprom[addr] != 0x00) return -1;
    	
    return 0;
}


char eeprom_write_byte(unsigned int addr, unsigned char dt)
{
	eeprom_entry();
	// 1. Reset page buffer
	FEMR = 0x41;
    FECR = 0x02;
	// 2. Select page buffer
    FEMR = 0x49;
    // 3. Write data to page buffer
    Eeprom[addr] = dt;
    // 4. Set page address.
    FEARL = (unsigned char)addr;
    FEARM = (unsigned char)(addr>>8);
    FEARH=0;
    // 5. Set write mode
    FEMR = 0x61;
    // 6. Set erase time.
    FETCR = PGMTIME;
    // 7. Start write.
    FECR = 0x0B;

	while( (FESR & 0x80) != 0);
	
	while( (FESR & 0x80) == 0);

	eeprom_exit();
    
    if( Eeprom[addr] != dt) return -1;
    	
    return 0;
}

