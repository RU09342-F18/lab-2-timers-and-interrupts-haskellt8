# Button Interrupt
It is energy inefficient to use a for loop to delay time on a microprocessor.  Rather, it is better to use interrupts.  When the processor is in low power mode with interrupts enabled, it is able to wake up at the interrupt trigger, and execute code, then go back to low power mode.  In addition, it also makes it easier to stop where you are in your code, execute another function, and then return.  This can be done at the push of a button.  The other option is to constantly check to see if the button is pressed or not. Interrupt is more reliable, saves more power, and is faster than the alternative.

```c
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
```

The above code is the interrupt function for the button interrupt.  This code at the down press of the button is triggered.  The code is in a for loop to wait for the button to be released before moving on.  (This is not the most efficient way of doing it, as timer methods can be used.)  After the button is released, the processor waits 1000 cycles and checks the button again to make sure it is still up.  This is incase of any hardware bounces on the button (incase the button bounces back down after it is released).  

This program flashes an LED in a while loop.  When the button is pressed down, the one LED stops flashing, and the other LED fips once.  When the button is released, the first LED continues to flash again.
