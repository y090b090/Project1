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
#include "buzzer.h"
#include "textlcd.h"
#include "libjpeg/jpeglib.h"

int main(void){
    
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int cols = 0, rows = 0;
    int start=0;
    int restart=0;
	char *data;
    buttonInit();
    menuviewer();
    ledLibInit();
    int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
    if(msgID == -1){
            printf("cannot find\n");
            return 1;
    }
    
    while(1){
        int check=1;
        buzzerInit();
        fndDisp(0,0);
    int retmsg =0;
    retmsg=msgrcv(msgID,&B,sizeof(B)-sizeof(long int),0,0);

        
    if ( B.type == EV_KEY)
    {
    printf("EV_KEY(");
    switch(B.keyInput)
    {
        case KEY_HOME:{
            if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 ){
		        printf ("FrameBuffer Init Failed\r\n");
                return 0;        
	        }
            start=1;
            break;
        }
        default:
            break;
        }
        if ( B.pressed )
        {
             printf("pressed\n");
        }
        else {
            printf("released\n");
            start=start+start;
            }
    }
        if(start==2){
            fb_clear();
            fb_playerdraw();
            fb_enemydraw();
            ledallon();
            fndInit();
            while(1)
            {
                if(Gyro()==-1)
                fb_pmvleft();
            else if(Gyro()==1)
                fb_pmvright();
            fb_enemymove();
            fb_bulleterase();
            check=fb_bulletmove();
            if(check==0)
            {  
                fb_bulleterase();
                restart=0;
                fndOff();
                gameoverviewer();
                start=0;
                while(1){
                retmsg=msgrcv(msgID,&B,sizeof(B)-sizeof(long int),0,0);
                if ( B.type == EV_KEY)
                {
                     printf("EV_KEY(");
                     switch(B.keyInput)
                     {
                     case KEY_HOME:
                        restart=1;
                        break;        
	                 case KEY_BACK:
                        restart=-1;    
                        break;      
                    default:
                            break;
                        }
                if ( B.pressed ) printf("pressed\n");
                else {
                    printf("released\n");
                    restart=restart+restart;
                    }
                    }   
                  
                if(restart==2)
                {
                   fb_varinit();
                   fb_close();
                   menuviewer();
                   break; 
                }    
                else if(restart==-2)
                {
                    fb_clear();
                    printf("exit");
                    fndOff();
                    fb_varinit();
                    buttonExit();
                    return 0;
                }
                }
                            }
            if(restart==2){
                restart=0;
                break;
            }
            }
            
            
    }
    }
     
}

