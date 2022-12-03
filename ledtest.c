#include "led.h"


int main(void)
{
    int n=0;
    ledLibInit();
        ledStatus();
        ledOnOff(2,1);
        ledOnOff(1,1);
        ledOnOff(0,1);
        sleep(1);
    ledLibExit();
    return 0;
}

