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

int buttonInit(void)
{
if (probeButtonPath(buttonPath) == 0)
return 0;
fd=open (buttonPath, O_RDONLY);
msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
pthread_create(&buttonTh_id, NULL, buttonThFunc, NULL);
return 1;
}

int buttonExit(void)
{

}

void buttonThFunc(void)
{
    struct input.event A;
    while(1)
    {
        read(fd,&A,sizeof(A));
        button_MSG_T B;
        B.messageNum=1;
        B.keyInput=A.code;
        B.pressed=A.value;
        msgsnd(MESSAGE_ID,&B,sizeof(B)-1);
    }
}