#include "button.h"

char buttonPath[200]={0,};
int ft,msgID;
pthread_t buttonTh_id;
int probeButtonPath(char *newPath)
{
    int returnValue = 0; //button에 해당하는 event#을 찾았나?
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




int buttonInit(void)    //button초기설정
{
    printf("hello\n");
    if (probeButtonPath(buttonPath) == 0){  //buttonPath를 찾지 못했다면
        printf("error\n");  //error을 출력하고 종료
        return 0;
    }
        
    ft=open (buttonPath, O_RDONLY); //buttonPath를 open
    msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);    //msgID의 값을 얻는다.
    if(msgID == -1){    //msgID가 -1을 리턴하면 실패
            printf("cannot find\n");
            return 1;
    }
    pthread_create(&buttonTh_id, NULL, &buttonThFunc, NULL);    //스레드에서 메시지를 보내기위해 스레드 생성
    return 1;
}


int buttonExit(void)    //button 종료
{
    pthread_cancel(buttonTh_id);    //스레드 종료
    return 0;
}

void* buttonThFunc(void *arg)   //스레드에서 실행될 함수
{
    int readSize;   
    B.messageNum=1; //longint 변수 1로 설정
    while(1)
    {
        readSize=read(ft,&A,sizeof(A)); //A의 값을 읽어온다.
        if(readSize!=sizeof(A)){    //만약 A의 크기만큼 읽어오지 못하면 다시 continue
            continue;
        }
        B.keyInput=A.code;  //A.code를 B.keyInput에 저장
        B.pressed=A.value;  //A.value를 B.pressed에 저장
        B.type=A.type;  //A.type을 B.type에 저장
        msgsnd(msgID,&B,sizeof(B)-sizeof(long int),0);  //B의 값을 msgsnd로 보낸다.
    }
    close(ft);  //ft 닫기
}
