#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "stateMachines.h"

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

static int blinkLimit = 0;   //  state var representing reciprocal of duty cycle

void blinkUpdate1(){ // called every 1/250s to blink with duty cycle 1/blinkLimit

  static int blinkCount = 0; // state var representing blink state

  blinkCount ++;

  if (blinkCount >= blinkLimit){ 
    blinkCount = 0;
    redControl(1);
  } else{
    redControl(0);
  }
}

void oncePerSecond1(){  // repeatedly start bright and gradually lower duty cycle, one step/sec

  blinkLimit ++;  // reduce duty cycle
  if (blinkLimit >= 8){  // but don't let duty cycle go below 1/7.
    blinkLimit = 0;
  }
}

void secondUpdate(){ // Called every 1/250 sec to call oncePerSecond once per second 
  static int interruptCount = 0;
  interruptCount ++;

  if(interruptCount >= 250){
    interruptCount = 0;
    oncePerSecond1();
  }
}

void brightToDim(){
  blinkUpdate1();
  secondUpdate();
}

void toggleRed(){ // called every 1/250 sec
  static int interruptCount = 0;
  interruptCount ++;

  if(interruptCount >= 250){
    interruptCount = 0;
    P1OUT ^= LED_RED;
  }
}

void toggleGreen(){ // called every 1/250 sec
  static int interruptCount = 0;
  interruptCount ++;

  if(interruptCount >= 250){
    interruptCount = 0;
    P1OUT ^= LED_GREEN;
  }
}


void __interrupt_vec(WDT_VECTOR) WDT(){ /* 250 interrupts/sec */

  switch(currentStateMachine){
  case 1:
    greenControl(0);
    toggleRed();
    break;
  case 2:
    redControl(0);
    toggleGreen();
    break;
  case 3:
    ledsControl(0);
    brightToDim();
    break;
  case 4:
    ledsControl(0);
    break;
  }
} 
