/**
 * Embedded Systems Interfacing
 * PIC24FJ128GA010
 * Toggle LEDs
 * Wilson Woods
 * 9.9.2020
 */
#include "mcc_generated_files/system.h"

int main(void)
{
    SYSTEM_Initialize();
    PORTA = 0x0000;           // clear PORTA
    TRISA = 0x00;             // set PORTA to output (LEDs)
    TRISD = 0xffff;           // set PORTD to input (buttons)

    while (1)
    {
        if (PORTDbits.RD6 == 0 && PORTDbits.RD13 == 0)  // check both buttons
            PORTA = 0xff;                               // All LEDs on
        else
            PORTA = 0x00;                               // All LEDs off
    }

    return 1;
}


