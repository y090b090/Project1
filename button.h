#ifndef _BUTTON_H_
#define _BUTTON_H_

#define MESSAGE_ID 1122
typedef struct
{
	long int messageNum;
	int ketInput;
	int pressed;
} BUTTON_MST_T;

int buttonInit(void);
int buttonExit(void);
#endif