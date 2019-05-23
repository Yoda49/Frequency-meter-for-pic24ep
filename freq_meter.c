#include "P24EP512GP806.h"

unsigned int freq_falue;

void ic1_init (void)
{
	TRISDbits.TRISD2 = 1; // IC1
	RPINR7bits.IC1R = 0b1000010; // RP66
	
	IC1CON1bits.ICSIDL = 0;
	
	IC1CON1bits.ICTSEL = 0b011; // timer 5
	IC1CON1bits.ICI = 0b01; // generate interrupt every 2 capture event

	IC1CON1bits.ICM = 0b101; // Input frequency prescaler 1:16
	
	IPC0bits.IC1IP = 0b01; // Setup IC1 interrupt priority level
	IFS0bits.IC1IF = 0;    // Clear IC1 Interrupt Status Flag
	IEC0bits.IC1IE = 1;    // Enable IC1 interrupt
}

void timer5_init ()
{
    TMR5 = 0;
    PR5  = 65535;
    
    IFS1bits.T5IF = 0;
    IPC7bits.T5IP = 0b010; // low priority  
    IEC1bits.T5IE = 0;
    
    T5CONbits.TCS   = 0;
    T5CONbits.TCKPS = 0b11;
    T5CONbits.TGATE = 0;
    T5CONbits.TSIDL = 0;
    T5CONbits.TON   = 1;
}


void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt(void)
{
	unsigned int t1 = IC1BUF;
	unsigned int t2 = IC1BUF;
	
	IFS0bits.IC1IF = 0;
	if (t2 > t1) freq_value = t2 - t1; else freq_value = (65535 - t1) + t2;
  
  freq_value = 3751000 / freq_value; // timer frequency divided by period
}
