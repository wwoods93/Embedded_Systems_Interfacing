/**
 * Embedded Systems Interfacing
 * PIC24FJ128GA010
 * Write I2C
 * Wilson Woods
 * 11.2.2020 
 */

#include "mcc_generated_files/system.h"

void usDelay( int time )
{
    T1CON = 0x8000;                     // 1:1 pre-scale, 16MHz clock
    TMR1 = 0;                           // clear TMR1
    while ( TMR1 < time * 16 );         // count input time as microseconds
}

void msDelay( int N )
{
    T1CON = 0x8030;                     // 1:256 pre-scale, 16MHz clock
    TMR1 = 0;                           // clear TMR1
    while ( TMR1 < N * 62.5 );          // count input N as milliseconds
}

/******************************************************************************/

void I2Cinit( int BRG )
{
    I2C1BRG = BRG;
    while ( I2C1STATbits.P );
    I2C1CONbits.A10M = 0;       // 7-bit address mode
    I2C1CONbits.I2CEN = 1;      // enable module
}

void I2CStart( void )
{
    usDelay( 10 );
    I2C1CONbits.SEN = 1;        // initiate start condition
    while ( I2C1CONbits.SEN );  // wait for start condition complete
    usDelay( 10 );
}

void I2CStop( void )
{
    usDelay( 10 );
    I2C1CONbits.PEN = 1;
    while ( I2C1CONbits.PEN );
    usDelay( 10 );
}

void I2Csendbyte( char data )
{
    while ( I2C1STATbits.TBF ); // wait if buffer is full
    I2C1TRN = data;             // pass data to transmission register
    usDelay( 10 );
}

char I2Cgetbyte( void )
{
    I2C1CONbits.RCEN = 1;           // set RCEN, enables I2C receive mode
    while ( !I2C1STATbits.RBF );    // wait for byte to shift into register
    I2C1CONbits.ACKEN = 1;          // master sends acknowledge
    usDelay( 10 );
    return ( I2C1RCV );
}
/******************************************************************************/

int main( void )
{
    SYSTEM_Initialize();
    I2Cinit( 0x9D );
    usDelay( 500 );
    int k;

    while ( 1 )
    {
        for ( k = 1; k < 16; k++ )
        {
            I2CStart();
            usDelay( 500 );
            I2Csendbyte( 0xE2 );      // send slave address
            usDelay( 500 );
            I2Csendbyte( 0x76 );      // reset cursor
            usDelay( 500 );
            I2Csendbyte( k );         // write 1 to 1st position to begin
            usDelay( 500 );
            I2Csendbyte( k + 1 );     // write 2 to 2nd position to begin
            usDelay( 500 );
            I2Csendbyte( k + 2 );     // write 3 to 3rd position to begin
            usDelay( 500 );
            I2Csendbyte( k + 3 );     // write 4 to right most position to begin
            usDelay( 500 );
            I2CStop();
            msDelay( 500 );
        }
    }

    return 1;
}
