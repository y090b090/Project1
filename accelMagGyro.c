#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define ACCELPATH "/sys/class/misc/FreescaleAccelerometer/"
#define MAGNEPATH "/sys/class/misc/FreescaleMagnetometer/"
#define GYROPATH "/sys/class/misc/FreescaleGyroscope/"
#include "accelMagGyro.h"


int fd = 0;
FILE *fp = NULL;

int acc(void){
//Accelerometer
fd = open (ACCELPATH "enable",O_WRONLY);        //파일 open
dprintf (fd,"1");       // 1 을 쓴다
close(fd);      //파일닫기
fp = fopen (ACCELPATH "data", "rt");    //파일 open
int accel[3];
fscanf(fp,"%d, %d, %d",&accel[0],&accel[1],&accel[2]);  //accel에 fp의 값을 scan
printf ("I read Accel %d, %d, %d\r\n",accel[0],accel[1],accel[2]);      //accel배열값 출력
fclose(fp);
}
// accelerometer와 같은 방식
int Mag(void){
//Magnetometer
fd = open (MAGNEPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);
fp = fopen (MAGNEPATH "data", "rt");
int magne[3];
fscanf(fp,"%d, %d, %d",&magne[0],&magne[1],&magne[2]);
printf ("I read Magneto %d, %d, %d\r\n",magne[0],magne[1],magne[2]);
fclose(fp);
}

static int status=0;    //전역변수 선언

int Gyro(void){
//Gyroscope
fd = open (GYROPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);
fp = fopen (GYROPATH "data", "rt");
int gyro[3];
fscanf(fp,"%d, %d, %d",&gyro[0],&gyro[1],&gyro[2]);
fclose(fp);
printf("%d\n",gyro[0]); //x축값만 출력

if(gyro[0]<-1000)       //만약 x의 변화량이 -1000 즉, 왼쪽으로 기울었을 때
        status=-1;      //status 에 -1입력
else if(gyro[0]>1000)   //오른쪽으로 기울었을때
        status=1;       //status 1 입력
   
    return status;      //status return
}

