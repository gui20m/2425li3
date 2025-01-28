#include "Interactive/querysolver.h"
#include "Interactive/home.h"
#include "Main/queries.h"
#include "Main/utils.h"
#include <unistd.h>

typedef struct buttons {
    char* label;
    int x, y;
} BUTTONS;

static BUTTONS create_button(const char* label, int x, int y) {
    BUTTONS button;
    button.label = strdup(label);
    button.x = x;
    button.y = y;
    return button;
}

static void free_button(BUTTONS* button) {
    if (button && button->label) {
        free(button->label);
        button->label = NULL;
    }
}

void process_query1(SETTINGS setts) {
    int win_height = 17, win_width = 61;
    int win_start_y = 4, win_start_x = 12;
    WINDOW* query_win = newwin(win_height, win_width, win_start_y, win_start_x);
    box(query_win, 0, 0);
    mvwprintw(query_win, 1, 1, "------------------Query1 Processing Window-----------------");
    mvwprintw(query_win, 2, 19, "QUERY ID: 1; arg1: <ID>");
    mvwprintw(query_win, 4, 1, "Output info: email;first_name;last_name;age;country [user]\n              name;type;country;total_recipe [artist]");
    mvwprintw(query_win, 6, 1, "--------------------------Options--------------------------");
    mvwprintw(query_win, 7, 4, "1");
    mvwprintw(query_win, 10, 1, "--------------------------Results--------------------------");
    
    const int max_options = 3;

    BUTTONS optionssettings[] = {
        create_button(".", 7, 7),
        create_button("Search", 4, 9),
        create_button("[BACK]", 4, 15),
    };
    
    int selected = 0;
    keypad(query_win, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE); 
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);  
    init_pair(4, COLOR_WHITE, COLOR_BLUE);

    wrefresh(query_win);

    int ch;
    while (1) {
        for (int i = 0; i < max_options; i++) {
            if (i == selected) {
                wattron(query_win, COLOR_PAIR(1));
            } else {
                wattron(query_win, COLOR_PAIR(2));
            }

            mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);

            if (i == selected && i == 2) {
                wattron(query_win, COLOR_PAIR(4));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);
                wattroff(query_win, COLOR_PAIR(4));
                wattron(query_win, COLOR_PAIR(3));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x + 1, "B");
                wattroff(query_win, COLOR_PAIR(3));
            }

            if (i == selected) {
                wattroff(query_win, COLOR_PAIR(1));
            } else {
                wattroff(query_win, COLOR_PAIR(2));
            }
        }

        wrefresh(query_win);

        ch = wgetch(query_win);
        switch (ch) {
            case KEY_UP:
                selected = (selected - 1 + max_options) % max_options;
                break;

            case KEY_DOWN:
                selected = (selected + 1) % max_options;
                break;

            case '\n':
                if (selected == 0) {
                    move(0, 0);
                    mvprintw(0, 0, "id: ");
                    wrefresh(query_win);
                    char id[50];
                    echo();
                    clrtoeol();
                    curs_set(1);
                    getstr(id);
                    curs_set(0);

                    optionssettings[0].label = strdup(id);
                    char **args = malloc(sizeof(char*) * 2);
                    args[0] = strdup(id);
                    args[1] = NULL;
                    set_argSETTINGS(setts, args, 1);

                    clrtoeol();
                    wrefresh(query_win);

                } else if (selected == 1) {
                    optionssettings[0].label = strdup("........");
                    char ** args = get_args(setts);
                    if(!args) break;
                    SPOTILIXO data = get_manager(setts);
                    if (!data) break;
                    char* output = queryOne(data,args,1,";");
                    mvwprintw(query_win, 12, 1, "%s", output);
                    wrefresh(query_win);
                } else if (selected == 2) {
                    clear();
                    refresh();
                    return;
                }
                break;

            case 27:
                delwin(query_win);
                clear();
                refresh();
                for (int i = 0; i < max_options; i++) {
                    free_button(&optionssettings[i]);
                }
                return;
        }
    }

    delwin(query_win);
    endwin();
}

