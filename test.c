#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

struct flaginfo
{
    int fl_value;
    char *fl_name;
};

void showbaud(int thespeed);
void show_some_flags(struct termios *ttyp);
void show_flagset(int thevalue, struct flaginfo thebitnames[]);
void checkopt(int argc, char *argv[], struct termios *ttyp);
void set_Iflag(int argc, char *argv[], struct termios *ttyp);
void set_Oflag(int argc, char *argv[]);
void set_Lflag(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    struct termios ttyinfo;
    if (tcgetattr(0, &ttyinfo) == -1)
    {
        perror("cannot get params about stdin");
        exit(1);
    }
    showbaud(cfgetospeed(&ttyinfo));
    printf("erase = ^%c;",
           ttyinfo.c_cc[VERASE] - 1 + 'A');
    printf("kill = ^%c;\n",
           ttyinfo.c_cc[VKILL] - 1 + 'A');
    if (argc > 1)
    {
        tcgetattr(STDIN_FILENO, &ttyinfo);
        checkopt(argc, argv, &ttyinfo);
        tcsetattr(STDIN_FILENO, TCSANOW, &ttyinfo);
    }
    show_some_flags(&ttyinfo);
}

void showbaud(int thespeed)
{
    switch (thespeed)
    {
    case B300:
        printf("speed 300 baud;\n");
        break;
    case B600:
        printf("speed 600 baud;\n");
        break;
    case B1200:
        printf("speed 1200 baud;\n");
        break;
    case B1800:
        printf("speed 1800 baud;\n");
        break;
    case B2400:
        printf("speed 2400 baud;\n");
        break;
    case B4800:
        printf("speed 4800 baud;\n");
        break;
    case B9600:
        printf("speed 9600 baud;\n");
        break;
    default:
        printf("Fast;\n");
        break;
    }
}

struct flaginfo input_flags[] = {
    IGNBRK, "ignbrk",
    BRKINT, "brkint",
    IGNPAR, "ignpar",
    PARMRK, "parmrk",
    INPCK, "inpck",
    ISTRIP, "istrip",
    INLCR, "inlcr",
    IGNCR, "igncr",
    ICRNL, "icrnl",
    IXON, "ixon",
    IXOFF, "ixoff",
    0, NULL};

struct flaginfo output_flags[] = {
    OLCUC, "olcuc",
    0, NULL};

struct flaginfo local_flags[] = {
    ISIG, "isig",
    ICANON, "icanon",
    ECHO, "echo",
    ECHOE, "echoe",
    ECHOK, "echok",
    0, NULL};

void show_some_flags(struct termios *ttyp)
{
    show_flagset(ttyp->c_iflag, input_flags);
    show_flagset(ttyp->c_oflag, output_flags);
    show_flagset(ttyp->c_lflag, local_flags);
}

void show_flagset(int thevalue, struct flaginfo thebitnames[])
{
    int i;
    for (i = 0; thebitnames[i].fl_value; i++)
    {
        if (thevalue & thebitnames[i].fl_value)
            printf("%s ", thebitnames[i].fl_name);
        else
            printf("-%s ", thebitnames[i].fl_name);
    }
    printf("\n");
}

void checkopt(int argc, char *argv[], struct termios *ttyp)
{
    if (strcmp(argv[1], "-FL") == 0)
    {
        if (argc > 2)
        {
            for (int i = 2; i < argc; i++)
            {
                if (strcmp(argv[i], "isig") == 0)
                    ttyp->c_lflag ^= ISIG;
                else if (strcmp(argv[i], "icanon") == 0)
                    ttyp->c_lflag ^= ICANON;
                else if (strcmp(argv[i], "echo") == 0)
                    ttyp->c_lflag ^= ECHO;
                else if (strcmp(argv[i], "echoe") == 0)
                    ttyp->c_lflag ^= ECHOE;
                else if (strcmp(argv[i], "echok") == 0)
                    ttyp->c_lflag ^= ECHOK;
            }
        }
    }
    else if (strcmp(argv[1], "-FO") == 0)
    {
        ttyp->c_oflag ^= OLCUC;
    }
    else if (strcmp(argv[1], "-FI") == 0)
    {
        if (argc > 2)
        {
            for (int i = 2; i < argc; i++)
            {
                if (strcmp(argv[i], "ignbrk") == 0)
                    ttyp->c_iflag ^= IGNBRK;
                else if (strcmp(argv[i], "brkint") == 0)
                    ttyp->c_iflag ^= BRKINT;
                else if (strcmp(argv[i], "ignpar") == 0)
                    ttyp->c_iflag ^= IGNPAR;
                else if (strcmp(argv[i], "parmrk") == 0)
                    ttyp->c_iflag ^= PARMRK;
                else if (strcmp(argv[i], "inpck") == 0)
                    ttyp->c_iflag ^= INPCK;
                else if (strcmp(argv[i], "istrip") == 0)
                    ttyp->c_iflag ^= ISTRIP;
                else if (strcmp(argv[i], "inlcr") == 0)
                    ttyp->c_iflag ^= INLCR;
                else if (strcmp(argv[i], "igncr") == 0)
                    ttyp->c_iflag ^= IGNCR;
                else if (strcmp(argv[i], "icrnl") == 0)
                    ttyp->c_iflag ^= ICRNL;
                else if (strcmp(argv[i], "ixon") == 0)
                    ttyp->c_iflag ^= IXON;
                else if (strcmp(argv[i], "ixoff") == 0)
                    ttyp->c_iflag ^= IXOFF;
            }
        }
    }
    else if (strcmp(argv[1], "-A") == 0)
    {
        ttyp->c_iflag ^= (IGNBRK | BRKINT | IGNPAR | PARMRK | INPCK | ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXOFF);
        ttyp->c_oflag ^= OLCUC;
        ttyp->c_lflag ^= (ISIG | ICANON | ECHO | ECHOE | ECHOK);
    }
}
