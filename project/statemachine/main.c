#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"
#include "stateMachines.h"


void main(void){

  configureClocks();            // Setup master oscilator, CPU & perihperal clocks
  
  led_init();                   // Initialize LEDS
  
  switch_init();                // Initialize Switches

  buzzer_init();
  
  enableWDTInterrupts();        // Enable WatchDog Timer

  or_sr(0x18);            // CPU off, GIE on
} 

/* Switch on P2 (S2) */
void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
