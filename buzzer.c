#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "buzzer.h"

#define MAX_SCALE_STEP 8
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"

char FrequencyPath[200];
char EnablePath[200];
char gBuzzerBaseSysDir[128];
int fdFre, fdEna;

int buzzerInit(void){ // directory를 찾아주는 함수

	DIR * dir_info = opendir(BUZZER_BASE_SYS_PATH);
	int ifNotFound = 1;

	if (dir_info != NULL){
	while (1){
		struct dirent *dir_entry;
		dir_entry = readdir (dir_info);
		if (dir_entry == NULL) break;
		if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0){
		ifNotFound = 0;
		sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH,dir_entry->d_name);
}
}
}
printf("find %s\n",gBuzzerBaseSysDir);
return ifNotFound;
}


int buzzerPlaySong(int scale, int i) // 주파수를 받아주고, enable에 1의 값을 넣어준다
{
    sprintf(EnablePath,"%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);	//enablePath를 받아온다.
	sprintf(FrequencyPath,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);	//frequencypath를 받아온다.
	
	fdEna=open(EnablePath,O_WRONLY);	//경로 open
	fdFre=open(FrequencyPath,O_WRONLY);
	write(fdEna, &"1", 1);	//enable에 1을 입력
	
	dprintf(fdFre, "%d", scale);	//fdFre에 변수로 입력받은 주파수입력
	usleep(i*5000);	//delay
    return 0;
}

int buzzerStopSong(void) // enable에 0의 값을 넣어서
{
	write(fdEna, &"0", 1);
	return 0;
}

int buzzerExit(void)		//부저종료
{
    write(fdEna, &"0", 1);	//enable에 0
    close(fdFre);	//fdFre종료
    close(fdEna);	//fdEna종료

    return 0;
}

int bgm(void) //챔피언 bgm
{
	buzzerPlaySong(349,50);
	buzzerPlaySong(415,50);
	buzzerPlaySong(349,50);
	buzzerPlaySong(349,50);
	buzzerPlaySong(466,50);
	buzzerPlaySong(311,50);

	buzzerPlaySong(349,50);
	buzzerPlaySong(523,50);
	buzzerPlaySong(349,50);
	buzzerPlaySong(349,50);
	buzzerPlaySong(554,50);
	buzzerPlaySong(523,50);
	buzzerPlaySong(415,50);

	buzzerPlaySong(349,50);
	buzzerPlaySong(523,50);
	buzzerPlaySong(698,50);
	buzzerPlaySong(349,50);
	buzzerPlaySong(311,50);
	buzzerPlaySong(311,50);
	buzzerPlaySong(262,50);
	buzzerPlaySong(392,50);
	buzzerPlaySong(349,50);
}

int bulletsound(void) // 총알 소리
{
	buzzerPlaySong(750,5);
	buzzerPlaySong(300,5);
	buzzerStopSong();
}