#include "touch.h"

int main(void)
{
	touchInit();
	int msgID = msgget (MESSAGE_ID,IPC_CREAT|0666);
	while(1)
	{
		msgrcv(msgID, &B,sizeof(B)-sizeof(long int),0, 0);
		switch(B.keyInput)
		{
			case 999:
					if(B.pressed == 1);
					{
						if(B.x <100 && B.y < 100)
						{
							printf("left up touch");
						}
						else if(B.x <500 && B.y < 100)
						{
							printf("right up touch");
						}
						else if(B.x <100 && B.y < 300)
						{
							printf("left down touch");
						}
						else if(B.x <100 && B.y < 100)
						{
							printf("right down touch");
						}
						else
						{
							printf("real position x:%d y:%d \r\n", B.x, B.y);
						}
					}

					
			break;
		}
	}
}
