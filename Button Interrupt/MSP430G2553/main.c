#include <msp430.h>

#define LED1 BIT0
#define LED2 BIT6
#define BTN1 BIT3

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

    P1DIR |= LED1;   // out
    P1DIR |= LED2;   // out
    P1DIR &= ~BTN1;  // in
    P1REN |= BTN1;   // enable resistor
    P1OUT |= BTN1;   // pull up

    P1IE |= BTN1;    // enable interrupt for button
    P1IES |= BTN1;   // set as falling edge
    P1IFG &=~ BTN1;  // clear interrupt flag

    //_BIS_SR(LPM4_bits + GIE); //Low power mode enable global interrupt
    __enable_interrupt();
    while(1) // will pause when button is held
    {
        P1OUT ^= LED2;
        __delay_cycles(320000);
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1OUT ^= LED1;
    while (1) // debouncing
    {
        if((P1IN & BTN1) != 0) // if the button is released
        {
            __delay_cycles(1000);
            if((P1IN & BTN1) != 0) // if the button is still released
            {
                P1IFG &= ~(BTN1); // reset interrupt flag
                break;
            }
        }
    } // break after button release
}
