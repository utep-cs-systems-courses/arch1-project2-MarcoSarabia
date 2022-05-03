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

  buzzer_init();                // Initialize Buzzer
  
  enableWDTInterrupts();        // Enable WatchDog Timer

  or_sr(0x18);                  // CPU off, GIE on
} 

