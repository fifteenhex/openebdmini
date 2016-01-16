CFLAGS=--std-sdcc99
CC=sdcc


openebdmini.ihx: openebdmini.c
	sdcc $(CFLAGS) -lstm8 -mstm8 --out-fmt-ihx $<

.PHONY:clean flash

clean:
	rm *.ihx

flash: openebdmini.ihx
	./stm8flash/stm8flash -c stlinkv2 -p stm8s105?4 -s flash -w $<
