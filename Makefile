libMyPeri.a: button.o led.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o
button.o: button.c button.h
	arm-linux-gnueabi-gcc -c button.c -o button.o -lpthread -L.
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o




