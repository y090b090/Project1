libMyPeri.a : led.o buzzer.o fnd.o button.o textlcd.o colorled.o temperature.o
        arm-linux-gnueabi-ar rc libMyPeri.a led.o buzzer.o fnd.o button.o textlcd.o colorled.o temperature.o
button.o : button.c button.h
        arm-linux-gnueabi-gcc -c button.c -o button.o -lpthread
led.o : led.h led.c
        arm-linux-gnueabi-gcc -c led.c -o led.o
buzzer.o : buzzer.h buzzer.c
        arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o
fnd.o : fnd.h fnd.c
        arm-linux-gnueabi-gcc -c fnd.c -o fnd.o
textlcd.o : textlcd.c textlcd.h
        arm-linux-gnueabi-gcc -c textlcd.c -o textlcd.o
colorled.o : colorled.c colorled.h
        arm-linux-gnueabi-gcc -c colorled.c -o colorled.o
temperature.o : temperature.c temperature.h
        arm-linux-gnueabi-gcc -c temperature.c -o temperature.o
