#include "textlcd.h"


int main(int argc , char **argv)
{
    lcdtextwrite("hello","world", 2);
    sleep(1);
    lcdtextwrite("come","back",1);
    sleep(1);
    lcdtextwrite("he","hi",0);
    sleep(1);
}
