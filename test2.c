#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <menu.h>
#include <termios.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

void update_terminal_settings(struct termios new_settings)
{
    tcsetattr(fileno(stdin), TCSANOW, &new_settings);
}

void set_canonical_mode()
{
    struct termios settings;
    tcgetattr(fileno(stdin), &settings);
    settings.c_lflag |= ICANON;
    update_terminal_settings(settings);
}

void set_noncanonical_mode()
{
    struct termios settings;
    tcgetattr(fileno(stdin), &settings);
    settings.c_lflag &= ~ICANON;
    update_terminal_settings(settings);
}

void toggle_echo()
{
    struct termios settings;
    tcgetattr(fileno(stdin), &settings);
    if (settings.c_lflag & ECHO)
    {
        settings.c_lflag &= ~ECHO;
    }
    else
    {
        settings.c_lflag |= ECHO;
    }
    update_terminal_settings(settings);
}

int main()
{
    ITEM *my_items[4];
    MENU *my_menu;
    WINDOW *my_menu_win;
    int i;
    int c;
    struct termios original_settings;

    /* Save original terminal settings */
    tcgetattr(fileno(stdin), &original_settings);

    /* Initialize curses */
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    /* Create menu items */
    my_items[0] = new_item("Toggle Echo", "");
    my_items[1] = new_item("Canonical Mode", "");
    my_items[2] = new_item("Non-canonical Mode", "");
    my_items[3] = NULL; // Add NULL item to mark the end

    /* Create menu */
    my_menu = new_menu((ITEM **)my_items);

    /* Create window for the menu */
    my_menu_win = newwin(10, 40, 4, 4);
    keypad(my_menu_win, TRUE);

    /* Set main window and sub window */
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

    /* Print a border around the main window */
    box(my_menu_win, 0, 0);

    /* Post the menu */
    post_menu(my_menu);
    wrefresh(my_menu_win);

    printw("Use arrow keys to navigate. Press <ENTER> to select a choice...");
    refresh();

    /* Loop to capture user input */
    while ((c = wgetch(my_menu_win)) != KEY_F(1))
    {
        switch (c)
        {
        case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
        case 10: /* Enter key */
        {
            ITEM *cur_item = current_item(my_menu);
            const char *item_name = item_name(cur_item);

            if (strcmp(item_name, "Toggle Echo") == 0)
            {
                toggle_echo();
            }
            else if (strcmp(item_name, "Canonical Mode") == 0)
            {
                set_canonical_mode();
            }
            else if (strcmp(item_name, "Non-canonical Mode") == 0)
            {
                set_noncanonical_mode();
            }

            pos_menu_cursor(my_menu);
            break;
        }
        }

        wrefresh(my_menu_win);
    }

    /* Free menu items */
    for (i = 0; i < ARRAY_SIZE(my_items) - 1; ++i)
    {
        free_item(my_items[i]);
    }

    /* Free menu */
    free_menu(my_menu);

    /* Restore original terminal settings */
    update_terminal_settings(original_settings);

    /* End curses mode */
    endwin();

    return 0;
}
