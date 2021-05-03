/* 
 *  Embedded Systems Interfacing
 *  PIC24FJ128GA010
 *  Read / Write LCD
 *  Wilson Woods
 *  Fall 2020
 */

#include "mcc_generated_files/system.h"

#define putLCD( d )     WriteLCD( 1, ( d ) )
#define BusyLCD()       ReadLCD( 0 ) & 0x80
#define CmdLCD( c )     WriteLCD( 0, ( c ) )
#define HomeLCD()       WriteLCD( 0, 2 )
#define ClrLCD()        WriteLCD( 0, 1 )
#define AddrLCD()       ReadLCD( 0 ) & 0x7F
#define getLCD()        ReadLCD( 1 )



void ms_delay( int N )
{
	TMR1 = 0;                         // clear timer
	while ( TMR1 < N * 62.5 );        // while loop to hold program 			
}

char ReadLCD( int addr )
{
    int dummy;
    while ( PMMODEbits.BUSY );        // wait for PMP to be available
    PMADDR = addr;                    // select the command address
    dummy = PMDIN1;                   // initiate a read cycle, dummy
    while ( PMMODEbits.BUSY );        // wait for PMP to be available
    return ( PMDIN1 );                // read the status register
}

void SetCursorAtLine( int i )
{
    if ( i == 1 )
        WriteLCD( 0, 0x00 );
    else if ( i == 2 )
        WriteLCD( 0, 0x40 );
    else
    {
        int n = 0;
        while ( n < 20 )
        {
            PORTA = 0xff;
            ms_delay( 100 );
            PORTA = 0x00;
            ms_delay( 100 );
            n++;
        }
    }     
}

void WriteLCD( int addr, char c)
{
    while ( BusyLCD() );
    while( PMMODEbits.BUSY );            // wait for PMP to be available
    PMADDR = addr;
    PMDIN1 = c;
}

void putsLCD( char *s )
{
    while( *s ) putLCD( *s++ );
}



void InitPMP( void )
{
    PMCON = 0x8303;
    PMMODE = 0x03FF;
    PMAEN = 0x0001;
}

void InitLCD( void )
{
    PMADDR = 0;
    PMDIN1 = 0b00111000;
    ms_delay( 1 );
    PMDIN1 = 0b00001111;
    ms_delay( 1 );
    PMDIN1 = 0b00000001;
    ms_delay( 2 );
    PMDIN1 = 0b00000110;
    ms_delay( 2 );
}

int main(void)
{
    SYSTEM_Initialize();
    TRISA = 0xff00;
    T1CON = 0x8030;
    ms_delay( 32 );
    InitPMP();
    InitLCD();
    //SetCursorAtLine(1);
    //ms_delay(500);
    //putsLCD("Hello World");
    
    while ( 1 )
    {
        WriteLCD( 0x0000, 'A' );
    }
    return 1;
}

