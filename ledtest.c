#include "libMyPeri.a"

int main(int argc , char **argv)
{
    ledLibInit();
    ledOnOff(8,1);
    ledStatus();
    ledOnOff(7,1);
    ledLibExit();
    return 0;
}