static void display_page(WINDOW* query_win, char* output) {
    int lines_per_page = 10;
    int start_line = 0;
    int total_lines = 0;
    int selected_button = 0;

    char* temp_output = strdup(output);
    char* token = strtok(temp_output, "\n");
    while (token != NULL) {
        total_lines++;
        token = strtok(NULL, "\n");
    }
    free(temp_output);

    BUTTONS buttons[] = {
        create_button("Anterior", 20, 14),
        create_button("Proximo", 30, 14),
        create_button("Voltar", 50, 14)
    };
    int button_count = sizeof(buttons) / sizeof(buttons[0]);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    while (1) {
        werase(query_win);
        box(query_win, 0, 0);
        mvwprintw(query_win, 1, 1, "--------------------------Results--------------------------");

        int current_line = 0;
        int x = 1, y = 3;

        char* temp_output_render = strdup(output);
        token = strtok(temp_output_render, "\n");

        for (int i = 0; i < start_line; i++) {
            token = strtok(NULL, "\n");
        }

        while (token != NULL && current_line < lines_per_page) {
            mvwprintw(query_win, y + current_line, x, "%s", token);
            current_line++;
            token = strtok(NULL, "\n");
        }

        free(temp_output_render);

        for (int i = 0; i < button_count; i++) {
            if (i == selected_button) {
                wattron(query_win, COLOR_PAIR(1));
            } else {
                wattron(query_win, COLOR_PAIR(2));
            }
            mvwprintw(query_win, buttons[i].y, buttons[i].x, "[%s]", buttons[i].label);
            wattroff(query_win, COLOR_PAIR(1));
            wattroff(query_win, COLOR_PAIR(2));
        }

        wrefresh(query_win);

        int ch = wgetch(query_win);

        if (ch == KEY_LEFT) {
            selected_button = (selected_button - 1 + button_count) % button_count;
        } else if (ch == KEY_RIGHT) {
            selected_button = (selected_button + 1) % button_count;
        } else if (ch == '\n') {
            if (selected_button == 0 && start_line - lines_per_page >= 0) {
                start_line -= lines_per_page;
            } else if (selected_button == 1 && start_line + lines_per_page < total_lines) {
                start_line += lines_per_page;
            } else if (selected_button == 2) {
                break;
            }
        }
    }

    for (int i = 0; i < button_count; i++) {
        free(buttons[i].label);
    }

    endwin();
    clear();
    refresh();
    werase(query_win);
}



