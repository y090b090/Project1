#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>     // for open/close
#include <fcntl.h>      // for O_RDWR
#include <sys/ioctl.h>  // for ioctl
#include <sys/mman.h>
#include <linux/fb.h>   // for fb_var_screeninfo, FBIOGET_VSCREENINFO
#include "libfbdev.h"
#include <pthread.h>
#include "textlcd.h"
#include "buzzer.h"
#include <time.h>
#define FBDEV_FILE "/dev/fb0"

struct { //총알의 정보를 담을 구조체
	int exist; //총알이 존재하는가?
	int bx; //총알의 x축 위치
	int by; //총알의 y축 위치
}bullet[100];

static int fbfd;
static int fbHeight=0;	//현재 하드웨어의 사이즈
static int fbWidth=0;	//현재 하드웨어의 사이즈
static int x=1024; //Player 캐릭터의 x축 위치 >> 초기: x축 맨 위
static int y=300; //Player 캐릭터의 y축 위치 >> 초기: y축 가운데
static int ex=0; //Enemy 캐릭터의 x축 위치 >> 초기: x축 맨 아래
static int ey=300; //Enemy 캐릭터의 y축 위치 >> 초기: y축 가운데
static unsigned long   *pfbmap;	//프레임 버퍼
static struct fb_var_screeninfo fbInfo;	//To use to do double buffering.
static struct fb_fix_screeninfo fbFixInfo;	//To use to do double buffering.
static int bulletnum=0; //Which bullet?
static int hp=3; //캐릭터의 목숨

#define PFBSIZE 			(fbHeight*fbWidth*sizeof(unsigned long)*2)	//Double Buffering
#define DOUBLE_BUFF_START	(fbHeight*fbWidth)	///Double Swaping
static int currentEmptyBufferPos = 0;
//1 Pixel 4Byte Framebuffer.

int fb_init(int * screen_width, int * screen_height, int * bits_per_pixel, int * line_length) //TFT 초기화
{
    struct  fb_fix_screeninfo fbfix;

	if( (fbfd = open(FBDEV_FILE, O_RDWR)) < 0)
    {
        printf("%s: open error\n", FBDEV_FILE);
        return -1;
    }

    if( ioctl(fbfd, FBIOGET_VSCREENINFO, &fbInfo) )
    {
        printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE);
		close(fbfd);
        return -1;
    }
   	if( ioctl(fbfd, FBIOGET_FSCREENINFO, &fbFixInfo) )
    {
        printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE);
        close(fbfd);
        return -1;
    }
	//printf ("FBInfo.YOffset:%d\r\n",fbInfo.yoffset);
	fbInfo.yoffset = 0;
	ioctl(fbfd, FBIOPUT_VSCREENINFO, &fbInfo);	//슉!
    if (fbInfo.bits_per_pixel != 32)
    {
        printf("bpp is not 32\n");
		close(fbfd);
        return -1;
    }	

    fbWidth = *screen_width    =   fbInfo.xres;
    fbHeight = *screen_height   =   fbInfo.yres;
    *bits_per_pixel  =   fbInfo.bits_per_pixel;
    *line_length     =   fbFixInfo.line_length;

	pfbmap  =   (unsigned long *)
        mmap(0, PFBSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);
	
	if ((unsigned)pfbmap == (unsigned)-1)
    {
        printf("fbdev mmap failed\n");
        close(fbfd);
		return -1;
    }
	#ifdef ENABLED_DOUBLE_BUFFERING
		currentEmptyBufferPos = DOUBLE_BUFF_START;	//더블버퍼링 임시 주소로 할당
	#else
		currentEmptyBufferPos = 0;
	#endif
	//printf ("CurrentEmptyBuffPos:%d\r\n",currentEmptyBufferPos);
	return 1;
}

void fb_clear(void)//TFT Black으로 Clear
{
	int coor_y = 0;
	int coor_x = 0;

	// fb clear - black
    for(coor_y = 0; coor_y < fbHeight; coor_y++) 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y);
        for(coor_x = 0; coor_x < fbWidth; coor_x++)
        {
            *ptr++  =   0x000000;
        }
    }
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
}

