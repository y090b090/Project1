#include "led.h"

static unsigned int ledValue = 0;
static int fd = 0;
int num=2;
int ledOnOff (int ledNum, int onOff) // led를 0,1로 껐다 켰다 할 수 있음 
{
	int i=1;	
	i = i<<ledNum;	// 1을 왼쪽으로 ledNum만큼 이동
	ledValue &=(~i);	//기존의 값에서 움직인 i의 위치만 0으로 바꿔준다.
	if (onOff !=0){		//onOff=1이면 on 0이면 off
		ledValue |= i;
	} 
	write (fd, &ledValue, 4);	//ldeValue의 값을 fd에 쓴다.
	return ledValue;
}

int ledallon(void)		// 캐릭터의 목숨 3개만 켜지게 만들었다.
{
	unsigned int data =0;		//data변수초기화
	data=strtol("0x07",NULL,16);	//0x07을 16진수로 변환
	write(fd,&data,4);	//그 값을 fd에 쓴다.
	ledValue=data;		//ledValue에 data값을 저장한다.
	return ledValue;	//ledValue return
}
int ledminus(void)		//캐릭터의 목숨이 하나씩 줄어들게 만드는 함수
{
	int i=1;
	if(num==-1)	//num의 값을 하나씩 줄여가며 led를 꺼지게 하는데 만약 3번 죽었다가 다시 시작할 경우
		num=2;	//num을 2로 만든다.
	
	i = i<<num;	//1을 왼쪽으로 num만큼 이동
	ledValue &=(~i);
	write (fd, &ledValue, 4);
	num--;	//num을 한 번 해줄때마다 --해준다.
	return ledValue;
	
}

int ledLibInit(void)	//led설정
{
	fd=open("/dev/periled",O_WRONLY);	///dev/periled라는 파일을 open한다.
	ledValue = 0;	//ledValue의 초기값은 0
	return ledValue;
}

int ledLibExit(void)	//led 종료
{
	ledValue = 0;	//ledValue의 값을 0으로 만든다.
	ledOnOff (0, 0);	//led를 모두 끈다.
	close(fd);	//fd를 닫는다.
	return ledValue;	
}

int ledStatus(void)	//led의 현재상태
{
	return ledValue;	//ledValue의 값을 retrurn
}
