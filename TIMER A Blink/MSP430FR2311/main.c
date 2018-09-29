#include <msp430.h>

#define LED1 BIT0
#define BTN1 BIT3

int skip = 0;

long HzToCount(long);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= LED1;   // out
    P2DIR |= LED1;   // out

    const long count0 = 10; // 5hz * 2

    TB0CCTL0 = CCIE;                    // sets time interrupt
    TB0CCR0 = HzToCount(count0);        // set count
    TB0CTL= MC_3 | TBSSEL_2 | ID_3;     // count up down | SMCLK | 2^1 division

    _BIS_SR(LPM0_bits + GIE); //Low power mode enable global interrupt
}

long HzToCount(long Hz)
{
    const long timerHz = 62500; // 1hz
    return (timerHz/Hz);
}

#pragma vector= TIMER0_B0_VECTOR
__interrupt void Timer_A0 (void)
{
    P1OUT ^= LED1;  //Toggle LED1
    if (skip == 0)
    {
        P2OUT ^= LED1;  //Half LED1 frequency
        skip = 1;
    }
    else
    {
        skip = 0;
    }
}
