#include <msp430.h>

#define LED1 BIT0
#define LED2 BIT6
#define BTN1 BIT3

int skip = 0;

long HzToCount(long);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

    P1DIR |= LED1;   // out
    P1DIR |= LED2;   // out

    const long count0 = 10; // 5hz * 2

    TA0CCTL0 = CCIE;                    // sets time interrupt
    TA0CCR0 = HzToCount(count0);        // set count
    TA0CTL= MC_3 | TASSEL_2 | ID_3;     // count up down | SMCLK | 2^1 division

    _BIS_SR(LPM0_bits + GIE); //Low power mode enable global interrupt
}

long HzToCount(long Hz)
{
    const long timerHz = 62500; // 1hz
    return (timerHz/Hz);
}

#pragma vector= TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    P1OUT ^= LED1;  //Toggle LED1
    if (skip == 0)
    {
        P1OUT ^= LED2;  //Half LED1 frequency
        skip = 1;
    }
    else
    {
        skip = 0;
    }
}
