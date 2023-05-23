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
    // 메뉴 출력
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
    mvwprintw(iflags_win, 2, max_x / 2 - 8, "Input  Flags");
    mvwprintw(iflags_win, 5, max_x - 18, "ON");
    mvwprintw(iflags_win, 6, max_x - 18, "ON");
    mvwprintw(iflags_win, 7, max_x - 18, "ON");
    mvwprintw(iflags_win, 8, max_x - 18, "ON");
    mvwprintw(iflags_win, 9, max_x - 18, "ON");
    mvwprintw(iflags_win, max_y - 5, max_x - 15, "F1 to Exit");

    // 사용자 입력 처리
    int c;
    int m = 1, i = 0, o = 0, l = 0;
    ITEM *cur_item;
    while (c != KEY_F(1))
    {
        if (m == 1)
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
                if (strcmp(item_name(cur_item), "Input  Flags") == 0)
                {
                    m = 0, i = 1;
                    break;
                }
            }
        }
        if (m == 0 && i == 1)
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
                if (strcmp(item_name(cur_item), "Back") == 0)
                {
                    m = 1, i = 0;
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
