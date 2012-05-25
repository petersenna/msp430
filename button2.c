#include <msp430x20x2.h>

#define REDLED		BIT0
#define GREENLED	BIT6
#define BUTTON		BIT3

//Delay is DELAY * DELAX
#define DELAY		1
#define DELAX		7500

void delay(void);
void green(void);
void init_port1(void);
void red(void);

int main(void) {
	int button_status = 0x00;	// 0x00 - released
					// 0x01 - pressed
	init_port1();

	P1OUT = REDLED;			// Turn on only REDLED

	for(;;) {
		// If button is pressed
		if (!((P1IN & BUTTON) == BUTTON))
			button_status ^= 0x01;	// Switch state

		if (button_status == 0x01)
			green();
		else
			red();
		delay();
	}
}

void delay(void)
{
	volatile unsigned int i, j;	// Avoid compiler optimization to
					// allow using counter as busy
					// wait loop

	for (i = 0; i <= DELAY; i++)	// Busy wait loop DELAY * DELAX
		for (j = 0; j <= DELAX; j++) {}
}

void green(void)
{
	P1OUT = GREENLED;
}

void init_port1(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer

	P1DIR = 0x00;			// Set all ports to input direction
					// This is not needed

	P1DIR |= REDLED;		// Set P1.0 to output direction
	P1DIR |= GREENLED;		// Set P1.6 to output direction
}

void red(void)
{
	P1OUT = REDLED;
}

