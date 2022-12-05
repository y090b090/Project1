#include "led.h"

static unsigned int ledValue = 0;
static int fd = 0;
int num=8;
int ledOnOff (int ledNum, int onOff)
{
	int i=1;
	i = i<<ledNum;
	ledValue &=(~i);
	if (onOff !=0){
		ledValue |= i;
	} 
	write (fd, &ledValue, 4);
	return ledValue;
}

int ledallon(void)
{
	unsigned int data =0;
	data=strtol("0xff",NULL,16);
	write(fd,&data,4);
	ledValue=data;
	return ledValue;
}
int ledminus(void)
{
	int i=1;
	
	i = i<<num;
	ledValue &=(~i);
	write (fd, &ledValue, 4);
	num--;
	return ledValue;
	
}

int ledLibInit(void)
{
	fd=open("/dev/periled",O_WRONLY);
	ledValue = 0;
	return ledValue;
}

int ledLibExit(void)
{
	ledValue = 0;
	ledOnOff (0, 0);
	close(fd);
	return ledValue;
}

int ledStatus(void)
{
	return ledValue;
}
