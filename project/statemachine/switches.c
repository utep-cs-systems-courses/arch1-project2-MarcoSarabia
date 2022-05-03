#include <msp430.h>
#include "libTimer.h"
#include "switches.h"
#include "stateMachines.h"

void switch_init(){
  P2REN |= SWITCHES;		// enables resistors for switches 
  P2IE |= SWITCHES;		// enable interrupts from switches 
  P2OUT |= SWITCHES;		// pull-ups for switches 
  P2DIR &= ~SWITCHES;		// set switches' bits for input 
}

void switch_interrupt_handler(){
  char p2val = P2IN;            //Switch is in P2
  
/* update switch interrupt sense to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */

/* Checks if any button is pressed down */
  if (~p2val & SW1) {           //When button 1 is pressed
    currentStateMachine = 1;
    
  }
  if(~p2val & SW2) {            //When button 2 is pressed
    currentStateMachine = 2;
    
  }
  if(~p2val & SW3) {            //When button 3 is pressed
    currentStateMachine = 3;
    
  }
  if(~p2val & SW4){             //When button 4 is pressed
    currentStateMachine = 4;
    
  }
}

/* Switch on P2 (S2) */
void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

