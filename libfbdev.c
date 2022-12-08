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

#define FBDEV_FILE "/dev/fb0"

struct {
	int exist;
	int bx;
	int by;
}bullet[100];

static int fbfd;
static int fbHeight=0;	//현재 하드웨어의 사이즈
static int fbWidth=0;	//현재 하드웨어의 사이즈
static int x=1024;
static int y=300;
static int ex=0;
static int ey=300;
static unsigned long   *pfbmap;	//프레임 버퍼
static struct fb_var_screeninfo fbInfo;	//To use to do double buffering.
static struct fb_fix_screeninfo fbFixInfo;	//To use to do double buffering.
static int bulletnum=0;
static int hp=3;

#define PFBSIZE 			(fbHeight*fbWidth*sizeof(unsigned long)*2)	//Double Buffering
#define DOUBLE_BUFF_START	(fbHeight*fbWidth)	///Double Swaping
static int currentEmptyBufferPos = 0;
//1 Pixel 4Byte Framebuffer.


int fb_init(int * screen_width, int * screen_height, int * bits_per_pixel, int * line_length)
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

void fb_clear(void)
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

void fb_playerdraw(void)
{
	int coor_y = 0;
	int coor_x = 0;
	// fb clear - black
    for(coor_y = y-15; coor_y < y+15; coor_y++) 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+x-30;
        for(coor_x = 0; coor_x < 30; coor_x++)
        {
            *ptr++  =   0xFFFFFF;
        }
    }
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif

}

void fb_playererase(void)
{
	int coor_y = 0;
	int coor_x = 0;
	// fb clear - black
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

void fb_pmvleft(void)
{	
	
	if(y<580)
	{
	int coor_y = 0;
	int coor_x = 0;
	fb_playererase();
	y=y+20;
	fb_playerdraw();
	usleep(50000);
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
	}
	
}

void fb_pmvright(void)
{
	
	if(y>20)
	{
	int coor_y = 0;
	int coor_x = 0;
	fb_playererase();
	y=y-20;
	fb_playerdraw();
	usleep(50000);
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
	}
	
}


void fb_enemydraw(void)
{
	int coor_y = 0;
	int coor_x = 0;
	
	// fb clear - black
    for(coor_y = ey-50; coor_y < ey+50; coor_y++) 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y);
        for(coor_x = 0; coor_x < 100; coor_x++)
        {
            *ptr++  =   0xFFFFFF;
        }
    }
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
}

void fb_enemyerase(void)
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

void fb_emvleft(void)
{	
	
	if(ey<540)
	{
	fb_enemyerase();
	ey=ey+30;
	fb_enemydraw();
	usleep(50000);
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
	}
	
}

void fb_emvright(void)
{
	
	if(ey>60)
	{
	fb_enemyerase();
	ey=ey-30;
	fb_enemydraw();
	usleep(50000);
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
	}
	
}

static int count=0;

void* fb_enemymove(void)
{
		
		int random=rand()%2;
		if(random==0)
			fb_emvleft();
		else
			fb_emvright();
		if(count==10){
		fb_bulletshow();
		count=0;
		}
		else
		count++;
}

void fb_bulletshow(void)
{
	int coor_y = 0;
	int coor_x = 0;
	if(bulletnum==99)
		bulletnum=0;
	bullet[bulletnum].exist=1;
	bullet[bulletnum].bx=ex+114;
	bullet[bulletnum].by=ey;
	// fb clear - black
    for(coor_y = bullet[bulletnum].by-5; coor_y < bullet[bulletnum].by+5; coor_y++) 
	{
        unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+bullet[bulletnum].bx;
        for(coor_x = 0; coor_x < 20; coor_x++)
        {
            *ptr++  =   0xFFFFFF;
        }
    }
	bulletnum++;
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
}

void fb_bulleterase(void)
{
	int num=bulletnum;
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

int fb_bulletmove(void)
{
	int coor_y = 0;
	int coor_x = 0;
	for(int i=0;i<100;i++){
		if(bullet[i].exist==1)
		{
			bullet[i].bx+=20;

		if(bullet[i].bx>983)
		{
			bullet[i].exist=0;
		}
		else
		{		
			for(coor_y = bullet[i].by-5; coor_y < bullet[i].by+5; coor_y++) 
			{
        		unsigned long *ptr =   pfbmap + currentEmptyBufferPos + (fbWidth * coor_y)+bullet[i].bx;
        		for(coor_x = 0; coor_x < 20; coor_x++)
        		{
            		*ptr++  =   0xFFFFFF;
        		}
    		}
		}
		}
		int check=fb_playerfall(i);
		if(check==0)
			return 0;
	}
	usleep(50000);
	#ifdef ENABLED_DOUBLE_BUFFERING
		fb_doubleBufSwap();
	#endif
	return 1;
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

int fb_playerfall(int num)
{
	if((bullet[num].bx==974)&&(((y-15)<(bullet[num].by+5))&((y+15)>(bullet[num].by-5))))
	{
		fb_bulleterase();
		for(bulletnum=0;bulletnum<100;bulletnum++)
		{
		bullet[bulletnum].exist=0;
		bullet[bulletnum].bx=ex+114;
		bullet[bulletnum].by=ey;
		}
		bulletnum=0;
		ledminus();
		hp--;
		if(hp==0)
			return 0;
		for(int i=0;i<3;i++)
		{
			fb_playererase();
			usleep(300000);
			fb_playerdraw();
			usleep(300000);
		}
	}
	return 1;
}