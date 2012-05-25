#include <msp430x20x2.h>

#define REDLED		BIT0
#define GREENLED	BIT6
#define DELAY		32000

int main(void) {
	volatile unsigned int i = DELAY; // Avoid compiler optimization to
					 // allow using counter as busy
					 // wait loop

	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1DIR |= REDLED;		// Set P1.0 to output direction
	P1DIR |= GREENLED;		// Set P1.6 to output direction

	P1OUT = REDLED;			// Turn on REDLED

	for(;;) {
		i = DELAY;

		P1OUT ^= REDLED;	// Switch state of REDLED
		P1OUT ^= GREENLED;	// Switch state of GREENLED

		do			// Busy wait loop for delay
			i--;
		while (i > 0);
	}
}
