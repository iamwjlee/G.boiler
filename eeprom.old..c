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
u8 ee_read(u8 offset)
{
	/* direct address mode uses external data of 8051 or Indirect address mode uses address SFR*/
	u8 d;
	d=*((unsigned char *)(0x3000+offset));
	return d;
	
}
void ee_page_read()
{
	u8 i;
	u8 d[8];
	enable_program_mode(1);
	
	/* select page buffer */
	FEMR =MR_EEPROM|MR_PROGRAM_ERASE| MR_PBUFF;

	for(i=0;i<16;i++)
	{
		d[i]=FEDR;
		FEMR |=CR_READ;
	//	NOP;
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

	/* write something to page buffer ---------------------------->?? */
	FEDR = 0;
	/* set erase mode */
	FEMR = MR_EEPROM | MR_ERASE|MR_PROGRAM_ERASE;
	/* set page address. one page=16bytes */
	FEARH=0x0;	
	FEARM=0x00;
	FEARL=0x00;
	/* set Timer Control Register */
	FETCR =0x1f;
	/* start erase */
	FECR = 0x0b;
//	NOP;
	
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
	/* write something to page buffer ------------------------------> ??*/
	FEDR = 0x77;  //??
	/* set write mode */
	FEMR= MR_EEPROM|MR_PGM|MR_PROGRAM_ERASE;


	/* set page address. one page=16bytes */
	FEARH=0x0;	
	FEARM=0x00;
	FEARL=0x00;
	/* set Timer Control Register */
	FETCR =0x1f;
	/* start program or erase */
	FECR = 0x0b;
//	NOP;
	
	/* check status flag==1 */
	while(!FESR&SR_FLAG);
	enable_program_mode(0);

}


