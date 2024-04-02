/*
 * File:   at24c04.c
 * Author: vinodhini
 *
 * Created on 13 March, 2024, 3:08 PM
 */


#include "main.h"

unsigned char eeprom_at24c04_random_read(unsigned char memory_loc)
{
    unsigned char data_received = 0;
    i2c_start();
    i2c_write(SLAVE_WRITE_EE);
    i2c_write(memory_loc);
    i2c_rep_start();
    i2c_write(SLAVE_READ_EE);
    data_received = i2c_read(0);
    i2c_stop();
    
    return data_received;
}

void eeprom_at24c04_byte_write(unsigned char memory_loc, unsigned char data)
{
    i2c_start();
    i2c_write(SLAVE_WRITE_EE);
    i2c_write(memory_loc);
    i2c_write(data);
    i2c_stop();
}

void eeprom_at24c04_str_write(unsigned char memory_loc, unsigned char *data)
{
    while(*data!=0){
        eeprom_at24c04_byte_write(memory_loc,*data);
        memory_loc++;
        data++;
    }
}