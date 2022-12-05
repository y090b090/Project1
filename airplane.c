#include <stdio.h>
#include <stdlib.h>
#include <sys/kd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "libfbdev.h"

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
    int x_init=1024-30;
    int y_init=285;
    fb_playerdraw();
    fb_enemydraw();
    for(int i=0;i<60;i++)
        fb_pmvleft();
    for(int i=0;i<150;i++)
        fb_pmvright();
    fb_close();
}
