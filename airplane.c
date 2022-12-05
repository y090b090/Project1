#include <stdio.h>
#include <stdlib.h>
#include <sys/kd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "libfbdev.h"
#include <pthread.h>
#include <unistd.h>
#include "accelMagGyro.h"


int main(void){
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int cols = 0, rows = 0;
	char *data;
	//FrameBuffer init
    if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 ){
		printf ("FrameBuffer Init Failed\r\n");
		return 0;
	}
    fb_clear();
    fb_playerdraw();
    fb_enemydraw();
    while(1)
    {
        if(Gyro()==-1)
            fb_pmvleft();
        else if(Gyro()==1)
            fb_pmvright();
        fb_enemymove();
    }
   
    fb_close();
}
