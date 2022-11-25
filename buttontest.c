#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include "button.h"
// first read input device
#define INPUT_DEVICE_LIST "/dev/input/event"
//실제 디바이스 드라이버 노드파일: 뒤에 숫자가 붙음., ex)/dev/input/event5
#define PROBE_FILE "/proc/bus/input/devices"
//PPT에 제시된 "이 파일을 까보면 event? 의 숫자를 알수 있다"는 바로 그 파일
int main(void)
{
    printf("Buttontest Start");
int buttonInit(void);

struct input_event stEvent;
int msgQueue=msgget(MESSAGE_ID,IPC_CREAT|0666);
if(msgQueue==-1)
{
    printf("Cannot get msgQueueID,Return! \r\n");
    return -1;
}

while(1){
msgrcv(msgQueue,&stEvent,sizeof(stEvent),0,IPC_NOWAIT);
if(stEvent.type==EV_KEY)
{
printf("EV_KEY(");
switch(stEvent.code)
{
case KEY_VOLUMEUP: printf("Volume up key):"); break;
case KEY_HOME: printf("Home key):"); break;
case KEY_SEARCH: printf("Search key):"); break;
case KEY_BACK: printf("Back key):"); break;
case KEY_MENU: printf("Menu key):"); break;
case KEY_VOLUMEDOWN: printf("Volume down key):"); break;
}
if ( stEvent.value ) printf("pressed\n");
else printf("released\n");
}
}
buttonExit();

}
