#include "buzzer.h"

int main(void){
    buzzerInit();
    buzzerPlaySong(70);
    sleep(1);
    buzzerPlaySong(500);
    sleep(1);
    buzzerPlaySong(460);
    sleep(1);
    buzzerPlaySong(357);
    sleep(1);
    buzzerPlaySong(44);
    sleep(1);
    buzzerExit();
}
