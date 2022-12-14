#include "touch.h"

char touchPath[200]={0,};
int fc,msgid;
pthread_t touchTh_id;
int probetouchPath(char *newPath)
{
    int returnValue = 0; //touch에 해당하는 event#을 찾았나?
    int number = 0; //찾았다면 여기에 집어넣자
    FILE *fp = fopen(PROBE_FILE,"rt"); //파일을 열고
    while(!feof(fp)) //끝까지 읽어들인다.
    {
        char tmpStr[200]; //200자를 읽을 수 있게 버퍼
        fgets(tmpStr,200,fp); //최대 200자를 읽어봄
        //printf ("%s",tmpStr);
    if (strcmp(tmpStr,HAVE_TO_FIND_1) == 0)
    {
        printf("YES! I found!: %s\r\n", tmpStr);
        returnValue = 1; //찾음
    }
    if ((returnValue == 1) && (strncasecmp(tmpStr, HAVE_TO_FIND_2, strlen(HAVE_TO_FIND_2)) == 0))
    {
        printf ("-->%s",tmpStr);
        printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
        number = tmpStr[strlen(tmpStr)-3] - '0'; 
        //Ascii character '0'-'9' (0x30-0x39) 
        //to interger(0)
        break;
    }
    }
    fclose(fp);
    if (returnValue == 1){
        sprintf (newPath,"%s%d",INPUT_DEVICE_LIST,number);
    }
   
    return returnValue;
}

int touchInit(void)
{
    printf("hello\n");
    if (probetouchPath(touchPath) == 0){
        printf("error\n");
        return 0;
    }
        
    fc=open (touchPath, O_RDONLY);
    msgid = msgget (MESSAGE_ID, IPC_CREAT|0666);
    if(msgid == -1){
            printf("cannot find\n");
            return 1;
    }
    pthread_create(&touchTh_id, NULL, &touchThFunc, NULL);
    return 1;
}


int touchExit(void)
{
    pthread_join(touchTh_id,NULL);
    return 0;
}

void* touchThFunc(void *arg)
{
    int x = 0;
    int y = 0;
    int readSize;
    B.messageNum=1;
    B.keyInput=999;
    while(1)
    {
        readSize=read(fc,&A,sizeof(A));
        if(readSize!=sizeof(A)){
            continue;
        }
        if (A.type == EV_ABS)
        {
            //무슨 값 들어옴
            if (A.code == ABS_MT_POSITION_X)
            {
                x =  A.value;
            }
            else if (A.code == ABS_MT_POSITION_Y)
            {
                y =  A.value;
            }
        }

        else if ((A.type == EV_KEY) && (A.code == BTN_TOUCH) )
        {
            B.x = x;
            B.y = y;
            if (A.value == 0) 
            {
                B.pressed = 0;
            }
            else if (A.value == 1)
            {
                B.pressed = 1;
            }
        }
        msgsnd(msgid,&B,sizeof(B)-sizeof(long int),0); 
    }
    close(fc);
}
