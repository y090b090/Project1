#ifndef _LED_H_
#define _LED_H_

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>

int ledLibInit(void);
int ledOnOff (int ledNum, int onOff);
int ledStatus (void);
int ledLibExit(void);
void doHelp(void);
int ledallon(void);
int ledminus(void);

#define LED_DRIVER_NAME  "/dev/periled"

#endif 
