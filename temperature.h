#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_


int spi_init(const char filename[40]);
char * spi_read_lm74(int file);


#endif //_TEMPERATURE_H_