void process_query2(SETTINGS setts) {
    int win_height = 17, win_width = 61;
    int win_start_y = 4, win_start_x = 12;
    WINDOW* query_win = newwin(win_height, win_width, win_start_y, win_start_x);
    box(query_win, 0, 0);
    
    const int max_options = 4;

    BUTTONS optionssettings[] = {
        create_button(".", 7, 7),
        create_button("Search", 4, 9),
        create_button("[BACK]", 4, 15),
        create_button(".", 10, 7),
    };
    
    int selected = 0;
    keypad(query_win, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE); 
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);  
    init_pair(4, COLOR_WHITE, COLOR_BLUE);

    wrefresh(query_win);

    int ch;
    while (1) {
        box(query_win, 0, 0);
        mvwprintw(query_win, 1, 1, "------------------Query2 Processing Window-----------------");
        mvwprintw(query_win, 2, 13, "QUERY ID: 2; arg1: <N>; arg2: [country]");
        mvwprintw(query_win, 4, 1, "Output info: name 1;type 1;discography duration 1;country 1\n              name 2;type 2;discography duration 2;country 2");
        mvwprintw(query_win, 6, 1, "--------------------------Options--------------------------");
        mvwprintw(query_win, 7, 4, "2");
        mvwprintw(query_win, 10, 1, "--------------------------Results--------------------------");
        for (int i = 0; i < max_options; i++) {
            if (i == selected) {
                wattron(query_win, COLOR_PAIR(1));
            } else {
                wattron(query_win, COLOR_PAIR(2));
            }

            mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);

            if (i == selected && i == 2) {
                wattron(query_win, COLOR_PAIR(4));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);
                wattroff(query_win, COLOR_PAIR(4));
                wattron(query_win, COLOR_PAIR(3));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x + 1, "B");
                wattroff(query_win, COLOR_PAIR(3));
            }

            if (i == selected) {
                wattroff(query_win, COLOR_PAIR(1));
            } else {
                wattroff(query_win, COLOR_PAIR(2));
            }
        }

        wrefresh(query_win);

        ch = wgetch(query_win);
        switch (ch) {
            case KEY_UP:
                selected = (selected - 1 + max_options) % max_options;
                break;

            case KEY_DOWN:
                if (selected == 0) {selected = 1; break;}
                if (selected == 3) {selected = 1; break;}
                if (selected == 1) selected = 2;
                break;
            case KEY_RIGHT:
                if (selected==0) {selected = 3; break;}
                break;
            case KEY_LEFT:
                if (selected==3) {selected = 0; break;}
                break;
            case '\n':
                if (selected == 0) {
                    char number[50];
                    while(1){
                        move(0, 0);
                        clrtoeol();
                        mvprintw(0, 0, "number of artists: ");
                        wrefresh(query_win);
                        echo();
                        clrtoeol();
                        curs_set(1);
                        getstr(number);
                        curs_set(0);
                        if (is_number(number)) break;
                    }
                    char ** args = get_args(setts);
                    if (!args){
                        args = malloc(sizeof(char*)*3);
                        args[0] = NULL;
                        args[1] = NULL;
                        args[2] = NULL;
                    }
                    args[0] = strdup(number);

                    set_argSETTINGS(setts, args, 1);

                    optionssettings[0].label = strdup(number);
                    wrefresh(query_win);

                    clrtoeol();
                    wrefresh(query_win);

                } else if (selected == 1) {
                    optionssettings[0].label = strdup("..");
                    optionssettings[3].label = strdup(".............");
                    char ** args = get_args(setts);
                    if(!args || !args[0]) break;
                    int n = 1;
                    if (args[1]) n++;
                    SPOTILIXO data = get_manager(setts);
                    if (!data) break;
                    char* output = queryTwo(data, args, n, ";");
                    if (atoi(args[0]) > 3) display_page(query_win, output);
                    else {
                        int y = 11, x = 1;
                        char* token = strtok(output, "\n");
                        while (token != NULL) {
                            mvwprintw(query_win, y, x, "%s", token);
                            wrefresh(query_win);
                            y++;
                            token = strtok(NULL, "\n");
                        }
                    }
                    draw_background();
                } else if (selected == 2) {
                    clear();
                    refresh();
                    return;
                } else if (selected == 3) {
                    char country[50];
                    move(0, 0);
                    clrtoeol();
                    mvprintw(0, 0, "country filter: ");
                    wrefresh(query_win);
                    echo();
                    clrtoeol();
                    curs_set(1);
                    getstr(country);
                    curs_set(0);
                    char** args = get_args(setts);
                    if (!args){
                        args = malloc(sizeof(char*)*3);
                        args[0] = NULL;
                        args[1] = NULL;
                        args[2] = NULL;
                    }
                    args[1] = country;

                    set_argSETTINGS(setts, args, 2);

                    optionssettings[3].label = strdup(country);
                    wrefresh(query_win);
                }
                break;

            case 27:
                delwin(query_win);
                clear();
                refresh();
                for (int i = 0; i < max_options; i++) {
                    free_button(&optionssettings[i]);
                }
                return;
        }
    }

    delwin(query_win);
    endwin();
}

