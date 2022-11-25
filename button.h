#ifndef _BUTTON_H_
#define _BUTTON_H_

#define MESSAGE_ID 1122
typedef struct
{
	long int messageNum;
	int keyInput;
	int pressed;
} BUTTON_MST_T;


int buttonExit(void);
int buttonInit(void);
void buttonThFunc(void);
int probeButtonPath(char *newPath);
#endif
