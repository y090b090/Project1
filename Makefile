all: libMyPeri.all
libMyPeri.a:button.o led.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o
button.o:botton.h button.c
	arm-linux-gnueabi-gcc -c button.c -o button.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o




