#include <msp430x20x2.h>

#define REDLED		BIT0
#define GREENLED	BIT6
#define DELAY		32000

void delay(void);
void init_port1(void);
void switch_state(void);

int main(void) {

	init_port1();

	P1OUT = REDLED;			// Turn on REDLED

	for(;;) {
		switch_state();
		delay();
	}
}

void delay(void)
{
	volatile unsigned int i = 0;	// Avoid compiler optimization to
					// allow using counter as busy
					// wait loop	
	i = DELAY;

	do				// Busy wait loop for delay
		i--;
	while (i > 0);
}

void init_port1(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1DIR |= REDLED;		// Set P1.0 to output direction
	P1DIR |= GREENLED;		// Set P1.6 to output direction
}

void switch_state(void)
{
	P1OUT ^= REDLED;		// Switch state of REDLED
	P1OUT ^= GREENLED;		// Switch state of GREENLED
}
