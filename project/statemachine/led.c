#include <msp430.h>
#include "libTimer.h"
#include "led.h"

void led_init(){
  P1DIR |= LEDS;                // Sets P1DIR to output
  P1OUT &= ~LEDS;		// LEDs initially off 
}

void redControl(int on){ //Determines if RED_LED should be on/off and changes its status 
  if (on) {
    P1OUT |= LED_RED;
  } else {
    P1OUT &= ~LED_RED;
  }
}

void greenControl(int on){ //Determines if GREEN_LED should be on/off and changes its status
  if (on) {
    P1OUT |= LED_GREEN;
  } else{
    P1OUT &= ~LED_GREEN;
  }
}

void ledsControl(int on){ //Determines if both LEDs should be on/off and changes their status
  if (on) {
    P1OUT |= LEDS;
  } else {
    P1OUT &= ~LEDS;
  }
}
