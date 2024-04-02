/*
 * File:   cbb.c
 * Author: vinodhini
 *
 * Created on 12 March, 2024, 5:36 PM
 */


#include "main.h"

unsigned char clock_reg[3];
char time[7];  // to store time in "HH:MM:SS"
char log[11]; // to store HHMMSSEVSP info
char pos = -1;
char event_count = 0;
extern unsigned char seconds, return_time;
char *menu[]={"View log","Clear log","Change password"};

void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(100);
}

static void get_time()
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
    
    
    // HH -> 
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    
    // MM 
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    
    // SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = '\0';
}

void display_dashboard(char *event, unsigned char speed)
{
    
    clcd_print("  TIME     E  SP",LINE1(0));
    //clcd_print("  TIME     E  SP",LINE2(0));
    
    get_time();
    
    //Display time in HH:MM:SS format
    clcd_putch(time[0],LINE2(2));
    clcd_putch(time[1],LINE2(3));
    clcd_putch(':',LINE2(4));
    clcd_putch(time[2],LINE2(5));
    clcd_putch(time[3],LINE2(6));
    clcd_putch(':',LINE2(7));
    clcd_putch(time[4],LINE2(8));
    clcd_putch(time[5],LINE2(9));
    
    //Set speed as Zero in ON state
    if(!strcmp(event,"ON")){
        speed=0x0;
    }
    //Display event
    clcd_print(event, LINE2(11));
    
    
    
    //Display speed
    clcd_putch(speed/10 +'0',LINE2(14)); //Displaying the first digit of speed by passing it as char
    clcd_putch(speed%10 +'0',LINE2(15)); //Displaying the second digit of speed by passing it as char
}
void log_event()
{
    unsigned char addr;
    pos++;
    if(pos==10){
        pos=0;
    }
    addr = pos * 10 + 5;
    for(int i=0; log[i]!='\0';i++){
        eeprom_at24c04_byte_write(addr,log[i]);
        addr++;
    }
    
    if(event_count<9){
        event_count++;
    }
}
void log_car_data(char *event,unsigned char speed)
{
    get_time();
    strncpy(log,time,6);
    strncpy(log+6,event,2);
    log[8]=speed/10 + '0';
    log[9]=speed%10 + '0';  
    log[10]='\0';
    
    log_event();
}

void get_system_password ( char *system_password) {
    for(int i=0; i<4; i++) {
            system_password[i]=eeprom_at24c04_random_read(i);
        }
}

void prompt_password_entry(void) {
    clear_screen();
    clcd_print("  ENTER PASSWORD", LINE1(0));
    clcd_write(CURSOR_POS, INST_MODE);
    clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
    __delay_us(100);
}

char login(unsigned char key, unsigned char reset_state)
{
    static char attempts_rem = MAXIMUM_LOGIN_ATTEMPTS;
    static int j=0;
    static char password_entered[5];
    static char system_password[5];
    if(reset_state == RESET_PASSWORD) {
        return_time = 5;
        attempts_rem = MAXIMUM_LOGIN_ATTEMPTS;
        for(int i=0; i<5; i++){
            password_entered[i]='\0';
        }  
        key = ALL_RELEASED;
    }
    if(return_time==0) {
        return RETURN_BACK;
    }
    if(key==SW4 && j<4) {
        password_entered[j]='1';
        clcd_putch('*',LINE2(j));
        j++;
        return_time=5;
    }
    else if(key==SW5 && j<4) {
        password_entered[j]='0';
        clcd_putch('*',LINE2(j));
        j++;
        return_time=5;
    }
    if(j==4) {
        get_system_password(system_password);
        if(!strcmp(password_entered,system_password)) {
            //Go to menu screen
            return LOGIN_SUCCESS;
        }
        else {
            attempts_rem--;
            if(attempts_rem == '0' ) {
                //lock user 
                clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                __delay_us(200);
                clcd_print("YOU ARE BLOCKED!",LINE1(0));
                clcd_print("Wait for 30 sec",LINE2(0));
                seconds = 30;
                
                while(seconds!=0) {
                    clcd_putch(seconds/10 + '0', LINE2(9));
                    clcd_putch(seconds%10 + '0', LINE2(10));
                }
                
                attempts_rem='3';
              
            }
            else{
                //prompt for another attempt
                clear_screen();
                clcd_print("WRONG PASSWORD!",LINE1(0));
                clcd_putch(attempts_rem, LINE2(0));
                clcd_print("attempts left",LINE2(1));
                __delay_ms(3000);
                
            }
            prompt_password_entry();
            j=0;
            return_time = 5;
        }
    }
    return SUCCESS;
}

