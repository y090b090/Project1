#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "temperature.h"

char gbuf[10];

int spi_init(const char filename[40]){
    
    int file;

    __u8 mode, lsb, bits;
    __u32 speed=20000;
    
    if ((file = open(filename,O_RDWR)) < 0) {
        printf("Failed to open the bus.");
        printf ("ErrorType:%d\r\n",errno); 
        exit(1); 
    }
    
    if (ioctl(file, SPI_IOC_RD_MODE, &mode) < 0){
        perror("SPI rd_mode"); 
        return 0;
    }
    
    if (ioctl(file, SPI_IOC_RD_LSB_FIRST, &lsb) < 0){ 
        perror("SPI rd_lsb_fist"); 
        return 0; 
    }

    if (ioctl(file, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0){ 
        perror("SPI bits_per_word"); 
        return 0; 
    }

    printf("%s: spi mode %d, %d bits %sper word, %d Hz max\n",filename, mode, bits, lsb ? "(lsb first) " : "", speed);

    return file;
}

char * spi_read_lm74(int file){

    int len;

    memset(gbuf, 0, sizeof( gbuf)); // 배열 초기화
    len = read(file, gbuf, 2); //2바이트를 읽는다.
    
    if (len !=2){
        perror("read error");

        return NULL;
    }

    return gbuf;
}

int gettem(void){ 
char *buffer; int file;
file=spi_init("/dev/spidev1.0"); //dev
buffer=(char *)spi_read_lm74(file); 
close(file);
}