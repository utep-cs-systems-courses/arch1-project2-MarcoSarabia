#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "switches.h"
#include "stateMachines.h"

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
    currentStateMachine = 1;
    P1OUT &= ~LEDS;
    enableWDTInterrupts();
  }
  if(~p2val & SW2) {
    currentStateMachine = 2;
    enableWDTInterrupts();
  }
  if(~p2val & SW3) {
    currentStateMachine = 3;
    enableWDTInterrupts();
  }
  if(~p2val & SW4){
    currentStateMachine = 4;
    enableWDTInterrupts();
  }
}


/* Switch on P2 (S2) */
void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
