#include <msp430x20x2.h>

#define REDLED		BIT0
#define GREENLED	BIT6
#define BUTTON		BIT3

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1DIR |= REDLED;		// Set P1.0 to output direction
	P1DIR |= GREENLED;		// Set P1.6 to output direction

	P1OUT = 0x00;			// Turn off both LEDS

	for(;;) {
		if ((P1IN & BUTTON) == BUTTON)
			P1OUT = REDLED;	// BUTTON RELEASED
		else
			P1OUT = GREENLED; // BUTTON PRESSED
	}
}
