/* 
 *  Embedded Systems Interfacing
 *  PIC24FJ128GA010
 *  Temperature Sensor Using ADC
 *  Wilson Woods
 *  Fall 2020
 */
#include "mcc_generated_files/system.h"
#include <math.h>

void InitADC( int amask )
{
    AD1PCFG = amask;                        // select analog input pins
    AD1CON1 = 0x00E0;                       // auto convert after sampling
    AD1CON2 = 0;
    AD1CON3 = 0x1F01;
    AD1CSSL = 0;                            // no scanning required
    AD1CON1bits.ADON = 1;                   // turn on the ADC
}

int ReadADC( int ch )
{
    AD1CHS = ch;                            // select analog input channel
    AD1CON1bits.SAMP = 1;                   // start sampling
    while ( !AD1CON1bits.DONE );            // wait for conversion to complete
    AD1CON1bits.DONE = 0;                   // clear flag
    return ADC1BUF0;                        // read the conversion results
}

void ms_delay( int ms )
{
    T2CON = 0x8030;
    TMR2 = 0;
    while ( TMR2 < ms * 62.5 );
}


int main( void )
{
    SYSTEM_Initialize();                      // initialize the device
    float quant_res, Vout, T_degC, T_degF;
    int temp;
    int N = 3;
    quant_res = 3.3 / 1024.0;                 // max Vin / num bits
    InitADC( 0xFFEF );                        // init the ADC and analog inputs
    TRISA = 0xff00;                           // all PORTA pins as outputs
    int i;
    while( 1 )
    {
        temp = 0;
        for ( i = 0; i < (int) pow( 2, N ); i++ )
        {
            temp += ReadADC( 4 );               // read temperature sensor on CH4
            ms_delay( 125 );
        }
        temp = temp >> N;                       // right shift by N -> divide by 2^N
        Vout = temp * quant_res; 
        T_degC = ( Vout - .5 ) * 100;           // from slide 19 lecture 7
        T_degF = ( ( 1.8 * T_degC ) + 32 );     // convert C to F
        PORTA = (int) T_degF;                   // display T_degF on LEDs
        ms_delay( 500 );                        // 0.5 sec between iterations
    }
    return 1;
}
