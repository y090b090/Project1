#include "buzzer.h"

int main(void){
    buzzerInit();
    buzzerPlaySong(520);
    sleep(1);
    buzzerPlaySong(220);
    sleep(1);
    buzzerPlaySong(440);
    sleep(1);
    buzzerExit();
}
