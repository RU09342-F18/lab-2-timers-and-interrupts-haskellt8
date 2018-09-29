#include <msp432.h>

#define P1LED1 BIT0
#define P2LED1 BIT2
#define BTN1 BIT1

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop watchdog timer

    P1DIR |= P1LED1; // out
    P2DIR |= P2LED1; // out
    P1DIR &= ~BTN1;  // in
    P1REN |= BTN1;   // enable resistor
    P1OUT |= BTN1;   // pull up
    P1SEL0 = 0;      // mux bit0 button
    P1SEL1 = 0;      // mux bit1 button

    P1IE |= BTN1;    // enable interrupt button
    P1IES |= BTN1;   // set as falling edge
    P1IFG &= ~BTN1;  // clear interrupt flag
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);   //enable port 1 interrupt

    __enable_irq();  // enable interrupts
    while(1) // will pause when button is held
    {
        P2OUT ^= P2LED1;
        __delay_cycles(320000);
    }
}

//interrupt routine
void PORT1_IRQHandler(void)
{
    P1OUT ^= P1LED1;
    while (1) // debouncing
    {
        if((P1IN & BTN1) != 0) // if the button is released
        {
            __delay_cycles(1000);
            if((P1IN & BTN1) != 0) // if the button is still released
            {
                P1IFG &= ~(BTN1); // reset interrupt flag
                // when flag is reset on the MSP432, P1IES resets
                P1IES |= BTN1; // reset as falling edge
                break;
            }
        }
    } // break after button release
}
