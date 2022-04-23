# makfile configuration

COMMON_OBJECTS  = 

CPU             = msp430g2553

CFLAGS          = -mmcu=${CPU} -I../h

LDFLAGS		= -L/opt/ti/msp430_gcc/include



#switch the compiler (for the internal make rules)

CC              = msp430-elf-gcc

AS              = msp430-elf-as



all: led.elf



#additional rules for files

led.elf: ${COMMON_OBJECTS} main.o stateMachines.o led.o switches.o buzzer.o ../lib/libTimer.a

	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ 



load: led.elf

	msp430loader.sh led.elf



clean:

	rm -f *.o *.elf



