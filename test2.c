#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

int main()
{
    struct termios old_settings;
    tcgetattr(0, &old_settings);
    printf("%d", old_settings.c_iflag & IGNBRK);
    if (old_settings.c_iflag & IGNBRK)
    {
        old_settings.c_iflag &= ~IGNBRK;
    }
    else
    {
        old_settings.c_iflag |= IGNBRK;
    }
    tcsetattr(0, TCSANOW, &old_settings);
    return 0;
}