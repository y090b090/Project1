#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "fnd.h"
#include <pthread.h>
#define FND_DRIVER_NAME "/dev/perifnd"

pthread_t tid;
int score=0;

int fndInit(void)	//fnd초기설정
{
	tid=0;
	pthread_create(&tid,NULL,&fndtime,NULL);		// 스레드 생성
}

void *fndtime(void *arg)	//스레드에서 수행될 함수
{	score=0;
	while(1)
	{
		fndDisp(score,0);	//fnd에 모두 0으로
		score++;	//score의 값을 1씩 증가
		sleep(1);	//1초주기
	}
}

int fndOff(void)	//fnd끄기
{	
	fndDisp(0,0);	//fnd에 0으로
	pthread_cancel(tid);	//스레드 종료
	return score;	// score값 return
}

int fndMode(int num, char mode)	//fnd 모드
{
	if(mode==MODE_STATIC_DIS)	//static모드
	{
		fndDisp(num,0);	//num값을 보여준다.
	}
	else if(mode==MODE_TIME_DIS)	//time모드 시간을 보여준다.
	{
	struct tm *ptmcur;
	time_t tTime;
	if(-1==time(&tTime))
		return-1;
	ptmcur=localtime(&tTime);
	
	num=ptmcur->tm_hour*10000;
	num+=ptmcur->tm_min*100;
	num+=ptmcur->tm_sec;
	
	fndDisp(num,0b1010);
	}

	else if(mode==MODE_COUNT_DIS)	//count 모드 1초씩 증가
	{
		int counter=0;
	
		while(1)
		{
		if(!fndDisp(counter,0))
		break;
	counter++;
	sleep(1);
	if(counter>num)
		break;
		}
	}
}

int fndDisp(int num,int dotflag)//0-999999숫자,비트로인코딩된doton/off
{
	int fd;
	int temp,i;
	stFndWriteForm stWriteData;
	for(i=0;i<MAX_FND_NUM;i++)
	{
		stWriteData.DataDot[i]=(dotflag&(0x1<<i))?1:0;	//도트플래그 입력
		stWriteData.DataValid[i]=1;
	}
	//if6fnd 데이터의 값을 한자리씩 넣는과정
	temp=num%1000000;stWriteData.DataNumeric[0]=temp/100000;
	temp=num%100000;stWriteData.DataNumeric[1]=temp/10000;
	temp=num%10000;stWriteData.DataNumeric[2]=temp/1000;
	temp=num%1000;stWriteData.DataNumeric[3]=temp/100;
	temp=num%100;stWriteData.DataNumeric[4]=temp/10;
	stWriteData.DataNumeric[5]=num%10;

	fd=open(FND_DRIVER_NAME,O_RDWR);
	if(fd<0)
	{
		perror("driveropenerror.\n");
		return 0;
	}
	write(fd,&stWriteData,sizeof(stFndWriteForm)); //구조체의 값을 쓴다
	close(fd);
	return 1;
}
