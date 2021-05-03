/**
 * Embedded Systems Interfacing
 * PIC24FJ128GA010
 * Basic Push Buttons
 * Wilson Woods
 * 11.2.2020 
 */
#include "mcc_generated_files/system.h"

#define SCALE 1000L
int main(void)
{
    SYSTEM_Initialize();
    unsigned long i;
    unsigned char display = 0;
    PORTA = 0x000;                      //clear port A
    TRISA = 0xFF00;                     // set PORTA <7:0> to output
    TRISD = 0xFFFF;                     // set PORTD to input
    // endless loop
    while (1)
    {
        Nop();
        PORTA = (unsigned int) display;
        for (i = 1001L * SCALE; i > 0; i--) Nop();
        if (PORTDbits.RD13 == 0)
            display = display + 1;      // if S4 pressed increment counter
        else if (PORTDbits.RD6 == 0)
            display = 0;                // if S3 pressed reset counter
        else
            display = display - 1;      // decrement display counter
    }
    return 0;
}
