#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "buzzer.h"
#include "stateMachines.h"

// Toggles the red LED
void toggleRed(){ 
  static int interruptCount = 0;
  interruptCount ++;

  if(interruptCount >= 250){
    interruptCount = 0;
    P1OUT ^= LED_RED;
  }
}
// -----------------------------------

// Toggles the green LED
void toggleGreen(){ 
  static int interruptCount = 0;
  interruptCount ++;

  if(interruptCount >= 250){
    interruptCount = 0;
    P1OUT ^= LED_GREEN;
  }
}
// -----------------------------------


static int blinkLimit1 = 0;
static int blinkLimit2 = 8;
void combinedBlinkUpdate(){ // called every 1/250s to blink with duty cycle 1/blinkLimit

  static int blinkCount = 0; // static var representing blink state
  static int blinkCount2 = 0;

  blinkCount ++;
  blinkCount2 ++;

  if (blinkCount >= blinkLimit1){
    blinkCount = 0;
    redControl(1);
    
  } else{
    redControl(0);
  }
  
  if (blinkCount2 >= blinkLimit2){
    blinkCount2 = 0;
    greenControl(1);
    
  } else{
    greenControl(0);
    
  }
}

void combinedOncePerSecond(){  // repeatedly start bright and gradually lower duty cycle, one step/sec

  blinkLimit1 ++;         // reduce duty cycle
  blinkLimit2 --;

  if (blinkLimit1 >= 8){  // but don't let duty cycle go below 1/7.
    blinkLimit1 = 0;

  }

  if(blinkLimit2 <= 0){
    blinkLimit2 = 8;
    
  }
}

void combinedSecondUpdate(){ // Called every 1/250 sec to call oncePerSecond once per second

  static int interruptCount = 0;
  
  interruptCount ++;

  if(interruptCount >= 250){
    interruptCount = 0;
    combinedOncePerSecond();

  }
}

// Makes red LED go from bright to dim while the green LED goes from dim to bright
void combinedStateMachine(){
  combinedBlinkUpdate();
  combinedSecondUpdate();
  
}
// -------------------------------

static int buzzerPeriod = 5000;
void decreasingBuzzer(){
  static int interruptCount = 0;
  interruptCount ++;

  buzzer_set_period(buzzerPeriod);

  if(interruptCount >= 250){
    interruptCount = 0;

    if(buzzerPeriod >= 10000){
      buzzerPeriod = 5000;
    } else{
      buzzerPeriod += 1000;
    }  
  }
}

void increasingBuzzer(){
  static int interruptCount = 0;
  interruptCount ++;

  buzzer_set_period(buzzerPeriod);

  if(interruptCount >= 250){
    interruptCount = 0;

    if(buzzerPeriod <= 1000){
      buzzerPeriod = 5000;
    } else{
      buzzerPeriod -= 1000;
    }  
  }
}

void __interrupt_vec(WDT_VECTOR) WDT(){ /* 250 interrupts/sec */

  switch(currentStateMachine){

  case 1:
    greenControl(0);
    toggleRed();
    
    decreasingBuzzer();
    break;

  case 2:
    redControl(0);
    toggleGreen();

    increasingBuzzer();
    break;

  case 3:
    ledsControl(0);
    combinedStateMachine();
    
    break;

  case 4:
    ledsControl(0);
    buzzer_set_period(0);
    
    break;

  }
} 
