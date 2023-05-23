#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

int main()
{
    struct termios old_settings;
    tcgetattr(0, &old_settings);
    printf("%d", old_settings.c_iflag & IGNBRK);
    return 0;
}