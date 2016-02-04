CFLAGS=--std-sdcc99 -lstm8 -mstm8
CC=sdcc
GLOBALDEPS=stm8.h state.h

all: openebdmini.ihx

protocol.rel: protocol.c protocol.h $(GLOBALDEPS)
	sdcc $(CFLAGS) -c $<

watchdog.rel: watchdog.c watchdog.h $(GLOBALDEPS)
	sdcc $(CFLAGS) -c $<

timer.rel: timer.c $(GLOBALDEPS)
	sdcc $(CFLAGS) -c $<

adc.rel: adc.c adc.h $(GLOBALDEPS)
	sdcc $(CFLAGS) -c $<

load.rel: load.c load.h $(GLOBALDEPS)
	sdcc $(CFLAGS) -c $<

buttons.rel: buttons.c buttons.h $(GLOBALDEPS)
	sdcc $(CFLAGS) -c $<

util.rel: util.c util.h $(GLOBALDEPS)
	sdcc $(CFLAGS) -c $<
	
state.rel: state.c $(GLOBALDEPS)
	sdcc $(CFLAGS) -c $<

display.rel: display.c $(GLOBALDEPS)
	sdcc $(CFLAGS) -c $<
	
uart.rel: uart.c uart.h $(GLOBALDEPS) 
	sdcc $(CFLAGS) -c uart.c

openebdmini.rel: openebdmini.c uart.h $(GLOBALDEPS) 
	sdcc $(CFLAGS) -c openebdmini.c 

openebdmini.ihx: openebdmini.rel display.rel uart.rel state.rel util.rel buttons.rel load.rel adc.rel timer.rel watchdog.rel protocol.rel
	sdcc $(CFLAGS) --out-fmt-ihx $^

.PHONY:clean flash

clean:
	rm *.ihx *.rel *.lst

flash: openebdmini.ihx
	sudo ./stm8flash/stm8flash -c stlinkv2 -p stm8s105?4 -s flash -w $<
