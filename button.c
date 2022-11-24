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

int probeButtonPath(char *newPath)
{
    int returnValue = 0; //button에 해당하는 event#을 찾았나?
    int number = 0; //찾았다면 여기에 집어넣자
    FILE *fp = fopen(PROBE_FILE,"rt"); //파일을 열고
    #define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
    #define HAVE_TO_FIND_2 "H: Handlers=kbd event"
    while(!feof(fp)) //끝까지 읽어들인다.
    {
        char tmpStr[200]; //200자를 읽을 수 있게 버퍼
        fgets(tmpStr,200,fp); //최대 200자를 읽어봄
        //printf ("%s",tmpStr);
    if (strcmp(tmpStr,HAVE_TO_FIND_1) == 0)
    {
        printf("YES! I found!: %s\r\n", tmpStr);
        returnValue = 1; //찾음
    }
    if ((returnValue == 1) && (strncasecmp(tmpStr, HAVE_TO_FIND_2, strlen(HAVE_TO_FIND_2)) == 0))
    {
        printf ("-->%s",tmpStr);
        printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
        number = tmpStr[strlen(tmpStr)-3] - '0'; 
        //Ascii character '0'-'9' (0x30-0x39) 
        //to interger(0)
        break;
    }
    }
    fclose(fp);
    if (returnValue == 1)
    sprintf (newPath,"%s%d",INPUT_DEVICE_LIST,number);
    return returnValue;
}

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

buttonThFunc(void)
{
    struct input.event A;
    while(1)
    {
        read
    }
}