void fb_playerdraw(void)//Player Character TFT에 그리기
{
	int coor_y = 0;
	int coor_x = 0;
	// fb clear - black
    for(coor_y = y-15; coor_y < y+15; coor_y++) //플레이어 y좌표 기준으로 왼쪽으로 15부터 오른쪽으로 14까지 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+x-30;//플레이어 x좌표부터 -30부터 
        for(coor_x = 0; coor_x < 31; coor_x++)
        {
            *ptr++  =   0x00B07B;
        }
    }
	//이하 플레이어 생김새 그리기용
	for(coor_y=y-8;coor_y<y-3;coor_y++)
	{
		unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+x-23;
		for(coor_x = 0; coor_x < 11; coor_x++)
				*ptr++=0x000000;
	}
	for(coor_y=y+8;coor_y>y+3;coor_y--)
	{
		unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+x-23;
		for(coor_x = 0; coor_x < 11; coor_x++)
				*ptr++=0x000000;
	}
	for(coor_y=y-7;coor_y<y-5;coor_y++)
	{
		unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+x-10;
		for(coor_x = 0; coor_x < 5; coor_x++)
				*ptr++=0x000000;
	}
	for(coor_y=y+7;coor_y>y+5;coor_y--)
	{
		unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+x-10;
		for(coor_x = 0; coor_x < 5; coor_x++)
				*ptr++=0x000000;
	}
	for(coor_y=y-5;coor_y<y+6;coor_y++)
	{
		unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+x-7;
		for(coor_x = 0; coor_x < 2; coor_x++)
				*ptr++=0x000000;
	}
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif

}

void fb_playererase(void)//플레이어 캐릭터 Black으로 Erase
{
	int coor_y = 0;
	int coor_x = 0;
	
    for(coor_y = y-15; coor_y < y+15; coor_y++) 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+x-30;
        for(coor_x = 0; coor_x < 30; coor_x++)
        {
            *ptr++  =   0x000000;
        }
    }
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
}

void fb_pmvleft(void)//플레이어 왼쪽으로 이동
{	
	
	if(y<580)//화면 바깥으로 빠져나가는 것 방지
	{
	int coor_y = 0;
	int coor_x = 0;
	fb_playererase();//플레이어 Erase
	y=y+20;//플레이어 y좌표 20 추가 >> 왼쪽으로 20만큼 이동
	fb_playerdraw();//이동한 좌표에 플레이어 Draw
	usleep(50000);//너무 빠르게 이동하는 것 방지
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
	}
	
}

void fb_pmvright(void)//플레이어 오른쪽으로 이동
{
	
	if(y>20)//화면 바깥으로 빠져나가는 것 방지
	{
	int coor_y = 0;
	int coor_x = 0;
	fb_playererase();//플레이어 Erase
	y=y-20;//플레이어 y좌표 20 감소 >> 오른쪽으로 20만큼 이동
	fb_playerdraw();//이동한 좌표에 플레이어 Draw
	usleep(50000);//너무 빠르게 이동하는 것 방지
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
	}
	
}


void fb_enemydraw(void)//Enemy Character TFT에 표시
{
	int coor_y = 0;
	int coor_x = 0;
	
	// fb clear - black
    for(coor_y = ey-50; coor_y < ey+50; coor_y++) // 적군 현재 y좌표 기준 왼쪽으로 50 오른쪽으로 49만큼 Draw
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y);
        for(coor_x = 0; coor_x < 100; coor_x++)//적군 현재 x좌표 기준으로 99까지 Draw
        {
            *ptr++  =   0x783C00;
        }
    }
	//이하 적군 생김새 구현
	for(coor_y = ey-35; coor_y < ey-19; coor_y++) 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+50;
        for(coor_x = 0; coor_x < 15; coor_x++)
        {
            *ptr++  =   0x000000;
        }
    }
	for(coor_y = ey+35; coor_y > ey+19; coor_y--) 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+50;
        for(coor_x = 0; coor_x < 15; coor_x++)
        {
            *ptr++  =   0x000000;
        }
    }
	for(coor_y = ey-25; coor_y < ey+26; coor_y++) 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+18;
        for(coor_x = 0; coor_x < 13; coor_x++)
        {
            *ptr++  =   0xFF0000;
        }
    }
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
}

void fb_enemyerase(void)//적군 Black으로 Clear
{
	int coor_y = 0;
	int coor_x = 0;
    for(coor_y = ey-50; coor_y < ey+50; coor_y++) 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y);
        for(coor_x = 0; coor_x < 100; coor_x++)
        {
            *ptr++  =   0x000000;
        }
    }
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
}

