#include "textlcd.h"

int main(int argc,char **argv)
{
unsigned int linenum=0;
stTextLCD stlcd;//stTextLCD구조체를가지고드라이버와인터페이스
int fd;
int len;
memset(&stlcd,0,sizeof(stTextLCD));//구조체초기화
if(argc<3){//line정보와쓸정보를확인
perror("Argsnumberislessthan2\n");
doHelp();
return 1;
}
linenum = strtol(argv[1],NULL,10);
printf("linenum:%d\n",linenum);
if(linenum==1)
stlcd.cmdData=CMD_DATA_WRITE_LINE_1;
elseif(linenum==2)
stlcd.cmdData=CMD_DATA_WRITE_LINE_2;
else{
printf("linenum:%dwrong.range(1~2)\n",linenum);
return 1;}

len=strlen(argv[2]);
if(len>COLUMN_NUM)
memcpy(stlcd.TextData[stlcd.cmdData-1],argv[2],COLUMN_NUM);
else
memcpy(stlcd.TextData[stlcd.cmdData-1],argv[2],len);
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

