/**
 * Embedded Systems Interfacing
 * PIC24FJ128GA010
 * FIR Filter
 * Wilson Woods
 * 11.2.2020 
 */

#include "mcc_generated_files/system.h"

#define N 128           // moving average size power of 2
int PNTR = 0;           // points to sample in buffer

int Sig_Array[ 256 ];     // array for moving average

int Sample = 0;         // individual sample collected from A/D conversion
int MA_Output = 0;      // moving average output
long Summation = 0;     // sum of sample values for moving average

int Flag = 0;           // flag to initiate moving average calculation


void InitADC( int amask )
{
    AD1PCFG = amask;
    AD1CON1 = 0x00E0;
    AD1CON2 = 0;
    AD1CON3 = 0x1F01;
    AD1CSSL = 0;
    AD1CON1bits.ADON = 1;
}

void InitAudio( void )
{
    T3CON = 0x8000;                 // enable TMR3, 1:1, 16 bit Timer
    PR3 = 400 - 1;                  // 16MHz / 400 = 40kHz
    _T3IF = 0;                      // clear interrupt flag
    _T3IE = 1;                      // enable TMR3 interrupt
    OC1R = OC1RS = 8;               // initiate 1% duty cycle
    OC1CON = 0x000E;                // OCTSEL = 1 for Timer3, PWM mode
}

void _ISRFAST _T3Interrupt( void )
{
    AD1CHS = 3;                     // select analog input channel
    AD1CON1bits.SAMP = 1;           // start sampling
    while ( !AD1CON1bits.DONE );    // wait for conversion to complete
    AD1CON1bits.DONE = 0;           // clear flag.
    Sample = ADC1BUF0;              // acquire new A/D sample
    OC1RS = MA_Output >> 1;         // shift right by 1 to scale average output    
    Flag = 1;                       // set flag to indicate new sample available
    _T3IF = 0;                      // clear interrupt flag and exit
}

int main( void ) // main loop
{
    SYSTEM_Initialize();
    InitAudio();
    InitADC( 0xFFF7 );
    while ( 1 )
    {
        while ( Flag )
        {
            Sig_Array[ PNTR ] = Sample;
            PNTR++;
            if ( PNTR >= N )
                PNTR = 0;
            Summation = Summation + Sample;
            Summation = Summation - Sig_Array[ PNTR ];
            MA_Output = Summation >> 7;
            Flag = 0;
        }
    }
    return 1;
}
