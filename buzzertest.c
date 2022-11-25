#include "buzzer.h"

int main(void){
    buzzerInit();
    buzzerPlaySong(440);
    sleep(1);
    buzzerExit();
}