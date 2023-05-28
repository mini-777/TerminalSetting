#include <stdlib.h>
#include <curses.h>
#include <menu.h>
#include <string.h>
#include <termios.h>
#include <signal.h>

#include <unistd.h>

char *choices[] = {
    "C_Iflags",
    "C_Lflags",
    "ALL FLIP & EXIT",
    "Save & Exit",
};

char *I_choices[] = {
    "IGNBRK",
    "BRKINT",
    "IGNPAR",
    "PARMRK",
    "INPCK",
    "ISTRIP",
    "INLCR",
    "IGNCR",
    "ICRNL",
    "IXON",
    "IXOFF"
    "Back",

};
char *O_choices[] = {
    "OLCUC",
    "Back",

};
char *L_choices[] = {
    "ISIG",
    "ICANON",
    "ECHO",
    "ECHOE",
    "ECHOK",
    "Back",

};

MENU *create_menu(char **choices, int n_choices)
{
    ITEM **my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

    for (int i = 0; i < n_choices; ++i)
        my_items[i] = new_item(choices[i], "");
    MENU *my_menu = new_menu((ITEM **)my_items);
    return my_menu;
}
void create_window(WINDOW *flags_win, MENU *menu, int max_y, int max_x)
{
    WINDOW *sub_win = derwin(flags_win, 16, 20, 5, 13);
    keypad(flags_win, TRUE);
    set_menu_win(menu, flags_win);
    set_menu_sub(menu, sub_win);

    wbkgd(stdscr, COLOR_PAIR(1));
    wbkgd(flags_win, COLOR_PAIR(1));
    set_menu_fore(menu, COLOR_PAIR(2));
    set_menu_back(menu, COLOR_PAIR(1));
    box(flags_win, 0, 0);

    mvwprintw(flags_win, 0, max_x / 2 - 10, "TERMINAL SETTING");
    mvwprintw(flags_win, max_y - 5, max_x - 15, "F1 to Exit");
}

