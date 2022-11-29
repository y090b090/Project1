libMyPeri.a : led.o buzzer.o fnd.o button.o lcdtext.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o buzzer.o fnd.o button.o lcdtext.o
button.o : button.c button.h
	arm-linux-gnueabi-gcc -c button.c -o button.o -lpthread
led.o : led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
buzzer.o : buzzer.h buzzer.c
	arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o
fnd.o : fnd.h fnd.c
	arm-linux-gnueabi-gcc -c fnd.c -o fnd.o
lcdtext.o : lcdtext.c lcdtext.h
	arm-linux-gnueabi-gcc -c lcdtext.c -o lcdtext.o