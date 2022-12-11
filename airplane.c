#include <stdio.h>
#include <stdlib.h>
#include <sys/kd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "libfbdev.h"
#include <pthread.h>
#include <unistd.h>
#include "accelMagGyro.h"
#include "fnd.h"
#include "button.h"

int main(void){
    fndInit();
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int cols = 0, rows = 0;
	char *data;
    buttonInit();
    int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
    if(msgID == -1){
            printf("cannot find\n");
            return 1;
    }
    
    
    int retmsg =0;
    retmsg=msgrcv(msgID,&B,sizeof(B)-sizeof(long int),0,0);
        
    if ( B.type == EV_KEY)
    {
    printf("EV_KEY(");
    switch(B.keyInput)
    {
        case KEY_VOLUMEUP: printf("Volume up key):"); break;
        case KEY_HOME:{
            if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 ){
		        printf ("FrameBuffer Init Failed\r\n");
		        return 0;
	        }
            fb_clear();
            fb_playerdraw();
            fb_enemydraw();
            ledLibInit();
            ledallon();
            while(1)
            {
                if(Gyro()==-1)
                fb_pmvleft();
            else if(Gyro()==1)
                fb_pmvright();
            fb_enemymove();
            fb_bulleterase();
            int check=fb_bulletmove();
            if(check==0)
                break;
            }
            fb_close();
        }

    }
    }
    buttonExit();
}
