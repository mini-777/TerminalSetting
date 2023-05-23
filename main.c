#include <stdlib.h>
#include <curses.h>
#include <menu.h>
#include <string.h>
#include <termios.h>

char *choices[] = {
    "C_Iflags",
    "C_Oflags",
    "C_Cflags",
    "Save & Exit",
};

char *I_choices[] = {
    "IGNBRK",
    "BRKINT",
    "IGNPAR",
    "PARMRK",
    "INPCK",
    "Back",
    "Save & Exit",
};
char *O_choices[] = {
    "OPOST",
    "ONLCR",
    "OXTABS",
    "ONOEOT",
    "OCRNL",
    "Back",
    "Save & Exit",
};
char *C_choices[] = {
    "CIGNORE",
    "CSIZE",
    "CSTOPB",
    "CREAD",
    "PARENB",
    "Back",
    "Save & Exit",
};

void update_terminal_settings(struct termios new_settings)
{
    tcsetattr(fileno(stdin), TCSANOW, &new_settings);
}
MENU *create_menu(char **choices, int n_choices)
{
    ITEM **my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

    for (int i = 0; i < n_choices; ++i)
        my_items[i] = new_item(choices[i], "");
    MENU *my_menu = new_menu((ITEM **)my_items);
    return my_menu;
}

struct termios old_settings;