void process_query3(SETTINGS setts) {
    int win_height = 17, win_width = 61;
    int win_start_y = 4, win_start_x = 12;
    WINDOW* query_win = newwin(win_height, win_width, win_start_y, win_start_x);
    box(query_win, 0, 0);
    
    const int max_options = 4;

    BUTTONS optionssettings[] = {
        create_button(".", 7, 7),
        create_button("Search", 4, 9),
        create_button("[BACK]", 4, 15),
        create_button(".", 10, 7),
    };
    
    int selected = 0;
    keypad(query_win, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE); 
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);  
    init_pair(4, COLOR_WHITE, COLOR_BLUE);

    wrefresh(query_win);

    int ch;
    while (1) {
        box(query_win, 0, 0);
        mvwprintw(query_win, 1, 1, "------------------Query3 Processing Window-----------------");
        mvwprintw(query_win, 2, 8, "QUERY ID: 3; arg1: <min_age>; arg2: <max_age>");
        mvwprintw(query_win, 4, 1, "Output info: genre 1;total likes\n              genre 2;total likes");
        mvwprintw(query_win, 6, 1, "--------------------------Options--------------------------");
        mvwprintw(query_win, 7, 4, "3");
        mvwprintw(query_win, 10, 1, "--------------------------Results--------------------------");
        for (int i = 0; i < max_options; i++) {
            if (i == selected) {
                wattron(query_win, COLOR_PAIR(1));
            } else {
                wattron(query_win, COLOR_PAIR(2));
            }

            mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);

            if (i == selected && i == 2) {
                wattron(query_win, COLOR_PAIR(4));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);
                wattroff(query_win, COLOR_PAIR(4));
                wattron(query_win, COLOR_PAIR(3));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x + 1, "B");
                wattroff(query_win, COLOR_PAIR(3));
            }

            if (i == selected) {
                wattroff(query_win, COLOR_PAIR(1));
            } else {
                wattroff(query_win, COLOR_PAIR(2));
            }
        }

        wrefresh(query_win);

        ch = wgetch(query_win);
        switch (ch) {
            case KEY_UP:
                selected = (selected - 1 + max_options) % max_options;
                break;

            case KEY_DOWN:
                if (selected == 0) {selected = 1; break;}
                if (selected == 3) {selected = 1; break;}
                if (selected == 1) selected = 2;
                break;
            case KEY_RIGHT:
                if (selected==0) {selected = 3; break;}
                break;
            case KEY_LEFT:
                if (selected==3) {selected = 0; break;}
                break;

            case '\n':
                if (selected == 0) {
                    char min_age[50];
                    while(1){
                        move(0, 0);
                        clrtoeol();
                        mvprintw(0, 0, "min age: ");
                        wrefresh(query_win);
                        echo();
                        clrtoeol();
                        curs_set(1);
                        getstr(min_age);
                        curs_set(0);
                        if (is_number(min_age)) break;
                    }
                    char ** args = get_args(setts);
                    if (!args){
                        args = malloc(sizeof(char*)*3);
                        args[0] = NULL;
                        args[1] = NULL;
                        args[2] = NULL;
                    }
                    args[0] = strdup(min_age);

                    set_argSETTINGS(setts, args, 1);

                    optionssettings[0].label = strdup(min_age);
                    wrefresh(query_win);

                    clrtoeol();
                    wrefresh(query_win);

                } else if (selected == 1) {
                    optionssettings[0].label = strdup("...");
                    optionssettings[3].label = strdup("...");
                    char ** args = get_args(setts);
                    if(!args || !args[0] || !args[1]) break;
                    SPOTILIXO data = get_manager(setts);
                    if (!data) break;
                    char* output = queryThree(data, args, 2, ";");
                    if (atoi(args[0]) > 3) display_page(query_win, output);
                    else {
                        int y = 11, x = 1;
                        char* token = strtok(output, "\n");
                        while (token != NULL) {
                            mvwprintw(query_win, y, x, "%s", token);
                            wrefresh(query_win);
                            y++;
                            token = strtok(NULL, "\n");
                        }
                    }
                    draw_background();
                } else if (selected == 2) {
                    clear();
                    refresh();
                    return;
                } else if (selected == 3) {
                    char max_age[50];
                    while(1){
                        move(0, 0);
                        clrtoeol();
                        mvprintw(0, 0, "max age: ");
                        wrefresh(query_win);
                        echo();
                        clrtoeol();
                        curs_set(1);
                        getstr(max_age);
                        if(is_number(max_age)) break;
                    }
                    curs_set(0);
                    char** args = get_args(setts);
                    if (!args){
                        args = malloc(sizeof(char*)*3);
                        args[0] = NULL;
                        args[1] = NULL;
                        args[2] = NULL;
                    }
                    args[1] = max_age;

                    set_argSETTINGS(setts, args, 2);

                    optionssettings[3].x += 1;
                    optionssettings[3].label = strdup(max_age);
                    wrefresh(query_win);
                }
                break;

            case 27:
                delwin(query_win);
                clear();
                refresh();
                for (int i = 0; i < max_options; i++) {
                    free_button(&optionssettings[i]);
                }
                return;
        }
    }

    delwin(query_win);
    endwin();
}

