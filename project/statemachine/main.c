#include <msp430.h>
#include "libTimer.h"

#define LED_RED BIT0               // P1.0
#define LED_GREEN BIT6             // P1.6
#define LEDS (BIT0 | BIT6)

#define SW1 BIT0		// switch1 is p2.0
#define SW2 BIT1                // switch2 is p2.1
#define SW3 BIT2                // switch3 is p2.2
#define SW4 BIT3                // switch4 is p2.3
#define SWITCHES (SW1 | SW2 | SW3 | SW4) // 4 switches on this board

void led_init(){
  P1DIR |= LEDS;                // Sets P1DIR to output
  P1OUT &= ~LEDS;		// LEDs initially off 
}

void switch_init(){
  P2REN |= SWITCHES;		// enables resistors for switches 
  P2IE |= SWITCHES;		// enable interrupts from switches 
  P2OUT |= SWITCHES;		// pull-ups for switches 
  P2DIR &= ~SWITCHES;		// set switches' bits for input 
}

void wdt_init(){
  configureClocks();            // setup master oscillator, CPU & peripheral clocks
  //enableWDTInterrupts();        // Enable periodic interrupts
}

void main(void){  
  led_init();                   // Initialize LEDS
  switch_init();                // Initialize Switches 
  wdt_init();                   // initialize WatchDogTimer
  
  or_sr(0x18);            // CPU off, GIE on
} 

void switch_interrupt_handler(){
  char p2val = P2IN;            //Switch is in P2
  
/* update switch interrupt sense to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */

/* Checks if any button is pressed down */
  if (~p2val & SW1) {
    P1OUT |= LED_RED;
    P1OUT &= ~LED_GREEN;
  }
  if(~p2val & SW2) {
    P1OUT |= LED_GREEN;
    P1OUT &= ~LED_RED;
  }
  if(~p2val & SW3) {
    P1OUT |= LED_RED;
    P1OUT |= LED_GREEN;
  }
  if(~p2val & SW4){
    P1OUT &= ~LED_RED;
    P1OUT &= ~LED_GREEN;
  }
}


/* Switch on P2 (S2) */
void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