int main()
{
    struct termios new_settings;
    tcgetattr(0, &old_settings);
    new_settings = old_settings;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);

    // 메뉴 구성 및 생성
    int n_choices, i_choices, o_choices, l_choices, c_choices;
    n_choices = sizeof(choices) / sizeof(char *);
    i_choices = sizeof(I_choices) / sizeof(char *);
    o_choices = sizeof(O_choices) / sizeof(char *);
    c_choices = sizeof(C_choices) / sizeof(char *);

    MENU *my_menu = create_menu(choices, n_choices);
    MENU *i_menu = create_menu(I_choices, i_choices);
    MENU *o_menu = create_menu(O_choices, o_choices);
    MENU *c_menu = create_menu(C_choices, c_choices);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    WINDOW *menu_win = newwin(max_y - 4, max_x - 4, 1, 1);
    WINDOW *sub_win = derwin(menu_win, 8, 20, max_y / 2 - 6, max_x / 2 - 9);
    // 메뉴 구성
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
    // Iflag
    WINDOW *iflags_win = newwin(max_y - 4, max_x - 4, 1, 1);
    WINDOW *isub_win = derwin(iflags_win, 8, 20, 5, 13);
    keypad(iflags_win, TRUE);
    set_menu_win(i_menu, iflags_win);
    set_menu_sub(i_menu, isub_win);

    wbkgd(stdscr, COLOR_PAIR(1));
    wbkgd(iflags_win, COLOR_PAIR(1));
    set_menu_fore(i_menu, COLOR_PAIR(2));
    set_menu_back(i_menu, COLOR_PAIR(1));
    box(iflags_win, 0, 0);

    mvwprintw(iflags_win, 0, max_x / 2 - 10, "TERMINAL SETTING");
    mvwprintw(iflags_win, max_y - 5, max_x - 15, "F1 to Exit");
    // Oflag
    WINDOW *oflags_win = newwin(max_y - 4, max_x - 4, 1, 1);
    WINDOW *osub_win = derwin(oflags_win, 8, 20, 5, 13);
    keypad(oflags_win, TRUE);
    set_menu_win(o_menu, oflags_win);
    set_menu_sub(o_menu, osub_win);

    wbkgd(stdscr, COLOR_PAIR(1));
    wbkgd(oflags_win, COLOR_PAIR(1));
    set_menu_fore(o_menu, COLOR_PAIR(2));
    set_menu_back(o_menu, COLOR_PAIR(1));
    box(oflags_win, 0, 0);

    mvwprintw(oflags_win, 0, max_x / 2 - 10, "TERMINAL SETTING");
    mvwprintw(oflags_win, max_y - 5, max_x - 15, "F1 to Exit");
    // Cflag
    WINDOW *cflags_win = newwin(max_y - 4, max_x - 4, 1, 1);
    WINDOW *csub_win = derwin(cflags_win, 8, 20, 5, 13);
    keypad(cflags_win, TRUE);
    set_menu_win(c_menu, cflags_win);
    set_menu_sub(c_menu, csub_win);

    wbkgd(stdscr, COLOR_PAIR(1));
    wbkgd(cflags_win, COLOR_PAIR(1));
    set_menu_fore(c_menu, COLOR_PAIR(2));
    set_menu_back(c_menu, COLOR_PAIR(1));
    box(cflags_win, 0, 0);

    mvwprintw(iflags_win, 0, max_x / 2 - 10, "TERMINAL SETTING");
    mvwprintw(iflags_win, 2, max_x / 2 - 8, "Input  Flags!");
    mvwprintw(iflags_win, 5, max_x - 18, (new_settings.c_iflag & IGNBRK) ? "ON" : "OFF");
    mvwprintw(iflags_win, 6, max_x - 18, (new_settings.c_iflag & BRKINT) ? "ON" : "OFF");
    mvwprintw(iflags_win, 7, max_x - 18, (new_settings.c_iflag & IGNPAR) ? "ON" : "OFF");
    mvwprintw(iflags_win, 8, max_x - 18, (new_settings.c_iflag & PARMRK) ? "ON" : "OFF");
    mvwprintw(iflags_win, 9, max_x - 18, (new_settings.c_iflag & INPCK) ? "ON" : "OFF");
    mvwprintw(iflags_win, max_y - 5, max_x - 15, "F1 to Exit");

    mvwprintw(oflags_win, 0, max_x / 2 - 10, "TERMINAL SETTING");
    mvwprintw(oflags_win, 2, max_x / 2 - 8, "Output  Flags!");
    mvwprintw(oflags_win, 5, max_x - 18, (new_settings.c_iflag & OPOST) ? "ON" : "OFF");
    mvwprintw(oflags_win, 6, max_x - 18, (new_settings.c_iflag & ONLCR) ? "ON" : "OFF");
    mvwprintw(oflags_win, 7, max_x - 18, (new_settings.c_iflag & OXTABS) ? "ON" : "OFF");
    mvwprintw(oflags_win, 8, max_x - 18, (new_settings.c_iflag & ONOEOT) ? "ON" : "OFF");
    mvwprintw(oflags_win, 9, max_x - 18, (new_settings.c_iflag & OCRNL) ? "ON" : "OFF");
    mvwprintw(oflags_win, max_y - 5, max_x - 15, "F1 to Exit");

    mvwprintw(cflags_win, 0, max_x / 2 - 10, "TERMINAL SETTING");
    mvwprintw(cflags_win, 2, max_x / 2 - 8, "Control  Flags!");
    mvwprintw(cflags_win, 5, max_x - 18, (new_settings.c_iflag & CIGNORE) ? "ON" : "OFF");
    mvwprintw(cflags_win, 6, max_x - 18, (new_settings.c_iflag & CSIZE) ? "ON" : "OFF");
    mvwprintw(cflags_win, 7, max_x - 18, (new_settings.c_iflag & CSTOPB) ? "ON" : "OFF");
    mvwprintw(cflags_win, 8, max_x - 18, (new_settings.c_iflag & CREAD) ? "ON" : "OFF");
    mvwprintw(cflags_win, 9, max_x - 18, (new_settings.c_iflag & PARENB) ? "ON" : "OFF");
    mvwprintw(cflags_win, max_y - 5, max_x - 15, "F1 to Exit");

    // 사용자 입력 처리
    int c;
    int m = 0, i = 0, o = 0, l = 0;
    ITEM *cur_item;
    while (c != KEY_F(1))
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
                else if (strcmp(item_name(cur_item), "C_Oflags") == 0)
                {
                    m = 0, i = 2;
                    break;
                }
                else if (strcmp(item_name(cur_item), "C_Cflags") == 0)
                {
                    m = 0, i = 3;
                    break;
                }
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
                if (strcmp(item_name(cur_item), "IGNBRK") == 0 && (new_settings.c_iflag & IGNBRK))
                {
                    mvwprintw(iflags_win, 5, max_x - 18, "OFF");
                    new_settings.c_iflag &= ~IGNBRK;
                }
                else if (strcmp(item_name(cur_item), "IGNBRK") == 0 && !(new_settings.c_iflag & IGNBRK))
                {
                    mvwprintw(iflags_win, 5, max_x - 18, "ON ");
                    new_settings.c_iflag |= IGNBRK;
                }
                if (strcmp(item_name(cur_item), "BRKINT") == 0 && (new_settings.c_iflag & BRKINT))
                {
                    mvwprintw(iflags_win, 6, max_x - 18, "OFF");
                    new_settings.c_iflag &= ~BRKINT;
                }
                else if (strcmp(item_name(cur_item), "BRKINT") == 0 && !(new_settings.c_iflag & BRKINT))
                {
                    mvwprintw(iflags_win, 6, max_x - 18, "ON ");
                    new_settings.c_iflag |= BRKINT;
                }
                if (strcmp(item_name(cur_item), "IGNPAR") == 0 && (new_settings.c_iflag & IGNPAR))
                {
                    mvwprintw(iflags_win, 7, max_x - 18, "OFF");
                    new_settings.c_iflag &= ~IGNPAR;
                }
                else if (strcmp(item_name(cur_item), "IGNPAR") == 0 && !(new_settings.c_iflag & IGNPAR))
                {
                    mvwprintw(iflags_win, 7, max_x - 18, "ON ");
                    new_settings.c_iflag |= IGNPAR;
                }
                if (strcmp(item_name(cur_item), "PARMRK") == 0 && (new_settings.c_iflag & PARMRK))
                {
                    mvwprintw(iflags_win, 8, max_x - 18, "OFF");
                    new_settings.c_iflag &= ~PARMRK;
                }
                else if (strcmp(item_name(cur_item), "PARMRK") == 0 && !(new_settings.c_iflag & PARMRK))
                {
                    mvwprintw(iflags_win, 8, max_x - 18, "ON ");
                    new_settings.c_iflag |= PARMRK;
                }
                if (strcmp(item_name(cur_item), "INPCK") == 0 && (new_settings.c_iflag & INPCK))
                {
                    mvwprintw(iflags_win, 9, max_x - 18, "OFF");
                    new_settings.c_iflag &= ~INPCK;
                }
                else if (strcmp(item_name(cur_item), "INPCK") == 0 && !(new_settings.c_iflag & INPCK))
                {
                    mvwprintw(iflags_win, 9, max_x - 18, "ON ");
                    new_settings.c_iflag |= INPCK;
                }
                if (strcmp(item_name(cur_item), "Back") == 0)
                {
                    m = 0, i = 0;
                    mvwprintw(menu_win, 16, max_x, "m = %d, i = %d", m, i);
                    break;
                }
            }
        }
        else if (m == 0 && i == 2)
        {
            post_menu(o_menu);
            touchwin(oflags_win);
            wrefresh(oflags_win);
            c = wgetch(oflags_win);

            switch (c)
            {
            case KEY_DOWN:
                menu_driver(o_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(o_menu, REQ_UP_ITEM);
                break;
            case 10:
                cur_item = current_item(o_menu);
                if (strcmp(item_name(cur_item), "OPOST") == 0 && (new_settings.c_oflag & OPOST))
                {
                    mvwprintw(oflags_win, 5, max_x - 18, "OFF");
                    new_settings.c_oflag &= ~OPOST;
                }
                else if (strcmp(item_name(cur_item), "OPOST") == 0 && !(new_settings.c_oflag & OPOST))
                {
                    mvwprintw(oflags_win, 5, max_x - 18, "ON ");
                    new_settings.c_oflag |= OPOST;
                }
                if (strcmp(item_name(cur_item), "ONLCR") == 0 && (new_settings.c_oflag & ONLCR))
                {
                    mvwprintw(oflags_win, 6, max_x - 18, "OFF");
                    new_settings.c_oflag &= ~ONLCR;
                }
                else if (strcmp(item_name(cur_item), "ONLCR") == 0 && !(new_settings.c_oflag & ONLCR))
                {
                    mvwprintw(oflags_win, 6, max_x - 18, "ON ");
                    new_settings.c_oflag |= ONLCR;
                }
                if (strcmp(item_name(cur_item), "OXTABS") == 0 && (new_settings.c_oflag & OXTABS))
                {
                    mvwprintw(oflags_win, 7, max_x - 18, "OFF");
                    new_settings.c_oflag &= ~OXTABS;
                }
                else if (strcmp(item_name(cur_item), "OXTABS") == 0 && !(new_settings.c_oflag & OXTABS))
                {
                    mvwprintw(oflags_win, 7, max_x - 18, "ON ");
                    new_settings.c_oflag |= OXTABS;
                }
                if (strcmp(item_name(cur_item), "ONOEOT") == 0 && (new_settings.c_oflag & ONOEOT))
                {
                    mvwprintw(oflags_win, 8, max_x - 18, "OFF");
                    new_settings.c_oflag &= ~ONOEOT;
                }
                else if (strcmp(item_name(cur_item), "ONOEOT") == 0 && !(new_settings.c_oflag & ONOEOT))
                {
                    mvwprintw(oflags_win, 8, max_x - 18, "ON ");
                    new_settings.c_oflag |= ONOEOT;
                }
                if (strcmp(item_name(cur_item), "OCRNL") == 0 && (new_settings.c_oflag & OCRNL))
                {
                    mvwprintw(oflags_win, 9, max_x - 18, "OFF");
                    new_settings.c_oflag &= ~OCRNL;
                }
                else if (strcmp(item_name(cur_item), "OCRNL") == 0 && !(new_settings.c_oflag & OCRNL))
                {
                    mvwprintw(oflags_win, 9, max_x - 18, "ON ");
                    new_settings.c_oflag |= OCRNL;
                }
                if (strcmp(item_name(cur_item), "Back") == 0)
                {
                    m = 0, i = 0;
                    mvwprintw(menu_win, 16, max_x, "m = %d, i = %d", m, i);
                    break;
                }
            }
        }
        else if (m == 0 && i == 3)
        {
            post_menu(c_menu);
            touchwin(cflags_win);
            wrefresh(cflags_win);
            c = wgetch(cflags_win);
            switch (c)
            {
            case KEY_DOWN:
                menu_driver(c_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(c_menu, REQ_UP_ITEM);
                break;
            case 10:
                cur_item = current_item(c_menu);
                if (strcmp(item_name(cur_item), "CIGNORE") == 0 && (new_settings.c_cflag & CIGNORE))
                {
                    mvwprintw(cflags_win, 5, max_x - 18, "OFF");
                    new_settings.c_cflag &= ~CIGNORE;
                }
                else if (strcmp(item_name(cur_item), "CIGNORE") == 0 && !(new_settings.c_cflag & CIGNORE))
                {
                    mvwprintw(cflags_win, 5, max_x - 18, "ON ");
                    new_settings.c_cflag |= CIGNORE;
                }
                if (strcmp(item_name(cur_item), "CSIZE") == 0 && (new_settings.c_cflag & CSIZE))
                {
                    mvwprintw(cflags_win, 6, max_x - 18, "OFF");
                    new_settings.c_cflag &= ~CSIZE;
                }
                else if (strcmp(item_name(cur_item), "CSIZE") == 0 && !(new_settings.c_cflag & CSIZE))
                {
                    mvwprintw(cflags_win, 6, max_x - 18, "ON ");
                    new_settings.c_cflag |= CSIZE;
                }
                if (strcmp(item_name(cur_item), "CSTOPB") == 0 && (new_settings.c_cflag & CSTOPB))
                {
                    mvwprintw(cflags_win, 7, max_x - 18, "OFF");
                    new_settings.c_cflag &= ~CSTOPB;
                }
                else if (strcmp(item_name(cur_item), "CSTOPB") == 0 && !(new_settings.c_cflag & CSTOPB))
                {
                    mvwprintw(cflags_win, 7, max_x - 18, "ON ");
                    new_settings.c_cflag |= CSTOPB;
                }
                if (strcmp(item_name(cur_item), "CREAD") == 0 && (new_settings.c_cflag & CREAD))
                {
                    mvwprintw(cflags_win, 8, max_x - 18, "OFF");
                    new_settings.c_cflag &= ~CREAD;
                }
                else if (strcmp(item_name(cur_item), "CREAD") == 0 && !(new_settings.c_cflag & CREAD))
                {
                    mvwprintw(cflags_win, 8, max_x - 18, "ON ");
                    new_settings.c_cflag |= CREAD;
                }
                if (strcmp(item_name(cur_item), "PARENB") == 0 && (new_settings.c_cflag & PARENB))
                {
                    mvwprintw(cflags_win, 9, max_x - 18, "OFF");
                    new_settings.c_cflag &= ~PARENB;
                }
                else if (strcmp(item_name(cur_item), "PARENB") == 0 && !(new_settings.c_cflag & PARENB))
                {
                    mvwprintw(cflags_win, 9, max_x - 18, "ON ");
                    new_settings.c_cflag |= PARENB;
                }
                if (strcmp(item_name(cur_item), "Back") == 0)
                {
                    m = 0, i = 0;
                    mvwprintw(menu_win, 16, max_x, "m = %d, i = %d", m, i);
                    break;
                }
            }
        }
    }
    // 초기화
    unpost_menu(my_menu);
    free_menu(my_menu);
    endwin();
}
