
/* 
 * File:   
 * Author: 
 */

  
#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H

#define CURSOR_POS 0xC6


void display_dashboard(char *event, unsigned char speed);
void log_car_data(char *event,unsigned char speed);
void clear_screen(void);
void get_system_password ( char *system_password);
void prompt_password_entry(void);
char login(unsigned char key, unsigned char reset_state);
unsigned char prompt_menu_selection(unsigned char key, unsigned char reset_state);
void view_log(unsigned char key, unsigned char reset_state);
char clear_log(unsigned char reset_state);
char change_password(unsigned char key, unsigned char reset_state);

#endif	/* CAR_BLACK_BOX_H */

