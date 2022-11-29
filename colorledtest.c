#include "colorled.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    pwmLedInit();
    pwmLedOn(0,0,100);
    pwmInactiveAll();
}