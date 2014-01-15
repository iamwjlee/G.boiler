#ifndef _EEPROM_H_
#define _EEPROM_H_

#define PGMTIME                0x9A/2   // 2.5ms@8MHz
#define ERSTIME                0x9A/2   // 2.5ms@8MHz
#define BERSTIME               0x9C/4   // 2.5ms@8MHz

void eeprom_entry(void);
void eeprom_exit(void);
extern char eeprom_erase_byte(unsigned int addr);
extern char eeprom_erase_page(unsigned int addr);
extern char eeprom_erase_bulk(void);
extern char eeprom_write_byte(unsigned int addr, unsigned char dt);
extern char eeprom_write_page(unsigned int addr, unsigned char *dt);
extern void eeprom_read_page(unsigned int addr, unsigned char *dt);
extern unsigned char eeprom_read_byte(unsigned int addr);
extern char eeprom_interrupt(unsigned int addr, unsigned char );

#endif

