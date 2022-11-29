#include "colorled.h"

int main(){
    pwmLedInit();
    pwmLedOn(100,0,0);
    sleep(5);
    pwmInactiveAll();
}