void fb_emvleft(void)//적군 왼쪽으로 이동
{	
	
	if(ey<540)//화면 바깥으로 나가는 것 방지
	{
	fb_enemyerase();//적군 Erase
	ey=ey+30;//적군 y축에 30 추가 >> 30만큼 왼쪽으로 이동
	fb_enemydraw();//바뀐 위치에 적군 Draw
	usleep(50000);//너무 빠르게 이동하는 것 방지
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
	}
	
}

void fb_emvright(void)//적군 오른쪽으로 이동
{
	
	if(ey>60)//화면 바깥으로 나가는 것 방지
	{
	fb_enemyerase();//적군 Erase
	ey=ey-30;//적군 y축에 30 감소 >> 30만큼 오른쪽으로 이동
	fb_enemydraw();//바뀐 위치에 적군 Draw
	usleep(50000);//너무 빠르게 이동하는 것 방지
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
	}
	
}

static int count=0;//총알 쏘는 시간 제한용 변수

void* fb_enemymove(void)//적군 랜덤 방향으로 이동하며 총알 발사
{
		int random=rand()%2;//적군 왼쪽/오른쪽 이동 랜덤으로 결정
		if(random==0)//랜덤으로 왼쪽/오른쪽 이동
			fb_emvleft();
		else
			fb_emvright();
		if(count==10){//10번 움직일 때
		fb_bulletshow();//총알 발사
		count=0;//count 초기화
		}
		else
		count++;//움직인 횟수 측정
}

void fb_bulletshow(void)//총알 생성
{
	bulletsound();//총알이 나갈 때 효과음 발생
	int coor_y = 0;
	int coor_x = 0;
	if(bulletnum==99)//bullet 구조체 100번째 넘어가는 것 방지
		bulletnum=0;
	bullet[bulletnum].exist=1;//총알 상태 update
	bullet[bulletnum].bx=ex+114;//총알의 x축: 적군의 x축 위치로부터 114 추가
	bullet[bulletnum].by=ey;//총알의 y축: 적군의 y축 위치
    for(coor_y = bullet[bulletnum].by-5; coor_y < bullet[bulletnum].by+5; coor_y++) //총알 y좌표 기준 -5~4좌표 까지 Draw
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+bullet[bulletnum].bx;
        for(coor_x = 0; coor_x < 20; coor_x++)//총알 x좌표 기준 19까지 Draw
        {
            *ptr++  =   0xF7E600;
        }
    }
	bulletnum++;//bulletnum 추가 >> 다음 구조체 생성하기 위함
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
}

void fb_bulleterase(void)//총알 Erase
{
	int coor_y = 0;
	int coor_x = 0;
	// fb clear - black
    for(coor_y =0; coor_y < fbHeight; coor_y++) 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+100;
        for(coor_x = 100; coor_x < 1024-30; coor_x++)
        {
            *ptr++  =   0x000000;
        }
    }
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
}

int fb_bulletmove(void)//총알 움직이기
{
	int coor_y = 0;
	int coor_x = 0;
	for(int i=0;i<100;i++){//모든 총알에 실행
		if(bullet[i].exist==1)//총알이 존재하면 실행
		{
			bullet[i].bx+=20;//총알 x좌표 20 추가

		if(bullet[i].bx>983)//총알이 한계점까지 도달하면 실행
		{
			bullet[i].exist=0;//총알 상태 update >> bullet is not exist
		}
		else//한계점에 도달하지 않았으면 실행
		{		
			for(coor_y = bullet[i].by-5; coor_y < bullet[i].by+5; coor_y++) //움직인 좌표에 총알 draw
			{
        		unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+bullet[i].bx;
        		for(coor_x = 0; coor_x < 20; coor_x++)
        		{
            		*ptr++  =   0xF7E600;
        		}
    		}
		}
		}
		int check=fb_playerfall(i);//총알이 Player와 닿았는지 확인, 게임 Over인지 Chech
		if(check==0)//Game over면 실행
			return 0; //0 반환
	}
	usleep(50000);//너무 빠르게 이동하는 것 방지
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
	return 1;//Game over가 아니면 1 반환
}

void fb_doubleBufSwap(void)
{
	if (currentEmptyBufferPos == 0)
	{
		fbInfo.yoffset = 0;
		currentEmptyBufferPos = DOUBLE_BUFF_START;
	}
	else
	{
		fbInfo.yoffset = fbHeight;
		currentEmptyBufferPos = 0;		
	}
	ioctl(fbfd, FBIOPUT_VSCREENINFO, &fbInfo);	//슉!
}

