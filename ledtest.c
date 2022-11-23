//#include "libMyPeri.a"
#include "led.h"


int main(void)
{
    int n=0;
    doHelp();
    ledLibInit();
    for(int n=0;n<8;n++){
        ledStatus();
        ledOnOff(n,1);
        sleep(1);
    }
    ledLibExit();
    return 0;
}

