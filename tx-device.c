#include <msp430x20x2.h>
#include <signal.h>

#define REDLED		BIT0
#define GREENLED	BIT6
#define BUTTON		BIT3

#define TX		BIT1
#define RX		BIT2

#define DELAY		3500

#define DCOCTL_MASK     0xe0    //0b11100000
#define BCSCTL1_MASK    0x0f    //0b00001111
#define DCO3(rsel,dco) {\
        if ((rsel >= 0) && (rsel <= 15) && (dco >= 0) && (dco <= 7)){\
                BCSCTL1 = ((BCSCTL1 & ~BCSCTL1_MASK) | (rsel));\
                DCOCTL = ((DCOCTL & ~DCOCTL_MASK) | (dco << 5));\
        }\
}

void init_p1(void);
void delay(void);

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer

	DCO3(0,3);

	init_p1();

	// Config interrupts
	P1IES |= BUTTON;		// high -> low
	P1IES &= ~RX;			// low -> high

	P1IFG &= ~TX;
	P1IFG &= ~BUTTON;		// To prevent an immediate interrupt, clear the
					// flag before enabling the interrupt
	P1IE |= BUTTON;
	P1IE |= RX;
	eint();

	for (;;);

	return 0;
}

void delay(void)
{
	volatile unsigned int n;
	for (n = 0; n < DELAY; n++);
}

void init_p1(void)
{
	P1DIR = 0;			// Set all to 0

	P1OUT = 0;			// Turn off both LEDS and TX

	P1DIR |= REDLED;		// Set P1.0 to output direction
	P1DIR |= TX;			// Set P1.1 to output direction
	P1DIR |= GREENLED;		// Set P1.6 to output direction
}

interrupt(PORT1_VECTOR) button_press (void) {
	static int bp = 0;

	delay();
	if ((P1IFG & BUTTON) == BUTTON){
		P1IFG &= ~BUTTON;

		P1OUT &= ~GREENLED;

		P1OUT |= REDLED;
		P1OUT |= TX;

		bp = 1;

		return;
	}

	if((P1IFG & RX) == RX){
		P1IFG &= ~RX;

		P1OUT &= ~REDLED;
		P1OUT &= ~TX;
		if (bp == 1){
			P1OUT |= GREENLED;
			delay();
			P1OUT &= ~GREENLED;
			bp = 0;
		}
		return;
	}
}
