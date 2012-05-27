/* Demonstration of interrupt of button pressing
 * When Button is pressed BIT3 of PORT1 goes from 1 to 0 and trigger
 * the interrupt handler button_press (void) that switch the clock
 * between DCO3(6,3) and DCO3(10,3)
 *
 * Note that this implemmentation is for msp430-gcc.
 * More at: http://mspgcc.sourceforge.net/manual/
 */

#include <msp430x20x2.h>
#include <signal.h>

#define REDLED		BIT0
#define GREENLED	BIT6
#define BUTTON		BIT3
#define DCOCTL_MASK 	0xe0	//0b11100000
#define BCSCTL1_MASK	0x0f	//0b00001111

#define DCO3(rsel,dco) {\
	if ((rsel >= 0) && (rsel <= 15) && (dco >= 0) && (dco <= 7)){\
		BCSCTL1 = ((BCSCTL1 & ~BCSCTL1_MASK) | (rsel));\
		DCOCTL = ((DCOCTL & ~DCOCTL_MASK) | (dco << 5));\
	}\
}

void blink(unsigned int led, unsigned int times);
void delay(void);

int main (void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	P1OUT = 0;                  
	P1DIR = GREENLED;

	P1IES |= BUTTON;	// high -> low is selected with IES.x = 1

	P1IFG &= ~BUTTON;	// To prevent an immediate interrupt, clear the
				// flag for BUTTON before enabling the interrupt

	P1IE |= BUTTON;		// Enable interrupts for P1.3

	eint();			// Enable Interrupts

	DCO3(6,3);

	for (;;){
		blink(GREENLED, 5);
	}
}

void blink(unsigned int led, unsigned int times)
{
	for (; times > 0; times--){
		P1OUT |= led;
		delay();
		P1OUT &= ~led;
		delay();
	}
}

void delay(void)
{
	volatile unsigned int n;
	for (n = 0; n < 60000; n++);
}

interrupt(PORT1_VECTOR) button_press (void) {
	static int i = 1;	// By calling it static, the value is saved
				// Between function calls. It is initialized
				// only once

	P1IFG &= ~BUTTON;	// Clear the interrupt flag

	i ^= 1;			// For switching i between 1 and 0

	if (i == 0){
		DCO3(10,3);
	} else {
		DCO3(6,3);
	}
}
