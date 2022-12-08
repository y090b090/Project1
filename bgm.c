#include "buzzer.h"

int main(void){
    buzzerInit();

    while(1)
    {
    bgm();
    }
    buzzerExit();
}
