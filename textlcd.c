#include "textlcd.h"

int lcdtextwrite(const char *str1,const char *str2, int lineFlag){
    unsigned int linenum=0;
    stTextLCD stlcd;//stTextLCD구조체를가지고드라이버와인터페이스
    int fd;
    int len1=0,len2=0;
    memset(&stlcd,0,sizeof(stTextLCD));//구조체초기화
    linenum=lineFlag;
    printf("linenum:%d\n",linenum);
    /*if(linenum==0){
        stlcd.cmdData=CMD_DATA_WRITE_BOTH_LINE;
        len1 = strlen(str1);
        if ( len1 > COLUMN_NUM)
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, COLUMN_NUM);
        else
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, len1);
        len2 = strlen(str2);
        if ( len2 > COLUMN_NUM)
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, COLUMN_NUM);
        else
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, len2);
    }*/
        
    if(linenum==1){
        stlcd.cmdData=CMD_DATA_WRITE_LINE_1;
        len1 = strlen(str1);
        if ( len1 > COLUMN_NUM)
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, COLUMN_NUM);
        else
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, len1);
    }
    else if(linenum==2){
        stlcd.cmdData=CMD_DATA_WRITE_LINE_2;
        len2 = strlen(str2);
        if ( len2 > COLUMN_NUM)
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, COLUMN_NUM);
        else
        memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, len2);
    }
    else{
        printf("lineFlag:%dwrong.range(0~2)\n",linenum);
        return 1;
    }

    stlcd.cmd=CMD_WRITE_STRING;
    fd=open(TEXTLCD_DRIVER_NAME,O_RDWR);//opendriver
    if(fd<0){
    perror("driver(//dev//peritextlcd)openerror.\n");
    return 1;
    }
    write(fd,&stlcd,sizeof(stTextLCD));
    close(fd);
    return 0;

}