void fb_varinit(void)//변수 초기화용 >> 게임 다시 시작할 때 사용
{
	currentEmptyBufferPos = 0;
	fbHeight=0;	
	fbWidth=0;	
	x=1024;
	y=300;
	ex=0;
	ey=300;
	bulletnum=0;
	hp=3;
	for(int i=0;i<100;i++){
		bullet[i].exist=0;
	}
}

void fb_close(void)
{
	printf ("Memory UnMapped!\r\n");
    munmap( pfbmap, PFBSIZE);
	printf ("CloseFB\r\n");
    close( fbfd);
}

void fb_write_reverse(char* picData, int picWidth, int picHeight)
{
	int coor_y=0;
	int coor_x=0;
	int targetHeight = (fbHeight<picHeight)?fbHeight:picHeight;	//if Screen과 파일 사이즈가 안맞으면
	int targetWidth = (fbWidth<picWidth)?fbWidth:picWidth;		//if Screen과 파일 사이즈가 안맞으면
	
	for(coor_y = 0; coor_y < targetHeight; coor_y++) 
	{
		int bmpYOffset = coor_y*picWidth*3; ///Every 1Pixel requires 3Bytes.
		int bmpXOffset = 0;
		for (coor_x=0; coor_x < targetWidth; coor_x++)
		{
			//BMP: B-G-R로 인코딩 됨, FB: 0-R-G-B로 인코딩 됨.
			pfbmap[coor_y*fbWidth+ (coor_x) + currentEmptyBufferPos] = 
				((unsigned long)(picData[bmpYOffset+bmpXOffset+0])<<16) 	+
				((unsigned long)(picData[bmpYOffset+bmpXOffset+1])<<8) 		+
				((unsigned long)(picData[bmpYOffset+bmpXOffset+2]));
			bmpXOffset+=3;	//Three Byte.
		}
    }	
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif	
}

void fb_write(char* picData, int picWidth, int picHeight)
{
	int coor_y=0;
	int coor_x=0;
	int targetHeight = (fbHeight<picHeight)?fbHeight:picHeight;	//if Screen과 파일 사이즈가 안맞으면
	int targetWidth = (fbWidth<picWidth)?fbWidth:picWidth;		//if Screen과 파일 사이즈가 안맞으면
	
	for(coor_y = 0; coor_y < targetHeight; coor_y++) 
	{
		int bmpYOffset = coor_y*picWidth*3; ///Every 1Pixel requires 3Bytes.
		int bmpXOffset = 0;
		for (coor_x=0; coor_x < targetWidth; coor_x++)
		{
			//BMP: B-G-R로 인코딩 됨, FB: 0-R-G-B로 인코딩 됨.
			pfbmap[coor_y*fbWidth+ (fbWidth-coor_x) + currentEmptyBufferPos] = 
				((unsigned long)(picData[bmpYOffset+bmpXOffset+2])<<16) 	+
				((unsigned long)(picData[bmpYOffset+bmpXOffset+1])<<8) 		+
				((unsigned long)(picData[bmpYOffset+bmpXOffset+0]));
			bmpXOffset+=3;	//Three Byte.
		}
    }	
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
}

int fb_playerfall(int num)//플레이어가 총알에 맞았는지 확인
{
	if((bullet[num].bx==974)&&(((y-15)<(bullet[num].by+5))&((y+15)>(bullet[num].by-5))))//총알 좌표와 플레이어 좌표 비교 >> 닿았으면 실행
	{
		fb_bulleterase();//총알 Erase
		for(bulletnum=0;bulletnum<100;bulletnum++)//총알 초기화
		{
		bullet[bulletnum].exist=0;
		bullet[bulletnum].bx=ex+114;
		bullet[bulletnum].by=ey;
		}
		bulletnum=0;
		ledminus();//LED 하나 줄이기
		hp--;//목숨 감소
		if(hp==0)//목숨이 0 >> 게임 Over면 실행
			return 0;//0 반환
		for(int i=0;i<3;i++)//플레이어 3번 깜빡이기 >> 맞았음을 표현
		{
			fb_playererase();
			usleep(300000);
			fb_playerdraw();
			usleep(300000);
		}
	}
	return 1; //게임 Over가 아니면 1 반환
}