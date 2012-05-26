/*
 * Based on:
 * http://sites.google.com/site/mspscifiles/tutorials/dcodemo_g2211.c?attredirects=0&d=1
 *
 * Description at:
 * http://mspsci.blogspot.com.br/2010/07/tutorial-08-b-configuring-dco.html
 *
 * Demonstrates configuration of the DCO by showing changing LED flash frequency.
 * The LED flash uses the same delay loop, but the speed at which the loop runs
 * depends on the DCO frequency running MCLK.
 *
 * int dco(unsigned int rsel, unsigned int dco) - consumes 4 ints of memory but
 * it is easy to understand.
 *
 * int dco2(unsigned int rsel, unsigned int dco) - consumes 2 insts of memory
 * but is a little tricky.
 *
 * #define DCO3(rsel,dco) - consumes no memory but is tricky and may be 
 * dangerous
 *
 */

#include <msp430x20x2.h>

#define REDLED		BIT0
#define GREENLED	BIT6
#define BUTTON		BIT3
#define DCOCTL_MASK 	0xe0	//0b11100000
#define BCSCTL1_MASK	0x0f	//0b00001111

#define DCO3(rsel,dco) {\
	if (!((rsel < 0) || (rsel > 15) || (dco < 0) || (dco > 7))){\
		BCSCTL1 = ((BCSCTL1 & ~BCSCTL1_MASK) | (rsel));\
		DCOCTL = (DCOCTL & ~DCOCTL_MASK) | (dco << 5);\
	} else \
		blink(REDLED, 2);\
}

void blink(unsigned int led, unsigned int times);
int dco(unsigned int rsel, unsigned int dco);
int dco2(unsigned int rsel, unsigned int dco);
void delay(void);

/*
 * if (dco(2, 6) < 0)
 *	blink(REDLED, 2); // Error!
 *
 * blink(GREENLED, 5);
 * while ((P1IN&BUTTON) == BUTTON); // wait for button press
 *
 */
int main(void) {
	WDTCTL = WDTPW + WDTHOLD;
	
	P1OUT = 0;
	P1DIR = REDLED+GREENLED;
	
	for (;;) {
		if (dco(11, 3) < 0)
			blink(REDLED, 2);

		blink(GREENLED, 5);
		while ((P1IN&BUTTON) == BUTTON);

		if (dco2(11, 3) < 0)
			blink(REDLED, 2);

		blink(GREENLED, 5);
		while ((P1IN&BUTTON) == BUTTON);

		DCO3(11, 3);

		blink(GREENLED, 5);
		while ((P1IN&BUTTON) == BUTTON);

		if (dco(20, 8) < 0)
			blink(REDLED, 2);
		while ((P1IN&BUTTON) == BUTTON);

		if (dco2(20, 8) < 0)
			blink(REDLED, 2);
		while ((P1IN&BUTTON) == BUTTON);

		DCO3(20, 8);
		while ((P1IN&BUTTON) == BUTTON);
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

/* From: http://www.ti.com/lit/ds/symlink/msp430g2231.pdf page 23
 * This is chip specific. For the MSP430G2231 using 3V at 25C:
 *
 * dco(0,0) = [0.06 , 0.14] Mhz
 * dco(0,3) = 0.12 Mhz
 * dco(1,3) = 0.15 Mhz
 * dco(2,3) = 0.21 Mhz
 * dco(3,3) = 0.30 Mhz
 * dco(3,3) = 0.30 Mhz
 * dco(4,3) = 0.41 Mhz
 * dco(5,3) = 0.58 Mhz
 * dco(6,3) = 0.80 Mhz
 * dco(7,3) = [0.8 , 1.5] Mhz
 * dco(8,3) = 1.6 Mhz
 * dco(9,3) = 2.3 Mhz
 * dco(10,3) = 3.4 Mhz
 * dco(11,3) = 4.25 Mhz
 * dco(12,3) = [4.3 , 7.3] Mhz
 * dco(13,3) = 7.8 Mhz
 * dco(14,3) = [8.6 , 13.9] Mhz
 * dco(15,3) = 15.25 Mhz
 * dco(15,7) = 21.00 Mhz
 * 
 */

/* This is easyer to understand but consumes 4 ints of memory */
int dco(unsigned int rsel, unsigned int dco)
{
	unsigned int bcsctl1, dcoctl;

	/* Check if parameters are in correct range */
	if ((rsel < 0) || (rsel > 15) || (dco < 0) || (dco > 7))
		return -1;

	/* Shift dco so the 3 less significant bits
	 * become the 3 more signigicant bits */
	dco = dco << 5;

	/* read the registers */
	bcsctl1 = BCSCTL1;
	dcoctl = DCOCTL;

	/* set zeros */
	bcsctl1 &= (rsel & BCSCTL1_MASK);
	dcoctl  &= (dco & DCOCTL_MASK);

	/* set ones */
	bcsctl1 |= (rsel & BCSCTL1_MASK);
	dcoctl  |= (dco & DCOCTL_MASK);

	/* save it back to the registers */
	BCSCTL1 = bcsctl1;
	DCOCTL = dcoctl;

	return 0;
}

/* This is more tricky but consumes only 2 ints of memory */
int dco2(unsigned int rsel, unsigned int dco)
{
	/* Check if parameters are in correct range */
	if ((rsel < 0) || (rsel > 15) || (dco < 0) || (dco > 7))
		return -1;

	/* Shift dco so the 3 less significant bits
	 * become the 3 more signigicant bits */
	dco = dco << 5;

	/* We want to change only the bits described by BCSCTL1_MASK and 
	 * DCOCTL_MASK. Using |= (BCSCTL1 & ~BCSCTL1_MASK) and 
	 * |= (DCOCTL & ~DCOCTL_MASK) only reads the bits we will not change.
	 */
	rsel |= (BCSCTL1 & ~BCSCTL1_MASK);
	dco  |= (DCOCTL & ~DCOCTL_MASK);

	/* save it back to the registers */
	BCSCTL1 = rsel;
	DCOCTL = dco;

	return 0;
}

void delay(void)
{
	unsigned int n;
	for (n = 0; n < 60000; n++);
}
