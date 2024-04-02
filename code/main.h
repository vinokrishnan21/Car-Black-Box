/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "adc.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "ds1307.h"
#include "i2c.h"
#include "cbb.h"
#include "at24c04.h"
#include "timers.h"
#include <string.h>


#define DASHBOARD_STATE 0xA0
#define LOGIN_STATE 0x0A1
#define MAIN_MENU_STATE 0x0A2
#define VIEW_LOG_STATE 0x0A3
#define CLEAR_LOG_STATE 0x0A4
#define CHANGE_PWD_STATE 0x0A5
#define SET_TIME_STATE 0x0A6
#define RESET_NOTHING 0XB0
#define RESET_PASSWORD 0xB1
#define RESET_MAIN_MENU 0xB2
#define RESET_VIEW_LOG 0xB3
#define RESET_MEMORY 0xB4
#define RESET_CHANGE_PASSWORD 0XB5
#define RESET_SET_TIME 0XB5

#define MAXIMUM_LOGIN_ATTEMPTS '3'

#define SUCCESS 0x01
#define FAILURE 0x00
#define RETURN_BACK 0x02
#define LOGIN_SUCCESS 0x03

char *gears[] = {"GN","G1","G2","G3","G4","GR"};
char *password = "1010";
#endif	/* MAIN_H */

