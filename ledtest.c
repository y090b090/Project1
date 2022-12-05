#include "led.h"


int main(void)
{
    int n=0;
    ledLibInit();
    //ledOnOff(2,1);
    ledallon();

    sleep(1);
    /*for(int i=i;i<8;i++){
        ledStatus();
        ledOnOff(i,1);
        usleep(1);
    }*/
    for(int i=0;i<8;i++){
        ledminus();
        sleep(1);
    }
    

    ledLibExit();
    return 0;
}

