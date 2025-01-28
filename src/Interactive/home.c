#include "Interactive/home.h"
#include "Interactive/info.h"
#include "Interactive/querysolver.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPTIONS 8
#define MAX_OPTIONS_SETTINGS 6

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

static int load_data(SETTINGS setts, WINDOW* home_win) {
    WINDOW* load_win = newwin(5, 65, 19, 10);
    box(load_win, 0, 0);
    mvwprintw(load_win, 2, 5, "Insert directory on terminal above..");
    wrefresh(load_win);

    char DatasetsPath[50];
    echo();
    mvprintw(0, 0, "Datasets directory: ");
    clrtoeol();
    curs_set(1);
    getstr(DatasetsPath);

    set_datasetPath(setts, DatasetsPath);

    noecho();

    move(0,0);
    clrtoeol();

    werase(load_win);
    box(load_win, 0, 0);
    mvwprintw(load_win, 2, 5, "Processing data..");
    wrefresh(load_win);

    curs_set(0);

    int status = process_info(setts, DatasetsPath);
    move(0,0);
    if (status) {
        printw("Data lodaded..");
    } else {
        set_datasetPath(setts, "./");
        printw("Unable to load data..");
        wrefresh(home_win);
        noecho();
        return 0;
    }
    
    wclear(home_win);
    clear();
    refresh();
    wrefresh(home_win);
    noecho();

    return 1;
}

static int load_inputs(SETTINGS setts, WINDOW* home_win) {
    WINDOW* load_win = newwin(5, 65, 19, 10);
    box(load_win, 0, 0);
    mvwprintw(load_win, 2, 5, "Insert directory on terminal above..");
    wrefresh(load_win);

    char InputsPath[50];
    echo();
    mvprintw(0, 0, "Inputs directory file (w/ extension): ");
    clrtoeol();
    curs_set(1);
    getstr(InputsPath);

    set_inputsPath(setts, InputsPath);

    noecho();

    move(0,0);
    clrtoeol();

    wclear(load_win);
    werase(load_win);
    wrefresh(load_win);

    curs_set(0);

    int statusinputs = process_inputs(setts, InputsPath);
    move(0,0);
    if (statusinputs) {
        printw("Inputs lodaded..");
    } else {
        set_inputsPath(setts, "./");
        printw("Unable to open inputs file..");
        wrefresh(home_win);
        noecho();
        return 0;
    }

    wclear(home_win);
    wrefresh(home_win);
    noecho();

    return 1;
}

void draw_background() {
    const char* image[] = {
        "## ## ### #### #### ### ## # #",
        "#                            #",
        "#   ##########     ########  #",
        "#   |        |     |      |  #",
        "#   ##########     ########  #",
        "#                            #",
        "#########   #$#   #$#   ######",
        "        #   # #   # #   #     ",
        "#########   #$#   #$#   ######",
        "#                            #",
        "## ## ### #### #### ### ## # #"
    };

    int logo_rows = 11;
    int logo_cols = 30;
    int screen_rows, screen_cols;

    getmaxyx(stdscr, screen_rows, screen_cols);

    for (int logo_start_y = 0; logo_start_y < screen_rows; logo_start_y += logo_rows) {
        for (int logo_start_x = -3; logo_start_x < screen_cols; logo_start_x += logo_cols) {
            for (int i = 0; i < logo_rows; i++) {
                for (int j = 0; j < logo_cols; j++) {
                    if (logo_start_y + i < screen_rows && logo_start_x + j < screen_cols) {
                        if (image[i][j] == '#') {
                            attron(COLOR_PAIR(1));
                        } else {
                            attron(COLOR_PAIR(2));
                        }
                        mvaddch(logo_start_y + i, logo_start_x + j, image[i][j]);
                        attroff(COLOR_PAIR(1) | COLOR_PAIR(2));
                    }
                }
            }
        }
    }

    refresh();
}

