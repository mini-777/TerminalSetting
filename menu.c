#include <stdlib.h>
#include <curses.h>
#include <menu.h>

char *choices[] = {
    "Input  Flags",
    "Output Flags",
    "Local  Flags",
    "Save & Exit",
};

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);

    int n_choices = sizeof(choices) / sizeof(char *);
    ITEM **my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for (int i = 0; i < n_choices; ++i)
        my_items[i] = new_item(choices[i], "");

//메뉴 구성 및 생성
    MENU *my_menu = new_menu((ITEM **)my_items);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); 
    WINDOW *menu_win = newwin(max_y-2, max_x-2, 1, 1); 
    WINDOW *sub_win = derwin(menu_win, 8, 20, max_y/2-6, max_x/2-9); 
    
    keypad(menu_win, TRUE);
    set_menu_win(my_menu, menu_win);
    set_menu_sub(my_menu, sub_win);
//메뉴 출력   
    wbkgd(stdscr, COLOR_PAIR(1));
    wbkgd(menu_win, COLOR_PAIR(1));
    set_menu_fore(my_menu, COLOR_PAIR(2));
    set_menu_back(my_menu, COLOR_PAIR(1));
    box(menu_win, 0, 0);
    mvwprintw(menu_win, 2, max_x/2 - 8, "TERMINAL SETTING");
    mvwprintw(menu_win, max_y-4, max_x - 14, "F1 to Exit");
    post_menu(my_menu);
    wrefresh(menu_win);
 
//사용자 입력 처리    
    int c;
    ITEM *cur_item;
    while ((c = wgetch(menu_win)) != KEY_F(1))
        switch (c)
        {
        case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
        case 10: // Enter로 선택
            cur_item = current_item(my_menu);
            mvwprintw(menu_win, max_y - 6, 4, "Item selected is : %s", item_name(cur_item)); // 선택 메뉴 출력
            pos_menu_cursor(my_menu);
            break;    
        }
//초기화
    unpost_menu(my_menu);
    free_menu(my_menu);
    for (int i = 0; i < n_choices; ++i)
        free_item(my_items[i]);
    endwin();
}
