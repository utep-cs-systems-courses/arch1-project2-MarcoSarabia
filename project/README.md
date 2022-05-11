Lab Project 2: Buzzer Toy
==================================
# Description
This project code implements leds, buzzer sounds, and statemachines that are
initialized by pressing the buttons on the MSP430

# Files

The following are source files for this project. They are located in the
folder named "statemachine"
- main.c: Initializes every component in the project by calling all other files, including the periodic interrupts
- led.c: Initializes the LEDs
- buzzer.c:Initializes the buzzer
- switches.c: Initializes and handles the switches and its interrupts. Each
switch determines a state machine
- statemachines.c: Contains the statemachines that utilizes the leds and
handles all the logistics

# How to run

Once you are located in the main folder, go to "project"
    $ cd project/

Then run the command
    $ make

GO inside the folder "statemachine"
    $cd statemachine/

Then run the command
    $ make load
 
