
/* 
 * File:   
 * Author: 
 */

#ifndef AT24C04_H
#define	AT24C04_H

//macros for external EEPROM address for read and write modes
#define SLAVE_WRITE_EE             0xA0
#define SLAVE_READ_EE              0xA1
unsigned char eeprom_at24c04_random_read(unsigned char memory_loc);
void eeprom_at24c04_byte_write(unsigned char memory_loc, unsigned char data);
void eeprom_at24c04_str_write(unsigned char memory_loc, unsigned char *data);

#endif	

