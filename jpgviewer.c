#include <stdio.h>
#include <stdlib.h>
#include <sys/kd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "libfbdev.h"
#include "libjpeg/jpeglib.h"

int menuviewer(void)
{
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int cols = 0, rows = 0;
	char *data;
	//FrameBuffer init
    if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )
	{
		printf ("FrameBuffer Init Failed\r\n");
		return 0;
	}
	
	int conFD = open ("/dev/tty0", O_RDWR);	// /dev/tty0를 open
	ioctl(conFD, KDSETMODE, KD_GRAPHICS);	
	close (conFD);
	
	//Clear FB.
	fb_clear();
	
	
	//FileRead
	int error=0;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
 	cinfo.err = jpeg_std_error(&jerr);	//jpeg_std_error 함수 실행
	jpeg_create_decompress(&cinfo);	//jpeg_create_decompress 실행
	FILE *fp = fopen("menu.jpg", "rb"); //menu.jpg를 open
	jpeg_stdio_src(&cinfo, fp);	//jpeg_stdio_src실행
	jpeg_read_header(&cinfo, TRUE); 	//jpeg_read_header실행
	//printf ("JPG %d by %d by %d, %d\n",
	//	cinfo.image_width,cinfo.image_height,cinfo.num_components, cinfo.output_scanline);
	cols = cinfo.image_width;
	rows = cinfo.image_height;

	data = malloc(cols*rows*3);	
	int currPoint = 0;
	jpeg_start_decompress(&cinfo);		//jpeg_start_decompress 실행
	while(cinfo.output_scanline < cinfo.output_height) 
	{
		//printf ("CInfoScanlines:%d\r\n",cinfo.output_scanline);
		char *tempPtr=&data[currPoint];
		jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&tempPtr, 1);
		currPoint+=cols*3;
	}
	jpeg_finish_decompress(&cinfo);		//jpeg_finish_decompress실행
	jpeg_destroy_decompress(&cinfo);	//jpeg_destroy_decompress 실행
	fclose(fp);

	fb_write_reverse(data, cols,rows);
	free(data);

	fb_close();
    return 0;
}


int gameoverviewer(void)
{
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int cols = 0, rows = 0;
	char *data;
	//FrameBuffer init
    if ( fb_init(&screen_width, &screen_height, &bits_per_pixel, &line_length) < 0 )
	{
		printf ("FrameBuffer Init Failed\r\n");
		return 0;
	}
	
	int conFD = open ("/dev/tty0", O_RDWR);
	ioctl(conFD, KDSETMODE, KD_GRAPHICS);
	close (conFD);
	
	//Clear FB.
	fb_clear();
	
	
	//FileRead
	int error=0;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
 	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	FILE *fp = fopen("gameover.jpg", "rb");			// gameover.jpg 를 open한다.
	jpeg_stdio_src(&cinfo, fp);
	jpeg_read_header(&cinfo, TRUE); 
	//printf ("JPG %d by %d by %d, %d\n",
	//	cinfo.image_width,cinfo.image_height,cinfo.num_components, cinfo.output_scanline);
	cols = cinfo.image_width;
	rows = cinfo.image_height;

	data = malloc(cols*rows*3);
	int currPoint = 0;
	jpeg_start_decompress(&cinfo);
	while(cinfo.output_scanline < cinfo.output_height) 
	{
		//printf ("CInfoScanlines:%d\r\n",cinfo.output_scanline);
		char *tempPtr=&data[currPoint];
		jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&tempPtr, 1);
		currPoint+=cols*3;
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(fp);

	fb_write_reverse(data, cols,rows);
	free(data);

	fb_close();
    return 0;
}
