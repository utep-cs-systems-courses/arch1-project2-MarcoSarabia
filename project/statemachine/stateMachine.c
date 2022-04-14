#include <msp430.h>
#include "libTimer.h"
#include "led.h"



int main(void) {

  P1DIR |= LEDS; //Sets LEDS to output
  P1OUT &= ~LED_GREEN; // Turns off Green LED
  P1OUT |= LED_RED; // Turns on Red LED


  configureClocks();/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();/* enable periodic interrupt */
  
  or_sr(0x18);/* CPU off, GIE on */

}

void greenControl(int on){

  if (on) {
    P1OUT |= LED_GREEN;
  } else {
    P1OUT &= ~LED_GREEN;
  }
}

// blink state machine

static int blinkLimit = 8;   //  state var representing reciprocal of duty cycle

void blinkUpdate(){ // called every 1/250s to blink with duty cycle 1/blinkLimit

  static int blinkCount = 0; // state var representing blink state

  blinkCount ++;

  if (blinkCount >= blinkLimit) {

    blinkCount = 0;

    greenControl(1);

  } else

    greenControl(0);

}

void oncePerSecond(){  // repeatedly start bright and gradually lower duty cycle, one step/sec

  blinkLimit --;  // reduce duty cycle

  if (blinkLimit <= 0)  // but don't let duty cycle go below 1/7.

    blinkLimit = 8;

}

void secondUpdate(){ // Called every 1/250 sec to call oncePerSecond once per second 
  static int secondCount = 0;
  secondCount ++;

  if(secondCount >= 250){
    secondCount = 0;
    oncePerSecond();
  }
}

void timeAdvStateMachines(){ // called every 1/250 sec

  blinkUpdate();

  secondUpdate();

}

void __interrupt_vec(WDT_VECTOR) WDT(){ /* 250 interrupts/sec */

  // handle blinking

  timeAdvStateMachines();

} 
