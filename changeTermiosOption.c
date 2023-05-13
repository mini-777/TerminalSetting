#include <termios.h>
#include <stdbool.h>

void change_iflagOpt_toggle(int value, struct termios *options)
{
    options->c_iflag ^= value;
}
void change_oflagOpt_toggle(int value, struct termios *options)
{
    options->c_oflag ^= value;
}
void change_lflagOpt_toggle(int value, struct termios *options)
{   
    options->c_lflag ^= value;
}

bool checkONOFF(int value, struct termios *options){
    if(options->c_iflag &= value) return true;
    else return false;
}
