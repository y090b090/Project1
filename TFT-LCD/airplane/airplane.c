#include <stdio.h>
#include <stdlib.h>
#include <sys/kd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "../libfbdev/libfbdev.h"

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
    
    
    int x_init=0;
    int y_init=285;
    for(int i=0;i<5;i++)
    {
    fb_clear();
    fb_draw(x_init+10*i,y_init+10*i);
    usleep(1000000);
    }
    fb_close();
}
