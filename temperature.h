#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

char * spi_read_lm74(int file);

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



#endif //_TEMPERATURE_H_
