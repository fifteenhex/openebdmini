CFLAGS=--std-sdcc99 -lstm8 -mstm8
CC=sdcc

all: openebdmini.ihx

load.rel: load.c
	sdcc $(CFLAGS) -c $<

buttons.rel: buttons.c
	sdcc $(CFLAGS) -c $<

util.rel: util.c
	sdcc $(CFLAGS) -c $<
	
state.rel: state.c
	sdcc $(CFLAGS) -c $<

display.rel: display.c
	sdcc $(CFLAGS) -c $<
	
uart.rel: uart.c
	sdcc $(CFLAGS) -c uart.c

openebdmini.rel: openebdmini.c
	sdcc $(CFLAGS) -c openebdmini.c 

openebdmini.ihx: openebdmini.rel display.rel uart.rel state.rel util.rel buttons.rel load.rel
	sdcc $(CFLAGS) --out-fmt-ihx $^

.PHONY:clean flash

clean:
	rm *.ihx *.rel *.lst

flash: openebdmini.ihx
	sudo ./stm8flash/stm8flash -c stlinkv2 -p stm8s105?4 -s flash -w $<
