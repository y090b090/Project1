#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/ioctl.h> 
#include <sys/msg.h>
#include <pthread.h>
#include "button.h"
#define INPUT_DEVICE_LIST "/dev/input/event"
#define PROBE_FILE "/proc/bus/input/devices" 
#define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
#define HAVE_TO_FIND_2 "H: Handlers=kbd event"

#define MESSAGE_ID 1122
struct input_event A;
struct BUTTON_MST_T B;


struct  BUTTON_MST_T
{
	long int messageNum;
	int keyInput;
	int pressed;
	unsigned short type;
};


int buttonExit(void);
int buttonInit(void);
void* buttonThFunc(void *arg);
int probeButtonPath(char *newPath);
#endif
