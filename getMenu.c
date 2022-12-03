#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "textlcd.h"

int printTime(void) //시간 출력 함수
{
   struct tm pre;
   char year[100];
   char day[4];
   char mon[4];
   char hour[4];
   char min[4];
   char sec[4];
    while(1){
    time_t timer = time(NULL);
    struct tm t=*localtime(&timer);
    sprintf(day,"%02d",t.tm_mday);
    sprintf(year,"%d",t.tm_year+1900);
    sprintf(mon,"%02d",t.tm_mon+1);
    sprintf(hour,"%02d",t.tm_hour);
    sprintf(min,"%02d",t.tm_min);
    sprintf(sec,"%02d",t.tm_sec);
    strcat(year,"/");
    strncat(year,mon,2);
    strcat(year,"/");
    strncat(year,day,2);
    strcat(year," ");
    strncat(year,hour,2);
    strcat(year,":");
    strncat(year,min,2);
    strcat(year,":");
    strncat(year,sec,2);
    if(t.tm_sec==pre.tm_sec)
        lcdtextwrite(year,"hi", 1);
    pre=t;
    usleep(1000);
    }
    return 0;

}


int main(void)
{

    printTime();

}