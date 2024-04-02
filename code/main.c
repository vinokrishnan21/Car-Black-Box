/*
 * File:   main.c
 * Author: vinod
 *
 * Created on 12 March, 2024, 5:01 PM
 */


#include "main.h"

#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)

static void init_config(void) {
    //All initialize functions
    init_clcd();
    init_adc();
    init_digital_keypad();
    init_i2c(100000);
    init_ds1307();
    init_timer2();
    PEIE=1;
    GIE=1;
    
}

void main(void) {
    init_config();
    unsigned char displayState = DASHBOARD_STATE, reset_state, menu_selected;
    char event[3] = "ON";
    unsigned char speed = 0;
    unsigned char key, prev_key;
    short int currentGear = -1;
    eeprom_at24c04_byte_write(0x00,'1'); //Press SW4 SW5 SW4 SW5
    eeprom_at24c04_byte_write(0x01,'0');
    eeprom_at24c04_byte_write(0x02,'1');
    eeprom_at24c04_byte_write(0x03,'0');
    log_car_data(event,speed);
    char sys_pw[5];
    clear_screen();
    prev_key = ALL_RELEASED;
    while(1){
        //code
        speed = read_adc()/10;
        if(speed > 99){
            speed=99;
        }
        key=read_digital_keypad(STATE);
        if(prev_key==key && key == SW4){
            if (displayState==MAIN_MENU_STATE){
                displayState=DASHBOARD_STATE;
            }
            else if (displayState == VIEW_LOG_STATE || displayState== CLEAR_LOG_STATE|| displayState==CHANGE_PWD_STATE) {
                displayState= MAIN_MENU_STATE;
            }
        }
        prev_key=key;
        for(int i=3000;i--;);
        
        if(key==SW1){
            //Set collision event
            strcpy(event,"C ");
            log_car_data(event,speed);
        }
        else if(key==SW2 && currentGear<5){
            //up gearing
            strcpy(event,gears[++currentGear]);
            log_car_data(event,speed);
            
        }
        else if(key==SW3 && currentGear>0){
            //down gearing
            strcpy(event,gears[--currentGear]);
            log_car_data(event,speed);
        }
        else if((key==SW4 || key == SW5) && displayState== DASHBOARD_STATE ) {
            
            
            prompt_password_entry(); 
            displayState= LOGIN_STATE;
            reset_state =  RESET_PASSWORD;
            TMR2ON=1;
        }
        else if(displayState == MAIN_MENU_STATE && key==SW6 ) {
            switch (menu_selected) {
                case 0: //view log
                    clear_screen();
                    clcd_print("# TIME     E  SP",LINE1(0));
                    displayState=VIEW_LOG_STATE;
                    reset_state=RESET_VIEW_LOG;
                    break;
                case 1://clear log
                    log_car_data("CL",speed);                  
                    clear_screen();
                    displayState= CLEAR_LOG_STATE;
                    reset_state=RESET_MEMORY;
                    
                    break;
               
                case 2://change password
                    log_car_data("CP",speed);
                    clear_screen();
                    displayState=CHANGE_PWD_STATE;
                    reset_state= RESET_CHANGE_PASSWORD;
                    
                    break; 
                    
            }
        }
        
        switch(displayState){
            case DASHBOARD_STATE:
                //clcd_print("Hello", LINE1(0));
                
                display_dashboard(event,speed);
                break;
            case LOGIN_STATE:
                
                switch(login(key, reset_state)) {
                    case RETURN_BACK:
                        displayState=DASHBOARD_STATE;
                        TMR2ON = 0;
                        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                        __delay_us(100);
                        clear_screen();
                        break;
                    case SUCCESS: 
                        reset_state = RESET_NOTHING;
                        
                        continue;
                    case LOGIN_SUCCESS:
                        displayState = MAIN_MENU_STATE;
                        reset_state = RESET_MAIN_MENU;
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        break;
                        
                }
                break;
            case MAIN_MENU_STATE:
                menu_selected =  prompt_menu_selection(key,reset_state);
                break;
            case VIEW_LOG_STATE:
                view_log(key,reset_state);
                break;
            case CLEAR_LOG_STATE:
                if(clear_log(reset_state)==SUCCESS) {
                    clear_screen();
                    displayState = MAIN_MENU_STATE;
                    reset_state =  RESET_MAIN_MENU;
                    continue;
                }
                break;
                
            case CHANGE_PWD_STATE:
                switch(change_password(key,reset_state)) {
                    case SUCCESS:
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                        __delay_us(100);
                        displayState = MAIN_MENU_STATE;
                        reset_state =  RESET_MAIN_MENU;
                        continue;
                        break;                        
                }
        }
        reset_state = RESET_NOTHING;
    }
    
    return;
}