unsigned char prompt_menu_selection(unsigned char key, unsigned char reset_state)
{
    static char menu_selected;
    if(reset_state==RESET_MAIN_MENU) {
        menu_selected=0;
    }
    if(key==SW5 && menu_selected<2) {
        menu_selected++;
        clear_screen();
    }
    else if(key==SW4 && menu_selected>0) {
        menu_selected--;
        clear_screen();;
    }
    
    if(menu_selected<2) {
        clcd_putch('*',LINE1(0));
        clcd_print(menu[menu_selected],LINE1(2));
        clcd_print(menu[menu_selected+1],LINE2(2));
    }
    else if(menu_selected==3) {
        clcd_print(menu[menu_selected-1],LINE1(2));
        clcd_print(menu[menu_selected],LINE2(2));
        clcd_putch('*',LINE2(0));
    }
    return menu_selected;
}

void view_log(unsigned char key, unsigned char reset_state){
    char log_data[11];
    unsigned char addr;
    static unsigned char pos; 
    //no events there
    if(event_count==-1)
    {
        clcd_print("EMPTY LOG!",LINE2(0));
        
    }
    else
    {
       if(reset_state==RESET_VIEW_LOG)
    {
        pos=0;
    }
    if(key==SW5 && pos<event_count-1)
    {
        pos++;
    }
    else if(key==SW4 && pos>0)
    {
        pos--;
    }
    for(int i=0;i<10;i++){
        addr= pos*10+5;
        log_data[i]=eeprom_at24c04_random_read(addr+i);
        
    }
    clcd_putch(pos%10+'0',LINE2(0));
    
    clcd_putch(log_data[0],LINE2(2));
    clcd_putch(log_data[1],LINE2(3));
    clcd_putch(':',LINE2(4));
    
    clcd_putch(log_data[2],LINE2(5));
    clcd_putch(log_data[3],LINE2(6));
    clcd_putch(':',LINE2(7));
    
    clcd_putch(log_data[4],LINE2(8));
    clcd_putch(log_data[5],LINE2(9));
    //event
    clcd_putch(log_data[6],LINE2(11));
    clcd_putch(log_data[7],LINE2(12));
    
    //speed
    clcd_putch(log_data[8],LINE2(14));
    clcd_putch(log_data[9],LINE2(15)); 
    } 
    
}

char clear_log(unsigned char reset_state)
{
    
    
    if(reset_state==RESET_MEMORY){
        event_count=-1;
        pos=-1;
        clcd_print("Cleared Logs",LINE1(0));
        clcd_print("Successfully",LINE2(0));
        __delay_ms(3000);
        
        return SUCCESS;
        
    }
}
char change_password(unsigned char key, unsigned char reset_state)
{
    static char pwd[9];
    static int pos,pw_entered_once;
    if(reset_state==RESET_CHANGE_PASSWORD)
    {
      pos=0; 
      pw_entered_once=1;
    }
    if(pos<4 && pw_entered_once){
        pw_entered_once=0;
        clcd_print("Enter new password",LINE1(0));
        clcd_write(LINE2(0),INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
        
    }
    else if(pos>=4 && pw_entered_once==0){
        pw_entered_once=1;
        clear_screen();
        clcd_print("Re-enter password",LINE1(0));
        clcd_write(LINE2(0),INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
    }
    
    
    if (key==SW4)//4
    {
        pwd[pos]='4';
        clcd_putch('*',LINE2(pos%4));
        pos++;
    }
    else if(key==SW5)//2
    {
        pwd[pos]='2';
        clcd_putch('*',LINE2(pos%4));
        pos++;
    }
    if(pos==8)
    {
        if(strncmp(pwd,&pwd[4],4)==0)
        {
            //passwords verified
            for(int i=0;i<4;i++){
                eeprom_at24c04_byte_write(i,pwd[i]);
                
            }
            clear_screen();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
            clcd_print("Password changed",LINE1(0));
            clcd_print("Successfully",LINE2(0));
            __delay_ms(3000);
            return SUCCESS;
            
            
        }
        else//not matching
        {
            clear_screen();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
            clcd_print("FAILED",LINE1(0));
            clcd_print("Passwords mismatch!",LINE2(0));
            
            __delay_ms(3000);
            
            return SUCCESS;
        }
        
        
    }
    return 0x10;
}