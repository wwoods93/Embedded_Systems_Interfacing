/* 
 *  Embedded Systems Interfacing
 *  PIC24FJ128GA010
 *  Basic Interrupt
 *  Wilson Woods
 *  Fall 2020
 */

#include "mcc_generated_files/system.h"
#include <stdbool.h>

int dSec = 0, Sec, Min = 0;
bool toggle = 0;

void _ISR _T1Interrupt(void)
{
    dSec++;                     // increment the tenths of a second counter
    if (dSec > 1)               // toggle every 0.2 seconds
    {
        dSec = 0;
        
         if (dSec > 9)
         {
            dSec = 0;
            Sec++;              // increment seconds counter

            if (Sec > 59)       // count up to 1 minute
            {
                Sec = 0;
                Min++;          // increment the minute counter


            }   
    } 
    _T1IF = 0;
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    _T1IP = 4;
    TMR1 = 0;       // clear the timer
    PR1 = 24999;    // set the period register
    TRISA = 0xff00; // set PORTA LSB as output
    T1CON = 0x8020; // enabled, pre-scaler 1:64, internal clock
    _T1IF = 0;      // clear the interrupt flag
    _T1IE = 1;      // enable T1 interrupt source

    while(1)
    {
        
        if (dSec > 3)
        {
            if (toggle == 0)
                toggle = 1;
            else
                toggle = 0;

        }
}