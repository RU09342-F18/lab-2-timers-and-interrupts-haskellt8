#include <msp430.h>

#define LED1 BIT0
#define BTN1 BIT1

int offset = 0;
int offcount = 0;
int btnstate = 0;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

    P1DIR |= LED1;   // out
    P1DIR &= ~BTN1;  // in
    P1REN |= BTN1;   // enable resistor
    P1OUT |= BTN1;   // pull up

    TA0CCTL0 = CCIE;                    // sets time interrupt
    TA0CCR0 = 6250;                     // set 10Hz/2
    TA0CTL= MC_1 | TASSEL_2 | ID_3;     // count up | SMCLK | 2^1 division

    _BIS_SR(LPM0_bits + GIE); //Low power mode enable global interrupt
}

#pragma vector= TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    if (((P1IN & BTN1) == 0) && (btnstate == 0)) // if the button is pressed
    {
        offset = 0;
        offcount = 0;
        btnstate = 1;
        P1OUT &= ~LED1;  // LED1 off
    }
    else if ((P1IN & BTN1) == 0) // if the button is pressed
    {
        offset = offset + 1;
    }
    else if (btnstate == 1)
    {
        btnstate = 0;
    }
    else if(offcount == offset)
    {
        P1OUT ^= LED1;  //Toggle LED1
        offcount = 0;
    }
    else
    {
        offcount = offcount + 1;
    }
}
