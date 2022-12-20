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
    int start=0;//메뉴에서 버튼이 눌렸는지 상태 표시용 변수
    int restart=0;//재시작시 버튼이 눌렸는지 상태 표시용 변수
	char *data;
    buttonInit();//버튼 초기화
    menuviewer();//TFT에 메뉴 띄우기
    ledLibInit();//LED 초기화
    int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);//메세지큐 >> 버튼 입력 확인용
    if(msgID == -1){
            printf("cannot find\n");
            return 1;
    }
    
    while(1){
        int check=1;
        buzzerInit();//부저 초기화
        fndDisp(0,0);//FND 초기화 
        int retmsg =0;
        retmsg=msgrcv(msgID,&B,sizeof(B)-sizeof(long int),0,0);//버튼 입력까지 대기

        
    if ( B.type == EV_KEY)
    {
    printf("EV_KEY(");
    switch(B.keyInput)
    {
        case KEY_HOME:{//버튼 눌리면 실행
            if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 ){
		        printf ("FrameBuffer Init Failed\r\n");
                return 0;        
	        }
            start=1;//start 1로 변경
            break;
        }
        default://다른 버튼 눌리면 그냥 종료
            break;
        }
        if ( B.pressed )
        {
             printf("pressed\n");
        }
        else {
            printf("released\n");
            start=start+start;//버튼이 떼지면 start값 2배 >> 버튼이 떼졌을 때 게임 시작하게 하기 위함
            }
    }
        if(start==2){//Home 버튼이 눌렸다가 떼졌을 때 실행, 아니면 다시 처음 while문으로 돌아가서 버튼 대기
            fb_clear();//TFT 검은색으로 Clear
            fb_playerdraw();//플레이어 캐릭터 Draw
            fb_enemydraw();//적 캐릭터 Draw
            ledallon();//목숨 표시용 LED 3개 ON
            fndInit();//FND 초가화
            while(1)
            {
                if(Gyro()==-1)//자이로센서가 왼쪽으로 기울었으면 플레이어 왼쪽 이동
                fb_pmvleft();
            else if(Gyro()==1)//자이로 센서가 오른쪽으로 기울었으면 플레이어 오른쪽 이동
                fb_pmvright();
            fb_enemymove();//적군 이동 >> 랜덤
            fb_bulleterase();//총알 Erase후
            check=fb_bulletmove();//이동한 위치에 다시 Draw, 게임 Over인지 확인
            if(check==0)//Game Over면 실행
            {  
                fb_bulleterase();//총알 지우기
                restart=0;//버튼 안눌린 상태로 초기화
                fndOff();//FND 종료
                gameoverviewer();//게임 오버 화면 TFT에 표시
                start=0;
                while(1){
                retmsg=msgrcv(msgID,&B,sizeof(B)-sizeof(long int),0,0);//버튼 입력까지 대기
                if ( B.type == EV_KEY)
                {
                     printf("EV_KEY(");
                     switch(B.keyInput)
                     {
                     case KEY_HOME://Home button이 눌리면 restart=1 >> 게임 재시작
                        restart=1;
                        break;        
	                 case KEY_BACK://Back button이 눌리면 restart=-1 >> 게임 종료
                        restart=-1;    
                        break;      
                    default:
                            break;
                        }
                if ( B.pressed ) printf("pressed\n");
                else {
                    printf("released\n");
                    restart=restart+restart;//restart 2배 >> 버튼이 떼졌을 때 동작을 수행하기 위함
                    }
                    }   
                  
                if(restart==2)//Home버튼이 눌렸다가 떼지면 실행
                {
                   fb_varinit();//게임에서 사용되는 변수 초기화
                   fb_close();
                   menuviewer();//메뉴화면 TFT에 표시
                   break; //세 번째 실행된 while문 종료 >> 두 번째 While문으로 돌아감
                }    
                else if(restart==-2)//Back 버튼이 눌렸다가 떼지면 실행
                {//실행했던 것들 종료
                    fb_clear();
                    printf("exit");
                    fndOff();
                    fb_varinit();
                    buttonExit();
                    return 0;//main문 종료 >> 게임 종료
                }
                }
                            }
            if(restart==2)//재시작 버튼을 눌렀으면 실행
            {
                restart=0;
                break;//두번째 While문 종료 >> 첫번째 While문으로 돌아감
            }
            }
           
            
            
    }
    }
     
}

