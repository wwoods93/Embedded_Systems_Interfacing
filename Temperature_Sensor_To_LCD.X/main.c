/**
 * Embedded Systems Interfacing
 * PIC24FJ128GA010
 * Temperature Sensor with LCD
 * Wilson Woods
 * 11.2.2020 
 */


#include "mcc_generated_files/system.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****************************** LCD definitions *******************************/

#define putLCD( d) WriteLCD( 1, (d))
#define BusyLCD() ReadLCD( 0) & 0x80
#define CmdLCD( c) WriteLCD( 0, (c))
#define HomeLCD() WriteLCD( 0, 2)
#define ClrLCD() WriteLCD( 0, 1)

/******************************************************************************/
int temp = 0, potV = 0;

void InitADC(int amask)
{
    AD1PCFG = amask;
    AD1CON1 = 0x00E0;
    AD1CON2 = 0;
    AD1CON3 = 0x1F01;
    AD1CSSL = 0;
    AD1CON1bits.ADON = 1;
}

int ReadADC(int ch)
{
    AD1CHS = ch;
    AD1CON1bits.SAMP = 1;
    while (!AD1CON1bits.DONE);
    AD1CON1bits.DONE = 0;
    return ADC1BUF0;
}

void ms_delay(int ms)
{
    T2CON = 0x8030;
    TMR2 = 0;
    while (TMR2 < ms * 62.5);
}
/******************************* LCD Functions ********************************/

char ReadLCD(int addr)
{
    int dummy;
    while (PMMODEbits.BUSY);
    PMADDR = addr;
    dummy = PMDIN1;
    while (PMMODEbits.BUSY);
    return ( PMDIN1);
}

void SetCursorAtLine(int i)
{
    int k;
    if (i == 1)
        CmdLCD(0x80);
    else if(i == 2)
        CmdLCD(0xC0);
    else
    {
        TRISA = 0x00;         
        for (k = 1; k < 20; k++)    
        {   
            PORTA = 0xFF;
            ms_delay(100);
            PORTA = 0x00;
            ms_delay(100);
        } 
    } 
}

void WriteLCD( int addr, char c)
{
    while (BusyLCD());
    while( PMMODEbits.BUSY);
    PMADDR = addr;
    PMDIN1 = c;
}

void putsLCD( char *s)
{
    while( *s) putLCD( *s++);
}

void InitPMP(void)
{
    PMCON = 0x8303;
    PMMODE = 0x03FF;
    PMAEN = 0x0001;
}

void InitLCD(void)
{
    PMADDR = 0;
    PMDIN1 = 0b00111000;
    ms_delay(1);
    PMDIN1 = 0b00001111;
    ms_delay(1);
    PMDIN1 = 0b00000001;
    ms_delay(2);
    PMDIN1 = 0b00000110;
    ms_delay(2);
}

/******************************************************************************/

main()
{

    SYSTEM_Initialize();
    
    InitPMP();
    InitLCD();
    
    float quant_res, Vout, T_degC, T_degF;
    int i;
    int N = 4;
    quant_res = 3.3/1024.0;
    InitADC(0xFFEF);
    TRISA = 0xff00;
    
    while (1) {
        temp = 0;
        for (i = 0; i < (int) pow(2, N); i++)
        {
            temp += ReadADC(4);
            ms_delay(125);
        }
        temp = temp >> N;
        Vout = temp * quant_res; 
        T_degC = (Vout - .5) * 100;
        PORTA = T_degC;
        ms_delay(500);
        
        char LCDchar[8];
        sprintf(LCDchar, "%0.4f", T_degC);
        ms_delay(32);
        
        SetCursorAtLine(1);
        putsLCD("Temperature is : ");
        SetCursorAtLine(2);
        putsLCD(LCDchar);
    }
    
}

