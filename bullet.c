#include "buzzer.h"

int main(void){
    buzzerInit();

    while(1)
    {
    bullet();
    }
    buzzerExit();
}
