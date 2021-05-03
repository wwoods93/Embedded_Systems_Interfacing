/**
 * Embedded Systems Interfacing
 * PIC24FJ128GA010
 * Write SPI
 * Wilson Woods
 * 11.2.2020 
 */
#include "mcc_generated_files/system.h"

void usDelay(int time)
{
    T1CON = 0x8000;                     // 1:1 pre-scale, 16MHz clock
    TMR1 = 0;                           // clear TMR1
    while (TMR1 < time * 16);           // count input time as microseconds
}

void msDelay(int N)
{
    T1CON = 0x8030;                     // 1:256 pre-scale, 16MHz clock
    TMR1 = 0;                           // clear TMR1
    while (TMR1 < N * 62.5);            // count input N as milliseconds
}


void SPI1Init(void)
{
    SPI1CON1 = 0x0120;
    SPI1STAT = 0x8000;          // enable SPI peripheral
}


unsigned char writeSPI1(unsigned char j)
{
    while (SPI1STATbits.SPITBF);
    SPI1BUF = j;                        // write byte to SPI1BUF
    while (!SPI1STATbits.SPIRBF);       // wait if receive not complete
    return SPI1BUF;                     // read the received value
}

int main(void)
{
    SYSTEM_Initialize();
    SPI1Init();
    TRISB = 0x0000;                     // PORTB as output since RB0 is our SS
    PORTBbits.RB0 = 1;                  // SS active low
    writeSPI1(0x76);                    // reset cursor
    int k;
    while (1)
    {
        PORTBbits.RB0 = 0;              // set SS low
        writeSPI1(0x76);                // reset cursor
        usDelay(500);                   // delay to spread out waveform
        for (k = 1; k < 16; k++)
        {
            PORTBbits.RB0 = 0;              // set SS low
            writeSPI1(k);                   // write 1 to 1st position to begin
            usDelay(500);
            writeSPI1(k + 1);               // write 2 to 2nd position to begin
            usDelay(500);
            writeSPI1(k + 2);               // write 3 to 3rd position to begin
            usDelay(500);
            writeSPI1(k + 3);               // write 4 to 4th position to begin
            usDelay(500);
            PORTBbits.RB0 = 1;              // set SS high
            msDelay(500);
        }
    }

    return 1;
}

