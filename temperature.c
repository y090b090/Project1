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