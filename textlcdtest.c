#include "textlcd.h"


int main(int argc , char **argv)
{
    lcdtextwrite("he","hi", 2);
    sleep(1);
    lcdtextwrite("i'm mh","ss",1);
    sleep(1);
    lcdtextwrite("hello","hello",0);
    sleep(1);
}
