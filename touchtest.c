
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
							printf("좌측상단터치함");
						}
						else if(B.x <500 && B.y < 100)
						{
							printf("우측상단터치함");
						}
						else if(B.x <100 && B.y < 300)
						{
							printf("좌측하단터치함");
						}
						else if(B.x <100 && B.y < 100)
						{
							printf("우측하단터치함");
						}
						else
						{
							printf("중단 터치한듯? 정확한 위치는 x:%d y:%d \r\n", B.x, B.y);
						}
					}

					
			break;
		}
	}
}