int main()
{
    struct termios settings;
    tcgetattr(STDIN_FILENO, &settings);

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);

    // 메뉴 구성 및 생성
    int n_choices, i_choices, o_choices, l_choices;
    n_choices = sizeof(choices) / sizeof(char *);
    i_choices = sizeof(I_choices) / sizeof(char *);
    o_choices = sizeof(O_choices) / sizeof(char *);
    l_choices = sizeof(L_choices) / sizeof(char *);

    MENU *my_menu = create_menu(choices, n_choices);
    MENU *i_menu = create_menu(I_choices, i_choices);
    MENU *o_menu = create_menu(O_choices, o_choices);
    MENU *l_menu = create_menu(L_choices, l_choices);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    WINDOW *menu_win = newwin(max_y - 3, max_x - 4, 1, 1);
    WINDOW *sub_win = derwin(menu_win, 7, 20, max_y / 2 - 6, max_x / 2 - 9);
    // 윈도우 구성
    keypad(menu_win, TRUE);
    set_menu_win(my_menu, menu_win);
    set_menu_sub(my_menu, sub_win);

    wbkgd(stdscr, COLOR_PAIR(1));
    wbkgd(menu_win, COLOR_PAIR(1));
    set_menu_fore(my_menu, COLOR_PAIR(2));
    set_menu_back(my_menu, COLOR_PAIR(1));
    box(menu_win, 0, 0);
    mvwprintw(menu_win, 0, max_x / 2 - 10, "TERMINAL SETTING");
    mvwprintw(menu_win, max_y - 5, max_x - 15, "F1 to Exit");

    WINDOW *iflags_win = newwin(max_y - 3, max_x - 4, 1, 1);

    WINDOW *lflags_win = newwin(max_y - 3, max_x - 4, 1, 1);
    create_window(iflags_win, i_menu, max_y, max_x);
    create_window(lflags_win, l_menu, max_y, max_x);

    mvwprintw(iflags_win, 0, max_x / 2 - 10, "TERMINAL SETTING");
    mvwprintw(iflags_win, 2, max_x / 2 - 8, "Input  Flags!");
    mvwprintw(iflags_win, 4, max_x - 18, (settings.c_iflag & IGNBRK) ? "ON" : "OFF");
    mvwprintw(iflags_win, 6, max_x - 18, (settings.c_iflag & BRKINT) ? "ON" : "OFF");
    mvwprintw(iflags_win, 7, max_x - 18, (settings.c_iflag & IGNPAR) ? "ON" : "OFF");
    mvwprintw(iflags_win, 8, max_x - 18, (settings.c_iflag & PARMRK) ? "ON" : "OFF");
    mvwprintw(iflags_win, 9, max_x - 18, (settings.c_iflag & INPCK) ? "ON" : "OFF");
    mvwprintw(iflags_win, 10, max_x - 18, (settings.c_iflag & ISTRIP) ? "ON" : "OFF");
    mvwprintw(iflags_win, 11, max_x - 18, (settings.c_iflag & INLCR) ? "ON" : "OFF");
    mvwprintw(iflags_win, 12, max_x - 18, (settings.c_iflag & IGNCR) ? "ON" : "OFF");
    mvwprintw(iflags_win, 13, max_x - 18, (settings.c_iflag & ICRNL) ? "ON" : "OFF");
    mvwprintw(iflags_win, 14, max_x - 18, (settings.c_iflag & IXON) ? "ON" : "OFF");
    mvwprintw(iflags_win, 15, max_x - 18, (settings.c_iflag & IXOFF) ? "ON" : "OFF");
    mvwprintw(iflags_win, max_y - 5, max_x - 15, "F1 to Exit");

    mvwprintw(lflags_win, 0, max_x / 2 - 10, "TERMINAL SETTING");
    mvwprintw(lflags_win, 2, max_x / 2 - 9, "Local  Flags!");
    mvwprintw(lflags_win, 5, max_x - 18, (settings.c_lflag & ISIG) ? "ON" : "OFF");
    mvwprintw(lflags_win, 6, max_x - 18, (settings.c_lflag & ICANON) ? "ON" : "OFF");
    mvwprintw(lflags_win, 7, max_x - 18, (settings.c_lflag & ECHO) ? "ON" : "OFF");
    mvwprintw(lflags_win, 8, max_x - 18, (settings.c_lflag & ECHOE) ? "ON" : "OFF");
    mvwprintw(lflags_win, 9, max_x - 18, (settings.c_lflag & ECHOK) ? "ON" : "OFF");
    mvwprintw(lflags_win, max_y - 5, max_x - 15, "F1 to Exit");

    // 사용자 입력 처l

    int c;
    int m = 0, i = 0, o = 0, l = 0;
    ITEM *cur_item;
    while (c != KEY_F(1) && l == 0)
    {
        if (m == 0 && i == 0)
        {
            post_menu(my_menu);
            touchwin(menu_win);
            wrefresh(menu_win);
            c = wgetch(menu_win);
            switch (c)
            {
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
            case 10:
                cur_item = current_item(my_menu);
                if (strcmp(item_name(cur_item), "C_Iflags") == 0)
                {
                    m = 0, i = 1;
                    break;
                }
                else if (strcmp(item_name(cur_item), "C_Lflags") == 0)
                {
                    m = 0, i = 2;
                    break;
                }
                else if (strcmp(item_name(cur_item), "ALL FLIP & EXIT") == 0)
                {
                    m = 0, i = 3;
                    break;
                }

                else if (strcmp(item_name(cur_item), "Save & Exit") == 0)
                {

                    l = 1;
                    break;
                }
                break;
            }
        }
        else if (m == 0 && i == 1)
        {
            post_menu(i_menu);
            touchwin(iflags_win);
            wrefresh(iflags_win);
            c = wgetch(iflags_win);
            switch (c)
            {
            case KEY_DOWN:
                menu_driver(i_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(i_menu, REQ_UP_ITEM);
                break;
            case 10:
                cur_item = current_item(i_menu);
                if (strcmp(item_name(cur_item), "IGNBRK") == 0 && (settings.c_iflag & IGNBRK))
                {
                    mvwprintw(iflags_win, 4, max_x - 18, "OFF");
                    settings.c_iflag &= ~IGNBRK;
                }
                else if (strcmp(item_name(cur_item), "IGNBRK") == 0 && !(settings.c_iflag & IGNBRK))
                {
                    mvwprintw(iflags_win, 4, max_x - 18, "ON ");
                    settings.c_iflag |= IGNBRK;
                }
                if (strcmp(item_name(cur_item), "BRKINT") == 0 && (settings.c_iflag & BRKINT))
                {
                    mvwprintw(iflags_win, 6, max_x - 18, "OFF");
                    settings.c_iflag &= ~BRKINT;
                }
                else if (strcmp(item_name(cur_item), "BRKINT") == 0 && !(settings.c_iflag & BRKINT))
                {
                    mvwprintw(iflags_win, 6, max_x - 18, "ON ");
                    settings.c_iflag |= BRKINT;
                }
                if (strcmp(item_name(cur_item), "IGNPAR") == 0 && (settings.c_iflag & IGNPAR))
                {
                    mvwprintw(iflags_win, 7, max_x - 18, "OFF");
                    settings.c_iflag &= ~IGNPAR;
                }
                else if (strcmp(item_name(cur_item), "IGNPAR") == 0 && !(settings.c_iflag & IGNPAR))
                {
                    mvwprintw(iflags_win, 7, max_x - 18, "ON ");
                    settings.c_iflag |= IGNPAR;
                }
                if (strcmp(item_name(cur_item), "PARMRK") == 0 && (settings.c_iflag & PARMRK))
                {
                    mvwprintw(iflags_win, 8, max_x - 18, "OFF");
                    settings.c_iflag &= ~PARMRK;
                }
                else if (strcmp(item_name(cur_item), "PARMRK") == 0 && !(settings.c_iflag & PARMRK))
                {
                    mvwprintw(iflags_win, 8, max_x - 18, "ON ");
                    settings.c_iflag |= PARMRK;
                }
                if (strcmp(item_name(cur_item), "INPCK") == 0 && (settings.c_iflag & INPCK))
                {
                    mvwprintw(iflags_win, 9, max_x - 18, "OFF");
                    settings.c_iflag &= ~INPCK;
                }
                else if (strcmp(item_name(cur_item), "INPCK") == 0 && !(settings.c_iflag & INPCK))
                {
                    mvwprintw(iflags_win, 9, max_x - 18, "ON ");
                    settings.c_iflag |= INPCK;
                }
                if (strcmp(item_name(cur_item), "ISTRIP") == 0 && (settings.c_iflag & ISTRIP))
                {
                    mvwprintw(iflags_win, 10, max_x - 18, "OFF");
                    settings.c_iflag &= ~ISTRIP;
                }
                else if (strcmp(item_name(cur_item), "ISTRIP") == 0 && !(settings.c_iflag & ISTRIP))
                {
                    mvwprintw(iflags_win, 10, max_x - 18, "ON ");
                    settings.c_iflag |= ISTRIP;
                }
                if (strcmp(item_name(cur_item), "INLCR") == 0 && (settings.c_iflag & INLCR))
                {
                    mvwprintw(iflags_win, 11, max_x - 18, "OFF");
                    settings.c_iflag &= ~INLCR;
                }
                else if (strcmp(item_name(cur_item), "INLCR") == 0 && !(settings.c_iflag & INLCR))
                {
                    mvwprintw(iflags_win, 11, max_x - 18, "ON ");
                    settings.c_iflag |= INLCR;
                }

                if (strcmp(item_name(cur_item), "IGNCR") == 0 && (settings.c_iflag & IGNCR))
                {
                    mvwprintw(iflags_win, 12, max_x - 18, "OFF");
                    settings.c_iflag &= ~IGNCR;
                }
                else if (strcmp(item_name(cur_item), "IGNCR") == 0 && !(settings.c_iflag & IGNCR))
                {
                    mvwprintw(iflags_win, 12, max_x - 18, "ON ");
                    settings.c_iflag |= IGNCR;
                }

                if (strcmp(item_name(cur_item), "ICRNL") == 0 && (settings.c_iflag & ICRNL))
                {
                    mvwprintw(iflags_win, 13, max_x - 18, "OFF");
                    settings.c_iflag &= ~ICRNL;
                }
                else if (strcmp(item_name(cur_item), "ICRNL") == 0 && !(settings.c_iflag & ICRNL))
                {
                    mvwprintw(iflags_win, 13, max_x - 18, "ON ");
                    settings.c_iflag |= ICRNL;
                }

                if (strcmp(item_name(cur_item), "IXON") == 0 && (settings.c_iflag & IXON))
                {
                    mvwprintw(iflags_win, 14, max_x - 18, "OFF");
                    settings.c_iflag &= ~IXON;
                }
                else if (strcmp(item_name(cur_item), "IXON") == 0 && !(settings.c_iflag & IXON))
                {
                    mvwprintw(iflags_win, 14, max_x - 18, "ON ");
                    settings.c_iflag |= IXON;
                }
                if (strcmp(item_name(cur_item), "IXOFF") == 0 && (settings.c_iflag & IXOFF))
                {
                    mvwprintw(iflags_win, 15, max_x - 18, "OFF");
                    settings.c_iflag &= ~IXOFF;
                }
                else if (strcmp(item_name(cur_item), "IXOFF") == 0 && !(settings.c_iflag & IXOFF))
                {
                    mvwprintw(iflags_win, 15, max_x - 18, "ON ");
                    settings.c_iflag |= IXOFF;
                }
                if (strcmp(item_name(cur_item), "Back") == 0)
                {
                    m = 0, i = 0;

                    break;
                }
                break;
            }
        }
        else if (m == 0 && i == 2)
        {
            post_menu(l_menu);
            touchwin(lflags_win);
            wrefresh(lflags_win);
            c = wgetch(lflags_win);
            switch (c)
            {
            case KEY_DOWN:
                menu_driver(l_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(l_menu, REQ_UP_ITEM);
                break;
            case 10:
                cur_item = current_item(l_menu);
                if (strcmp(item_name(cur_item), "ISIG") == 0 && (settings.c_lflag & ISIG))
                {
                    mvwprintw(lflags_win, 5, max_x - 18, "OFF");
                    settings.c_lflag &= ~ISIG;
                }
                else if (strcmp(item_name(cur_item), "ISIG") == 0 && !(settings.c_lflag & ISIG))
                {
                    mvwprintw(lflags_win, 5, max_x - 18, "ON ");
                    settings.c_lflag |= ISIG;
                }
                if (strcmp(item_name(cur_item), "ICANON") == 0 && (settings.c_lflag & ICANON))
                {
                    mvwprintw(lflags_win, 6, max_x - 18, "OFF");
                    settings.c_lflag &= ~ICANON;
                }
                else if (strcmp(item_name(cur_item), "ICANON") == 0 && !(settings.c_lflag & ICANON))
                {
                    mvwprintw(lflags_win, 6, max_x - 18, "ON ");
                    settings.c_lflag |= ICANON;
                }
                if (strcmp(item_name(cur_item), "ECHO") == 0 && (settings.c_lflag & ECHO))
                {
                    mvwprintw(lflags_win, 7, max_x - 18, "OFF");
                    settings.c_lflag &= ~ECHO;
                }
                else if (strcmp(item_name(cur_item), "ECHO") == 0 && !(settings.c_lflag & ECHO))
                {
                    mvwprintw(lflags_win, 7, max_x - 18, "ON ");
                    settings.c_lflag |= ECHO;
                }
                if (strcmp(item_name(cur_item), "ECHOE") == 0 && (settings.c_lflag & ECHOE))
                {
                    mvwprintw(lflags_win, 8, max_x - 18, "OFF");
                    settings.c_lflag &= ~ECHOE;
                }
                else if (strcmp(item_name(cur_item), "ECHOE") == 0 && !(settings.c_lflag & ECHOE))
                {
                    mvwprintw(lflags_win, 8, max_x - 18, "ON ");
                    settings.c_lflag |= ECHOE;
                }
                if (strcmp(item_name(cur_item), "ECHOK") == 0 && (settings.c_lflag & ECHOK))
                {
                    mvwprintw(lflags_win, 9, max_x - 18, "OFF");
                    settings.c_lflag &= ~ECHOK;
                }
                else if (strcmp(item_name(cur_item), "ECHOK") == 0 && !(settings.c_lflag & ECHOK))
                {
                    mvwprintw(lflags_win, 9, max_x - 18, "ON ");
                    settings.c_lflag |= ECHOK;
                }
                if (strcmp(item_name(cur_item), "Back") == 0)
                {
                    m = 0, i = 0;

                    break;
                }
                break;
            }
        }
        else if (m == 0 && i == 3)
        {
            settings.c_iflag ^= (IGNBRK | BRKINT | IGNPAR | PARMRK | INPCK | ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXOFF);
            settings.c_lflag ^= (ISIG | ICANON | ECHO | ECHOE | ECHOK);
            l = 1;
        }
    }
    // 초기화

    unpost_menu(my_menu);
    free_menu(my_menu);
    endwin();
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);

    return 0;
}