void process_query4(SETTINGS setts) {
    int win_height = 17, win_width = 61;
    int win_start_y = 4, win_start_x = 12;
    WINDOW* query_win = newwin(win_height, win_width, win_start_y, win_start_x);
    box(query_win, 0, 0);
    
    const int max_options = 4;

    BUTTONS optionssettings[] = {
        create_button(".", 7, 7),
        create_button("Search", 4, 9),
        create_button("[BACK]", 4, 15),
        create_button(".", 10, 7),
    };
    
    int selected = 0;
    keypad(query_win, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE); 
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);  
    init_pair(4, COLOR_WHITE, COLOR_BLUE);

    wrefresh(query_win);

    int ch;
    while (1) {
        box(query_win, 0, 0);
        mvwprintw(query_win, 1, 1, "------------------Query4 Processing Window-----------------");
        mvwprintw(query_win, 2, 8, "QUERY ID: 4; arg1: [begin_date]; arg2: [end_date]");
        mvwprintw(query_win, 4, 1, "Output info: name;type;count_top_10");
        mvwprintw(query_win, 6, 1, "--------------------------Options--------------------------");
        mvwprintw(query_win, 7, 4, "4");
        mvwprintw(query_win, 10, 1, "--------------------------Results--------------------------");
        for (int i = 0; i < max_options; i++) {
            if (i == selected) {
                wattron(query_win, COLOR_PAIR(1));
            } else {
                wattron(query_win, COLOR_PAIR(2));
            }

            mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);

            if (i == selected && i == 2) {
                wattron(query_win, COLOR_PAIR(4));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);
                wattroff(query_win, COLOR_PAIR(4));
                wattron(query_win, COLOR_PAIR(3));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x + 1, "B");
                wattroff(query_win, COLOR_PAIR(3));
            }

            if (i == selected) {
                wattroff(query_win, COLOR_PAIR(1));
            } else {
                wattroff(query_win, COLOR_PAIR(2));
            }
        }

        wrefresh(query_win);

        ch = wgetch(query_win);
        switch (ch) {
            case KEY_UP:
                selected = (selected - 1 + max_options) % max_options;
                break;

            case KEY_DOWN:
                if (selected == 0) {selected = 1; break;}
                if (selected == 3) {selected = 1; break;}
                if (selected == 1) selected = 2;
                break;
            case KEY_RIGHT:
                if (selected==0) {selected = 3; break;}
                break;
            case KEY_LEFT:
                if (selected==3) {selected = 0; break;}
                break;

            case '\n':
                if (selected == 0) {
                    char begin_date[50];
                    move(0, 0);
                    clrtoeol();
                    mvprintw(0, 0, "begin date: ");
                    wrefresh(query_win);
                    echo();
                    clrtoeol();
                    curs_set(1);
                    getstr(begin_date);
                    curs_set(0);
                    char ** args = get_args(setts);
                    if (!args){
                        args = malloc(sizeof(char*)*3);
                        args[0] = NULL;
                        args[1] = NULL;
                        args[2] = NULL;
                    }
                    args[0] = strdup(begin_date);

                    set_argSETTINGS(setts, args, 1);

                    optionssettings[0].label = strdup(begin_date);
                    wrefresh(query_win);

                    clrtoeol();
                    wrefresh(query_win);

                } else if (selected == 1) {
                    optionssettings[0].label = strdup("..........");
                    optionssettings[0].label = strdup("..........");
                    char ** args = get_args(setts);
                    int n = get_argsN(setts);
                    if (n!=0) if ((args[1] && !args[0]) || (args[0] && !args[1])) break;
                    SPOTILIXO data = get_manager(setts);
                    if (!data) break;
                    char* output = queryFour(data, args, n, ";");
                    mvwprintw(query_win, 11,1, "%s", output);
                    wrefresh(query_win);
                } else if (selected == 2) {
                    clear();
                    refresh();
                    return;
                } else if (selected == 3) {
                    char end_date[50];
                    move(0, 0);
                    clrtoeol();
                    mvprintw(0, 0, "end date: ");
                    wrefresh(query_win);
                    echo();
                    clrtoeol();
                    curs_set(1);
                    getstr(end_date);
                    curs_set(0);
                    char** args = get_args(setts);
                    if (!args){
                        args = malloc(sizeof(char*)*3);
                        args[0] = NULL;
                        args[1] = NULL;
                        args[2] = NULL;
                    }
                    args[1] = end_date;

                    set_argSETTINGS(setts, args, 2);

                    optionssettings[3].x += 9;
                    optionssettings[3].label = strdup(end_date);
                    wrefresh(query_win);
                }
                break;

            case 27:
                delwin(query_win);
                clear();
                refresh();
                for (int i = 0; i < max_options; i++) {
                    free_button(&optionssettings[i]);
                }
                return;
        }
    }

    delwin(query_win);
    endwin();
}

