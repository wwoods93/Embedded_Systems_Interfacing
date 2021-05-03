/**
 * Embedded Systems Interfacing
 * PIC24FJ128GA010
 * Toggle LEDs Odd / Even
 * Wilson Woods
 * 9.16.2020
*/

#include "mcc_generated_files/system.h"

void ms_delay( int N );

int main(void) 
{
                                // initialize device
    SYSTEM_Initialize();
    TRISA = 0xff00;             // all pins of Port A are outputs
    T1CON = 0x8030;
    
    while (1)
    {
        PORTA = 0x55;            // odd LEDs on
        ms_delay( 500 );
        PORTA = 0xAA;            // even LEDs on
        ms_delay( 500 );
    }
    return 1;
}

void ms_delay( int N )
{
	double delay = N * 62.5;     // calculate delay from N in ms
	TMR1 = 0;                    // clear timer
	while ( TMR1 < delay );      // while loop to hold program 			
}