#ifndef _LIB_FBDEV_H_
#define _LIB_FBDEV_H_
#include "led.h"
//#define ENABLED_DOUBLE_BUFFERING	1	//This makes fb slow...
pthread_t tid;
pthread_mutex_t lock;

int fb_init(int * screen_width, int * screen_height, int * bits_per_pixel, int * line_length);
void fb_close(void);
void fb_clear(void);
void fb_write(char *picData, int picWidth, int picHeight);
void fb_write_reverse(char *picData, int picWidth, int picHeight);
void fb_doubleBufSwap(void);
void fb_playerdraw(void);
void fb_playererase(void);
void fb_pmvleft(void);
void fb_pmvright(void);
void fb_enemydraw(void);
void* fb_enemymove(void);
void fb_enemyerase(void);
void fb_emvleft(void);
void fb_emvright(void);
void fb_bulletshow(void);
void fb_bulleterase(void);
int fb_bulletmove(void);
int fb_playerfall(int num);
void fb_varinit(void);
#endif