void process_query5(SETTINGS setts) {
    int win_height = 17, win_width = 61;
    int win_start_y = 4, win_start_x = 12;
    WINDOW* query_win = newwin(win_height, win_width, win_start_y, win_start_x);
    box(query_win, 0, 0);
    
    const int max_options = 4;

    BUTTONS optionssettings[] = {
        create_button(".", 7, 7),
        create_button("Search", 4, 9),
        create_button("[BACK]", 4, 15),
        create_button(".", 10, 7),
    };
    
    int selected = 0;
    keypad(query_win, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE); 
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);  
    init_pair(4, COLOR_WHITE, COLOR_BLUE);

    wrefresh(query_win);

    int ch;
    while (1) {
        box(query_win, 0, 0);
        mvwprintw(query_win, 1, 1, "------------------Query5 Processing Window-----------------");
        mvwprintw(query_win, 2, 5, "QUERY ID: 5; arg1: <username>; arg2: <users_number>");
        mvwprintw(query_win, 4, 1, "Output info: username_1\n              username_2");
        mvwprintw(query_win, 6, 1, "--------------------------Options--------------------------");
        mvwprintw(query_win, 7, 4, "5");
        mvwprintw(query_win, 10, 1, "--------------------------Results--------------------------");
        for (int i = 0; i < max_options; i++) {
            if (i == selected) {
                wattron(query_win, COLOR_PAIR(1));
            } else {
                wattron(query_win, COLOR_PAIR(2));
            }

            mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);

            if (i == selected && i == 2) {
                wattron(query_win, COLOR_PAIR(4));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);
                wattroff(query_win, COLOR_PAIR(4));
                wattron(query_win, COLOR_PAIR(3));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x + 1, "B");
                wattroff(query_win, COLOR_PAIR(3));
            }

            if (i == selected) {
                wattroff(query_win, COLOR_PAIR(1));
            } else {
                wattroff(query_win, COLOR_PAIR(2));
            }
        }

        wrefresh(query_win);

        ch = wgetch(query_win);
        switch (ch) {
            case KEY_UP:
                selected = (selected - 1 + max_options) % max_options;
                break;

            case KEY_DOWN:
                if (selected == 0) {selected = 1; break;}
                if (selected == 3) {selected = 1; break;}
                if (selected == 1) selected = 2;
                break;
            case KEY_RIGHT:
                if (selected==0) {selected = 3; break;}
                break;
            case KEY_LEFT:
                if (selected==3) {selected = 0; break;}
                break;

            case '\n':
                if (selected == 0) {
                    char username[50];
                    move(0, 0);
                    clrtoeol();
                    mvprintw(0, 0, "username: ");
                    wrefresh(query_win);
                    echo();
                    clrtoeol();
                    curs_set(1);
                    getstr(username);
                    curs_set(0);
                    char ** args = get_args(setts);
                    if (!args){
                        args = malloc(sizeof(char*)*3);
                        args[0] = NULL;
                        args[1] = NULL;
                        args[2] = NULL;
                    }
                    args[0] = strdup(username);

                    set_argSETTINGS(setts, args, 1);

                    optionssettings[0].label = strdup(username);
                    wrefresh(query_win);

                    clrtoeol();
                    wrefresh(query_win);

                } else if (selected == 1) {
                    optionssettings[0].label = strdup("........");
                    optionssettings[3].label = strdup("..");
                    char ** args = get_args(setts);
                    if(!args || !args[0] || !args[1]) break;
                    SPOTILIXO data = get_manager(setts);
                    if (!data) break;
                    char* output = queryFive(data, args, 2, ";");
                    if (atoi(args[1]) > 3) display_page(query_win, output);
                    else {
                        int y = 11, x = 1;
                        char* token = strtok(output, "\n");
                        while (token != NULL) {
                            mvwprintw(query_win, y, x, "%s", token);
                            wrefresh(query_win);
                            y++;
                            token = strtok(NULL, "\n");
                        }
                    }
                    draw_background();
                } else if (selected == 2) {
                    clear();
                    refresh();
                    return;
                } else if (selected == 3) {
                    char number[50];
                    while(1){
                        move(0, 0);
                        clrtoeol();
                        mvprintw(0, 0, "users number: ");
                        wrefresh(query_win);
                        echo();
                        clrtoeol();
                        curs_set(1);
                        getstr(number);
                        if (is_number(number)) break;
                    }
                    curs_set(0);
                    char** args = get_args(setts);
                    if (!args){
                        args = malloc(sizeof(char*)*3);
                        args[0] = NULL;
                        args[1] = NULL;
                        args[2] = NULL;
                    }
                    args[1] = number;

                    set_argSETTINGS(setts, args, 2);

                    optionssettings[3].x+=6;
                    optionssettings[3].label = strdup(number);
                    wrefresh(query_win);
                }
                break;

            case 27:
                delwin(query_win);
                clear();
                refresh();
                for (int i = 0; i < max_options; i++) {
                    free_button(&optionssettings[i]);
                }
                return;
        }
    }

    delwin(query_win);
    endwin();
}

