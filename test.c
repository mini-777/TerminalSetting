#include <stdlib.h>
#include <curses.h>
#include <menu.h>
#include <string.h>

char *choices[] = {
    "Input  Flags",
    "Output Flags",
    "Local  Flags",
    "Save & Exit",
};

char *Ichoices[] = {
    "Flags 1",
    "Flags 2",
    "Flags 3",
    "Flags 4",
    "Flags 5",
    "Exit",
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

//메뉴 구성 및 생성
    int n_choices = sizeof(choices) / sizeof(char *);
    ITEM **my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for (int i = 0; i < n_choices; ++i)
        my_items[i] = new_item(choices[i], "");
        
    MENU *my_menu = new_menu((ITEM **)my_items);
    
//Input Flags 메뉴
    int i_choices = sizeof(Ichoices) / sizeof(char *);
    ITEM **i_items = (ITEM **)calloc(i_choices + 1, sizeof(ITEM *));
    for (int i = 0; i < i_choices; ++i)
        i_items[i] = new_item(Ichoices[i], "");
        
    MENU *i_menu = new_menu((ITEM **)i_items);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); 
    WINDOW *menu_win = newwin(max_y-2, max_x-2, 1, 1); 
    WINDOW *sub_win = derwin(menu_win, 8, 20, max_y/2-6, max_x/2-9); 
 //메뉴 출력    
    keypad(menu_win, TRUE);
    set_menu_win(my_menu, menu_win);
    set_menu_sub(my_menu, sub_win);
  
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
            if (strcmp(item_name(cur_item), "Input  Flags") == 0) {
            	
            	WINDOW *input_flags_win = newwin(max_y-2, max_x-2, 1, 1);
            	WINDOW *isub_win = derwin(input_flags_win, 8, 20, 5, 13); 
    
            	keypad(input_flags_win, TRUE);
            	set_menu_win(i_menu, input_flags_win);
            	set_menu_sub(i_menu, isub_win);
            	
            	wbkgd(stdscr, COLOR_PAIR(1));
            	wbkgd(input_flags_win, COLOR_PAIR(1));
		set_menu_fore(i_menu, COLOR_PAIR(2));   
		set_menu_back(i_menu, COLOR_PAIR(1));
            	box(input_flags_win,0,0);
            	mvwprintw(input_flags_win, 2, max_x/2 - 8, "Input  Flags");
    		mvwprintw(input_flags_win, 5, max_x - 18, "ON");
            	mvwprintw(input_flags_win, 6, max_x - 18, "ON");
            	mvwprintw(input_flags_win, 7, max_x - 18, "ON");
            	mvwprintw(input_flags_win, 8, max_x - 18, "ON");
            	mvwprintw(input_flags_win, 9, max_x - 18, "ON");
            	mvwprintw(input_flags_win, 10, max_x - 18, "ON");
            	mvwprintw(input_flags_win, 11, max_x - 18, "ON");
            	
            	post_menu(i_menu);
            	wrefresh(input_flags_win);
		while ((c = wgetch(input_flags_win)) != -1)
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
        if (strcmp(item_name(cur_item), "Exit") == 0) {
        	unpost_menu(i_menu);
        	wrefresh(input_flags_win);
        	delwin(input_flags_win);
        	touchwin(menu_win);
        	wrefresh(menu_win);
        	d = -1;
        }
              
        }
        }
               
    }
//초기화
    unpost_menu(my_menu);
    free_menu(my_menu);
    for (int i = 0; i < n_choices; ++i)
        free_item(my_items[i]);
    endwin();
}
  

