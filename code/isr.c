/*
 * File:   isr.c
 * Author: vinodhini
 *
 * Created on 14 March, 2024, 9:52 PM
 */


#include <xc.h>
#include "main.h"
unsigned char seconds,return_time;

void __interrupt() isr(void)
{
    static unsigned int count = 0;
    
    if (TMR2IF == 1)
    {
        if (++count == 1250) // 1sec
        {
            count = 0;
            if(seconds>0){
                seconds--;
                
            }
            else if(seconds==0 && return_time>0){
                return_time--;
            }
            
        }
        
        TMR2IF = 0;
    }
}