void process_query6(SETTINGS setts) {
    int win_height = 20, win_width = 61;
    int win_start_y = 2, win_start_x = 12;
    WINDOW* query_win = newwin(win_height, win_width, win_start_y, win_start_x);
    box(query_win, 0, 0);
    
    const int max_options = 5;

    BUTTONS optionssettings[] = {
        create_button(".", 7, 8),
        create_button("Search", 4, 10),
        create_button("[BACK]", 4, 17),
        create_button(".", 10, 8),
        create_button(".", 13, 8),
    };
    
    int selected = 0;
    keypad(query_win, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE); 
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLUE);  
    init_pair(4, COLOR_WHITE, COLOR_BLUE);

    wrefresh(query_win);

    int ch;
    while (1) {
        box(query_win, 0, 0);
        mvwprintw(query_win, 1, 1, "------------------Query6 Processing Window-----------------");
        mvwprintw(query_win, 2, 4, "QUERY ID: 6; arg1: <user_id>; arg2: <year>; arg3: [N]");
        mvwprintw(query_win, 4, 1, "Output info: listening_time;#musics;artist;day;genre;\n              favorite_album;hour\n              [artista_preferido_1;#musicas;listening_time]");
        mvwprintw(query_win, 7, 1, "--------------------------Options--------------------------");
        mvwprintw(query_win, 8, 4, "6");
        mvwprintw(query_win, 11, 1, "--------------------------Results--------------------------");
        for (int i = 0; i < max_options; i++) {
            if (i == selected) {
                wattron(query_win, COLOR_PAIR(1));
            } else {
                wattron(query_win, COLOR_PAIR(2));
            }

            mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);

            if (i == selected && i == 2) {
                wattron(query_win, COLOR_PAIR(4));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);
                wattroff(query_win, COLOR_PAIR(4));
                wattron(query_win, COLOR_PAIR(3));
                mvwprintw(query_win, optionssettings[i].y, optionssettings[i].x + 1, "B");
                wattroff(query_win, COLOR_PAIR(3));
            }

            if (i == selected) {
                wattroff(query_win, COLOR_PAIR(1));
            } else {
                wattroff(query_win, COLOR_PAIR(2));
            }
        }

        wrefresh(query_win);

        ch = wgetch(query_win);
        switch (ch) {
            case KEY_UP:
                if (selected==1) {selected=0;break;}
                if (selected==2) {selected=1;break;}
                break;

            case KEY_DOWN:
                if (selected == 0) {selected = 1; break;}
                if (selected == 3) {selected = 1; break;}
                if (selected == 1) selected = 2;
                if (selected == 4) {selected = 1; break;}
                break;
            case KEY_RIGHT:
                if (selected==0) {selected = 3; break;}
                if (selected==3) {selected = 4; break;}
                break;
            case KEY_LEFT:
                if (selected==3) {selected = 0; break;}
                if (selected==4) {selected = 3; break;}
                break;

            case '\n':
                if (selected == 0) {
                    char username[50];
                    move(0, 0);
                    clrtoeol();
                    mvprintw(0, 0, "username: ");
                    wrefresh(query_win);
                    echo();
                    clrtoeol();
                    curs_set(1);
                    getstr(username);
                    curs_set(0);
                    char ** args = get_args(setts);
                    if (!args){
                        args = malloc(sizeof(char*)*3);
                        args[0] = NULL;
                        args[1] = NULL;
                        args[2] = NULL;
                    }
                    args[0] = strdup(username);

                    set_argSETTINGS(setts, args, 1);

                    optionssettings[0].label = strdup(username);
                    wrefresh(query_win);

                    clrtoeol();
                    wrefresh(query_win);

                } else if (selected == 1) {
                    optionssettings[0].label = strdup("........");
                    optionssettings[3].label = strdup("....");
                    optionssettings[4].label = strdup(".");
                    char ** args = get_args(setts);
                    if(!args || !args[0] || !args[1]) break;
                    SPOTILIXO data = get_manager(setts);
                    if (!data) break;
                    int n = 2;
                    char* nArt = get_nArtists(setts);
                    if (nArt!=NULL) n++;
                    args[2] = nArt;
                    char* output = querySix(data, args, n, ";");
                    if (atoi(nArt)>2){
                        display_page(query_win, output);
                    }
                    else mvwprintw(query_win, 12,1, "%s", output);
                    wrefresh(query_win);
                    draw_background();
                } else if (selected == 2) {
                    clear();
                    refresh();
                    return;
                } else if (selected == 3) {
                    char number[50];
                    while(1){
                        move(0, 0);
                        clrtoeol();
                        mvprintw(0, 0, "year: ");
                        wrefresh(query_win);
                        echo();
                        clrtoeol();
                        curs_set(1);
                        getstr(number);
                        if (is_number(number)) break;
                    }
                    curs_set(0);
                    char** args = get_args(setts);
                    if (!args){
                        args = malloc(sizeof(char*)*3);
                        args[0] = NULL;
                        args[1] = NULL;
                        args[2] = NULL;
                    }
                    args[1] = number;

                    set_argSETTINGS(setts, args, 2);

                    optionssettings[3].x += 6;
                    optionssettings[3].label = strdup(number);
                    wrefresh(query_win);
                } else if (selected == 4) {
                    char artists[50];
                    while(1){
                        move(0, 0);
                        clrtoeol();
                        mvprintw(0, 0, "number of related artists: ");
                        wrefresh(query_win);
                        echo();
                        clrtoeol();
                        curs_set(1);
                        getstr(artists);
                        if (is_number(artists)) break;
                    }
                    curs_set(0);

                    set_nArtists(setts, artists);

                    optionssettings[4].x += 8;
                    optionssettings[4].label = strdup(artists);
                    wrefresh(query_win);
                }
                break;

            case 27:
                delwin(query_win);
                clear();
                refresh();
                for (int i = 0; i < max_options; i++) {
                    free_button(&optionssettings[i]);
                }
                return;
        }
    }

    delwin(query_win);
    endwin();
}