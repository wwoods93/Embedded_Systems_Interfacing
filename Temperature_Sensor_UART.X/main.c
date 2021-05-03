/**
 * Embedded Systems Interfacing
 * PIC24FJ128GA010
 * Temperature Sensor UART
 * Wilson Woods
 * 11.2.2020 
 */
#include "mcc_generated_files/system.h"
#include <stdio.h>

#define RTS _RF13                       // output
#define CTS _RF12                       // input

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
    while (TMR1 < N * 62.5);            // count input N in ms
}

/******************************************************************************/


void InitU2(void)
{
    U2BRG = 34;
    U2MODE = 0x8008;
    U2STA = 0x0400;
    TRISFbits.TRISF13 = 1;
    RTS = 1;
}

char putU2(char c)
{
    while (CTS);
    while (U2STAbits.UTXBF);
    U2TXREG = c;
    return c;
}

char getU2(void)
{
    RTS = 0;
    while (!U2STAbits . URXDA);
    RTS = 1;
    return U2RXREG;
}




/******************************************************************************/

void I2Cinit(int BRG)
{
    I2C1BRG = BRG;
    while (I2C1STATbits.P);
    I2C1CONbits.A10M = 0;
    I2C1CONbits.I2CEN = 1;
}

void I2CStart(void)
{
    usDelay(10);
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN);
    usDelay(10);
}

void I2CStop(void)
{
    usDelay(10);
    I2C1CONbits.PEN = 1;
    while (I2C1CONbits.PEN);
    usDelay(10);
}

void I2Csendbyte(char data)
{
    while (I2C1STATbits.TBF);
    I2C1TRN = data;
    usDelay(10);
}

char I2Cgetbyte(void)
{
    I2C1CONbits.RCEN = 1;
    while (!I2C1STATbits.RBF);
    I2C1CONbits.ACKEN = 1;
    usDelay(10);
    return (I2C1RCV);
}
/******************************************************************************/

int main(void)
{
    SYSTEM_Initialize();
    TRISA = 0x00;
    PORTA = 0x00;
    
    InitU2();
    I2Cinit(0x9D);
    usDelay(10);
    
    char byte1, byte2;
    char str[40];
    char Hi_Byte;
    int d1, d2, d3, d4;
    int k, length;                        // for ascii output
    int Count = 1;
    while (1)
    {
        I2CStart();
        usDelay(100);
        I2Csendbyte(0x90);                // tmp102 write address
        usDelay(100);
        I2Csendbyte(0x00);                // pointer register byte
        usDelay(100);
        I2CStop();                        // stop after pointer register access
        
        I2CStart();                       // start before reading sensor
        usDelay(100);
        I2Csendbyte(0x91);
        usDelay(100);
        byte1 = I2Cgetbyte();             // read sensor
        usDelay(100);
        byte2 = I2Cgetbyte();             // read sensor
        usDelay(100);
        I2CStop();                        // stop after read        
        
        PORTA = byte1;
        
        // Hi_Byte = byte1;
        d1 = byte1 / 1000;
        byte1 = byte1 % 1000;
        d2 = byte1 / 100;
        byte1 = byte1 % 100;
        d3 = byte1 / 10;
        d4 = byte1 % 10;
        
        
        I2CStart();
        usDelay(100);
        I2Csendbyte(0xE2);                // send 7-seg slave address
        usDelay(100);
        I2Csendbyte(0x76);                // reset cursor
        usDelay(100);
        I2Csendbyte(d1);
        usDelay(100);
        I2Csendbyte(d2);
        usDelay(100);
        I2Csendbyte(d3);
        usDelay(100);
        I2Csendbyte(d4);
        usDelay(100);
        I2CStop();


        length = sprintf(str, "Count = %d", Count);
        
        for (k = 0; k < length; k++) {
            putU2(str[k]);                  // output message on UART 2
        }

        length = sprintf(str, "  Deg. C = %d", byte1);
        
        for (k = 0; k < length; k++) {
            putU2(str[k]);                  // output message array on UART 2
        }
        putU2(0x0D);                        // output carriage return on UART 2
        putU2(0x0A);                        // output line feed on UART 2

        
        Count++;
        msDelay(500);
        
    }

    return 1;
}