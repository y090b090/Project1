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
fd = open (ACCELPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);
fp = fopen (ACCELPATH "data", "rt");
int accel[3];
fscanf(fp,"%d, %d, %d",&accel[0],&accel[1],&accel[2]);
printf ("I read Accel %d, %d, %d\r\n",accel[0],accel[1],accel[2]);
fclose(fp);
}

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

static int status=0;

int Gyro(void){
//Gyroscope
fd = open (GYROPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);
fp = fopen (GYROPATH "data", "rt");
int gyro[3];
fscanf(fp,"%d, %d, %d",&gyro[0],&gyro[1],&gyro[2]);
fclose(fp);
printf("%d\n",gyro[0]);

if(gyro[0]<-1000)
        status=-1;     
else if(gyro[0]>1000)
        status=1;
   
    return status;
}