void home(SETTINGS setts) {
    initscr();
    cbreak();
    start_color();
    noecho();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    draw_background();

    WINDOW* settings = newwin(8, 65, 3, 10);
    const char* titlesettings = "---------------------------Settings----------------------------";
    int selectedsettings = 0;

    char* dataset = get_datasetPath(setts);
    char* inputs = get_inputsPath(setts);
    char* outputs = get_outputPath(setts);

    BUTTONS optionssettings[MAX_OPTIONS_SETTINGS] = {
        create_button("Dataset path:", 2, 3),
        create_button(dataset, 16, 3),
        create_button("Inputs path:", 2, 5),
        create_button(inputs, 16, 5),
        create_button("Outputs path:", 33, 3),
        create_button(outputs, 33, 5),
    };

    WINDOW* home_win = newwin(12, 65, 12, 10);
    refresh();

    int ch;
    const char* title = "-------------------Spotilixo Query Processor-------------------";

    BUTTONS options[MAX_OPTIONS] = {
        create_button("Process query1", 7, 3),
        create_button("Process query2", 7, 5),
        create_button("Process query4", 33, 3),
        create_button("Process query5", 33, 5),
        create_button("Process query6", 33, 7),
        create_button("Process query3", 7, 7),
        create_button("Process inputs file", 7, 9),
        create_button("Exit", 33, 9),
    };

    int selected = -1;
    int current_menu = 0;

    while (1) {
        curs_set(0);

        werase(settings);
        box(settings, 0, 0);
        mvwprintw(settings, 1, 1, "%s", titlesettings);
        for (int i = 0; i < MAX_OPTIONS_SETTINGS; i++) {
            if (i == selectedsettings) {
                wattron(settings, COLOR_PAIR(1));
                mvwprintw(settings, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);
                wattroff(settings, COLOR_PAIR(1));
            } else {
                wattron(settings, COLOR_PAIR(2));
                mvwprintw(settings, optionssettings[i].y, optionssettings[i].x, "%s", optionssettings[i].label);
                wattroff(settings, COLOR_PAIR(2));
            }
        }
        wrefresh(settings);
        werase(home_win);
        box(home_win, 0, 0);
        mvwprintw(home_win, 1, 1, "%s", title);
        for (int i = 0; i < MAX_OPTIONS; i++) {
            if (i == selected) {
                wattron(home_win, COLOR_PAIR(1));
                mvwprintw(home_win, options[i].y, options[i].x, "%s", options[i].label);
                wattroff(home_win, COLOR_PAIR(1));
            } else {
                wattron(home_win, COLOR_PAIR(2));
                mvwprintw(home_win, options[i].y, options[i].x, "%s", options[i].label);
                wattroff(home_win, COLOR_PAIR(2));
            }
        }
        wrefresh(home_win);

        ch = getch();
        switch (ch) {
            case KEY_UP:
                if (current_menu==0){
                    if (selectedsettings==2) selectedsettings = 0;
                }
                if (current_menu==1){
                    if (selected == 0) {
                        selectedsettings = 2;
                        selected = -1;
                        current_menu = 0;
                        break;
                    }
                    if (selected==0) {
                        current_menu = 0;
                        selected = -1;
                        selectedsettings = 2;
                        break;
                    }
                    if (selected==1) selected = 0;
                    if (selected==4) {selected = 3; break;}
                    if (selected==5) {selected = 1;break;}
                    if (selected==3) {selected = 2;break;}
                    if (selected==6) {selected = 5; break;}
                    if (selected==7) {selected = 4; break;}
                    if (selected==2){
                        current_menu = 0;
                        selected = -1;
                        selectedsettings = 4;
                    }
                }
                break;

            case KEY_DOWN:
                if (current_menu==0){
                    if (selectedsettings==0){
                        selectedsettings = 2;
                        break;
                    }
                    if (selectedsettings==2) {
                        current_menu = 1;
                        selectedsettings = -1;
                        selected = 0;
                        break;
                    }
                    if (selectedsettings==4){
                        current_menu = 1;
                        selectedsettings = -1;
                        selected = 2;
                        break;
                    }
                }
                if (current_menu==1){
                    if (selected==0) {selected = 1; break;}
                    if (selected==1) {selected = 5;break;}
                    if (selected==5) {selected = 6; break;}
                    if (selected==2) {selected = 3;break;}
                    if (selected==3) {selected = 4; break;}
                    if (selected==4) {selected = 7; break;}
                }

            case KEY_LEFT:
                if (current_menu == 0) {
                    if (selectedsettings==1) selectedsettings = 0;
                    if (selectedsettings==3) selectedsettings = 2;
                    if (selectedsettings==4) selectedsettings = 0;
                }
                if (current_menu == 1) {
                    if (selected==2) selected = 0;
                    if (selected==3) selected = 1;
                    if (selected==5) selected = 4;
                    if (selected==4) selected = 5;
                    if (selected==7) selected = 6;
                }
                break;

            case KEY_RIGHT:
                if (current_menu == 0) {
                    if (selectedsettings==0) selectedsettings = 4;
                    if (selectedsettings==2) selectedsettings = 3;
                    if (selectedsettings==3) selectedsettings = 4;
                }
                if (current_menu == 1) {
                    if (selected == 0) selected = 2;
                    if (selected == 1) selected = 3;
                    if (selected == 4) selected = 5;
                    if (selected == 5) selected = 4;
                    if (selected == 6) selected = 7;
                }
                break;

            case '\n':
                if (current_menu == 0) {
                    if (selectedsettings == 0) {
                        int statusdatasets = load_data(setts, home_win);
                        draw_background();
                        if (statusdatasets) {
                            dataset = get_datasetPath(setts);
                            optionssettings[1].label = dataset;
                        } else {
                            optionssettings[1].label = "./";
                            wclear(home_win);
                            wrefresh(home_win);
                        }
                    } else if (selectedsettings == 2) {
                        int statusinputs = load_inputs(setts,home_win);
                        draw_background();
                        if (statusinputs) {
                            inputs = get_inputsPath(setts);
                            optionssettings[3].label = inputs;
                        } else {
                            optionssettings[3].label = "./";
                            wclear(home_win);
                            wrefresh(home_win);
                        }
                    } else if (selectedsettings == 4) {
                        move(0,0);
                        char outputPath[50];
                        echo();
                        mvprintw(0, 0, "Output directory: ");
                        clrtoeol();
                        curs_set(1);
                        getstr(outputPath);
                        set_outputPath(setts, outputPath);
                        noecho();
                        move(0,0);
                        clrtoeol();
                        optionssettings[5].label = outputPath;
                    }
                } else if (current_menu == 1) {
                    char* option = options[selected].label;

                    if (strcmp(option, "Process inputs file") == 0) {
                        move(0,0);
                        clrtoeol();

                        char* dataPath = get_datasetPath(setts);
                        char* inputsPath = get_inputsPath(setts);
                        if (strcmp(dataPath,"./")==0){
                            move(0,0);
                            clrtoeol();
                            printw("Missing datasets..");
                            free(dataPath);
                            free(inputsPath);
                            break;
                        }
                        if (strcmp(inputsPath,"./")==0){
                            move(0,0);
                            clrtoeol();
                            printw("Missing inputs file..");
                            free(dataPath);
                            free(inputsPath);
                            break;
                        }
                        free(dataPath);
                        free(inputsPath);
                        int datastatus = process_data(setts);
                        if (datastatus){
                            move(0,0);
                            clrtoeol();
                            char * outputsPath = get_outputPath(setts);
                            printw("Queries processed, outputs in %s..", outputs);
                            free(outputsPath);
                        } else {
                            move(0,0);
                            clrtoeol();
                            printw("Unable to process data..");
                        }
                    } 
                    if (selected == 7) {
                        for (int i = 0; i < MAX_OPTIONS; i++) {
                            free_button(&options[i]);
                        }
                        delwin(home_win);
                        endwin();
                        return;
                    }
                    //process query1
                    if (selected ==0) {
                        process_query1(setts);
                        wrefresh(home_win);
                        draw_background();
                        wrefresh(settings);
                    }
                    if (selected==1) {
                        process_query2(setts);
                        wrefresh(home_win);
                        draw_background();
                        wrefresh(settings);
                    }
                    if (selected==5) {
                        process_query3(setts);
                        wrefresh(home_win);
                        draw_background();
                        wrefresh(settings);
                    }
                    if (selected==2) {
                        process_query4(setts);
                        wrefresh(home_win);
                        draw_background();
                        wrefresh(settings);
                    }
                    if (selected==3) {
                        process_query5(setts);
                        wrefresh(home_win);
                        draw_background();
                        wrefresh(settings);
                    }
                    if (selected==4) {
                        process_query6(setts);
                        wrefresh(home_win);
                        draw_background();
                        wrefresh(settings);
                    }
                }
                break;
        }
    }

    delwin(home_win);
    endwin();
}
