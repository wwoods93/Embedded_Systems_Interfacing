/**
 * Embedded Systems Interfacing
 * Blink LED
 * Wilson Woods
 * PIC24FJ128GA010
 * 8.31.2020
*/

#include "mcc_generated_files/system.h"

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TRISB = 0;                  // Set all port B pins as output
    AD1PCFG = 0xffff;           // Set all port B pins as digital

    while (1)
    {
        PORTB = 0xff;           // toggle high
        PORTB = 0x00;           // toggle low
    }
    return 1;
}
