#define VOOM_SECTION
#include "../voom_main.h"
#include "display_internals.h"


Window *windows_info;

int startVoom(Window *main_win) {
    int x = main_win->coords.width;
    WINDOW *workplace_win = main_win->window;

    int a = (x / 2) - 20;
    int d = 2;
    int c = 0;

    wattron(workplace_win, A_BOLD | COLOR_PAIR(9));
    while (c != 5) {
        mvwprintw(workplace_win, d, a, "%s", voom[c]);
        c++;
        d++;
    }
    wattroff(workplace_win, A_BOLD | COLOR_PAIR(9));

    wattron(workplace_win, COLOR_PAIR(11));
    wprintw(workplace_win, "..Editor");
    wattroff(workplace_win, COLOR_PAIR(11));

    d++;
    int e = (x / 2) - 6;
    mvwprintw(workplace_win, d, e, "VOOM version %.1f\n", version);

    d++;
    int f = (x / 2) - 27;
    mvwprintw(workplace_win, d, f, "voom is a simple programmable text & code editor\n\n");

    d += 2;
    e = (x / 2) - 13;

    wattron(workplace_win, A_BOLD | COLOR_PAIR(9));
    mvwprintw(workplace_win, d, e, "#)Author :");
    wattroff(workplace_win, A_BOLD | COLOR_PAIR(9));

    wattron(workplace_win, COLOR_PAIR(10));
    wprintw(workplace_win, " Edwin_ignas\n");
    wattroff(workplace_win, COLOR_PAIR(10));

    d++;
    wattron(workplace_win, A_BOLD | COLOR_PAIR(9));
    mvwprintw(workplace_win, d, e, "#)Website :");
    wattroff(workplace_win, A_BOLD | COLOR_PAIR(9));

    wattron(workplace_win, COLOR_PAIR(10));
    wprintw(workplace_win, "Edwinignas.com\n");
    wattroff(workplace_win, COLOR_PAIR(10));

    wrefresh(workplace_win);
    wgetch(workplace_win);
    return EXIT_SUCCESS;
}

void close_subwins(Window *window) {
    for (int i = 0; i < window->num_subs; i++) {
        delwin(window->subs[i]->window);
        free(window->subs[i]);
    }
    free(window->subs);
}

void terminal_stop() {
    int win_num = 5;
    if(cmd_info->display_line_numbers)win_num++;

    for (int i = 0; i < win_num; i++) {
        if (windows_info[i].has_subs) {
            close_subwins(&windows_info[i]);
        }
        if (windows_info[i].window != NULL) {
            delwin(windows_info[i].window);
        }
    }
    endwin();
}


PRIVATE int terminal_start(){
    if (initscr() == NULL){
        fprintf(stderr, "Could not initialize ncurses!\n");
        exit(EXIT_FAILURE);
    }
    raw();
    clear();
    noecho();
    keypad(stdscr, TRUE);
    return EXIT_SUCCESS;
}

void copy_structs(config_t *dest, const config_t *src) {
  
  #define COPY_INT_FIELD(field) \
    if(dest->field == 0){ \
        dest->field = src->field; \
        cmd_info->field = dest->field; \
    }

#define COPY_STRING_FIELD(field) \
    if (strlen(dest->field) == 0) { \
        strcpy(dest->field, src->field); \
    } else { \
        if (dest->field != NULL && *dest->field != '#') { \
            for (size_t i = 0; i < COLOR_NUM; i++) { \
                if (!strcmp(dest->field, color_codes[i].color)) { \
                    strcpy(dest->field, color_codes[i].code); \
                    break; \
                } \
            } \
        } \
    }


  // Copy integer fields
  COPY_INT_FIELD(tab_width);

  //Copy boolean fields (treated as integers here)
  COPY_INT_FIELD(text_autowrap);
  COPY_INT_FIELD(display_encoding);
  COPY_INT_FIELD(display_lang_server);
  COPY_INT_FIELD(display_tabspace);
  COPY_INT_FIELD(display_line_numbers);
  COPY_INT_FIELD(display_cursor_position);
  COPY_INT_FIELD(indent_using_spaces);
  COPY_INT_FIELD(highlight_current_line);
  COPY_INT_FIELD(enable_spellcheck);
  COPY_INT_FIELD(enable_autocorrect);
  COPY_INT_FIELD(enable_mouse);
  COPY_INT_FIELD(display_time);

  // Copy string fields
  COPY_STRING_FIELD(title_bg);
  COPY_STRING_FIELD(title_fg);
  COPY_STRING_FIELD(title_bg_voom);
  COPY_STRING_FIELD(title_fg_voom);
  COPY_STRING_FIELD(side_bg);
  COPY_STRING_FIELD(side_fg);
  COPY_STRING_FIELD(command_bg);
  COPY_STRING_FIELD(command_fg);
  COPY_STRING_FIELD(command_bg_cmd);
  COPY_STRING_FIELD(command_fg_cmd);
  COPY_STRING_FIELD(main_bg);
  COPY_STRING_FIELD(main_fg);
  COPY_STRING_FIELD(status_bg);
  COPY_STRING_FIELD(status_fg);
  COPY_STRING_FIELD(status_bg_status);
  COPY_STRING_FIELD(status_fg_status);

  #undef COPY_INT_FIELD
  #undef COPY_STRING_FIELD
}

void init_config() {
    static config_t config_default = {
        .tab_width = 4,
        .text_autowrap = true,
        .display_encoding = true,
        .display_lang_server = true,
        .display_tabspace = true,
        .display_line_numbers = true,
        .display_cursor_position = true,
        .indent_using_spaces = true,
        .highlight_current_line = false,
        .enable_spellcheck = true,
        .enable_autocorrect = false,
        .enable_mouse = false,
        .display_time = true,
        .title_bg = "#(3,46,66)",
        .title_fg = "#(255,87,51)",
        .title_bg_voom = "(#39,122,161)",
        .title_fg_voom = "#88,24,69)",
        .side_bg = "#(39,122,161)",
        .side_fg = "#(255,87,51)",
        .command_bg = "#(39,122,161)",
        .command_fg = "#(255,255,255)",
        .command_bg_cmd = "#(39,122,161)",
        .command_fg_cmd = "#(0,250,154)",
        .main_bg = "#(17,64,86)",
        .main_fg = "#(255,255,255)",
        .status_bg = "#(39,122,161)",
        .status_fg = "#(255,87,51)",
        .status_bg_status = "#(0,128,0)",
        .status_fg_status = "#(0,0,0)"
    };

    // Initialize main_config with default values
    copy_structs(config, &config_default);
    return;
}

bool extract_colors(const char *str, uint8_t *red, uint8_t *green, uint8_t *blue) {
  // Check for empty or invalid input strings
  if (!str || *str == '\0') {
    fprintf(stderr, "Invalid RGB string: empty or NULL\n");
    return false;
  }

  // Skip the initial '#' if present, but handle strings with only "#"
  if (*str == '#') {
    str++;
    if (*str == '\0') {
      fprintf(stderr, "Invalid RGB string: only '#' found\n");
      return false;
    }
  }

  // Parse the RGB values using sscanf
  int values_parsed = sscanf(str, "(%hhu,%hhu,%hhu)", red, green, blue);

  if (values_parsed != 3) {
    fprintf(stderr, "Invalid RGB string format\n");
    return false;
  }

  // Restrict values to 0-1000 for ncurses compatibility (assuming 0-1000 range)
  *red = (*red * 1000) / 255;
  *green = (*green * 1000) / 255;
  *blue = (*blue * 1000) / 255;

  return true;
}


void setup_pair(uint8_t pair, char *bg, char *fg) {

  uint8_t bg_red, bg_green, bg_blue;
  uint8_t fg_red, fg_green, fg_blue;
  if (!extract_colors(bg, &bg_red, &bg_green, &bg_blue) ||
      !extract_colors(fg, &fg_red, &fg_green, &fg_blue)) {
    fprintf(stderr, "Failed to extract colors from strings\n");
    return;
  }

  // Initialize colors and pair:
  init_color(COLOR_BLACK, bg_red, bg_green, bg_blue);
  init_color(COLOR_YELLOW, fg_red, fg_green, fg_blue);
  init_pair(pair, COLOR_YELLOW, COLOR_BLACK);
}

void setup_color_display(){
if(has_colors()){
    start_color();
    cmd_info->has_colors = true;
    if(can_change_color()){
        init_color(COLOR_RED,17,64,86);
        init_color(COLOR_BLACK, 39, 122, 161);
        init_color(COLOR_CYAN,3, 46, 66);
        init_color(COLOR_YELLOW,237, 237, 12);
        init_color(COLOR_BLUE,94, 131, 166);
        init_pair(1,COLOR_MAGENTA,COLOR_BLUE); // title_win *unselected
        init_pair(2,COLOR_WHITE, COLOR_GREEN); //workplace_bg_voom
        init_pair(3,COLOR_BLUE,COLOR_CYAN); // side/numbers bar
        init_pair(4,COLOR_WHITE,COLOR_CYAN); //command
        init_pair(5,COLOR_MAGENTA,COLOR_CYAN);  //command_cmd
        init_pair(6,COLOR_WHITE,COLOR_RED); //main_win
        init_pair(7,COLOR_MAGENTA,COLOR_CYAN); //status_win
        init_pair(8,COLOR_GREEN,COLOR_WHITE); // status_win_status
        init_pair(9,COLOR_GREEN,COLOR_RED);
        init_pair(10,COLOR_YELLOW,COLOR_RED);
        init_pair(11,COLOR_MAGENTA,COLOR_RED);
        // setup_pair(1, main_config->title_bg, main_config->title_fg);
        // setup_pair(2, main_config->title_bg_voom, main_config->title_fg_voom);
        // setup_pair(3, main_config->side_bg, main_config->side_fg);
        // setup_pair(4, main_config->command_bg, main_config->command_fg);
        // setup_pair(5, main_config->command_bg_cmd, main_config->command_fg_cmd);
        // setup_pair(6, main_config->main_bg, main_config->main_fg);
        // setup_pair(7, main_config->status_bg, main_config->status_fg);
        // setup_pair(8, main_config->status_bg_status, main_config->status_fg_status);
    }else{
        init_pair(1,COLOR_MAGENTA, COLOR_BLUE);
        init_pair(2,COLOR_MAGENTA,COLOR_GREEN);
        init_pair(3,COLOR_MAGENTA,COLOR_BLACK);
        init_pair(4,COLOR_WHITE,COLOR_BLACK);
        init_pair(5,COLOR_WHITE,COLOR_BLUE);
        init_pair(6,COLOR_WHITE,COLOR_BLACK);
        init_pair(7,COLOR_WHITE,COLOR_BLACK);
        init_pair(8,COLOR_GREEN,COLOR_WHITE);
    }
}else
    cmd_info->has_colors = false;
    //todo...handle this case.
}


void title_setup(Window *title) {
    wattron(title->window, COLOR_PAIR(1));
    wbkgd(title->window, COLOR_PAIR(1));
    wnoutrefresh(title->window);
    int voom_width = 6;
    title->has_subs = true;
    title->num_subs = (cmd_info->file_no + 1);
    title->subs = calloc(title->num_subs, sizeof(SubWin *));
    if (title->subs == NULL) {
        log_error("title->subs: could not allocate memory");
    }

    for (uint8_t i = 0; i < title->num_subs; i++) {
        title->subs[i] = calloc(1, sizeof(SubWin));
        if (title->subs[i] == NULL) {
            log_error("title->subs[%d]: could not allocate memory", i);
        }
    }

    FILL_SUB_WIN_STRUCT(title, 0, 1, voom_width, 0, 0);
    title->subs[0]->window = derwin(title->window, title->subs[0]->coords.height, title->subs[0]->coords.width,
                                    title->subs[0]->coords.start_y, title->subs[0]->coords.start_x);
    if (title->subs[0]->window == NULL) {
        log_error("title->subs[0]: could not initilize subwin");
    }

    wattron(title->subs[0]->window, COLOR_PAIR(2) | A_BOLD);
    wbkgd(title->subs[0]->window, COLOR_PAIR(2));
    mvwprintw(title->subs[0]->window, 0, 0, "VOOM");
    wnoutrefresh(title->subs[0]->window);
    wattroff(title->subs[0]->window, COLOR_PAIR(2) | A_BOLD);

    if (cmd_info->file_no == 0) {
        int sub_width = (COLS - 5) / 2;
        int width = 12;
        WINDOW *sub_win = derwin(title->window, 1, (COLS - 5), 0, 5);
        if (sub_win == NULL) {
            log_error("title->subs[0]: could not initilize subwin");
        }

        wattron(sub_win, COLOR_PAIR(1));
        wbkgd(sub_win, COLOR_PAIR(1));
        mvwprintw(sub_win, 0, (sub_width - 4), "%s", "untitled");
        wnoutrefresh(sub_win);
        wattroff(sub_win, COLOR_PAIR(1));
        doupdate();
        return;
    }
    int width = ((COLS - voom_width) / (cmd_info->file_no));
    int x = 6;
    for (uint8_t count = 1; count < title->num_subs; count++, x += width) {
        FILL_SUB_WIN_STRUCT(title, count, 1, width, 0, x);
        title->subs[count]->window = derwin(title->window, title->subs[count]->coords.height, title->subs[count]->coords.width,
                                            title->subs[count]->coords.start_y, title->subs[count]->coords.start_x);
        if (title->subs[count]->window == NULL) {
            log_error("title->subs[%d]: could not initilize subwin", count);
        }
    }

    for (uint8_t count = 1, file = 0; count < title->num_subs; count++, file++) {
        int x = (width - strlen(cmd_info->file_names[file])) / 2;
        if (file == cmd_info->default_file) {
            wattron(title->subs[count]->window, COLOR_PAIR(6));
            wbkgd(title->subs[count]->window, COLOR_PAIR(6));
        } else {
            wattron(title->subs[count]->window, COLOR_PAIR(1));
            wbkgd(title->subs[count]->window, COLOR_PAIR(1));
        }

        mvwprintw(title->subs[count]->window, 0, x, "%s", cmd_info->file_names[file]);
        if (file < title->num_subs - 1) {
            mvwaddch(title->subs[count]->window, 0, (width - 1), '|');
        }

        wnoutrefresh(title->subs[count]->window);
        if (file == cmd_info->default_file)
            wattroff(title->subs[count]->window, COLOR_PAIR(6));
        else
            wattroff(title->subs[count]->window, COLOR_PAIR(1));
    }
}

void sidebar_count(Window *side, int ed_l) {
    wattron(side->window, COLOR_PAIR(3));
    wbkgd(side->window, COLOR_PAIR(3));
    //wclear(side->window);

    int dot_count = 0, line_count = 1;
    int sidebar_w_adj = 4 - MAX_LINE_DIGITS;

    for (int count = 1; count <= ed_l; count++) {
        int line_digits = snprintf(NULL, 0, "%d", line_count);
        int x_pos = sidebar_w_adj + (line_digits < MAX_LINE_DIGITS ? MAX_LINE_DIGITS - line_digits : 0);
        mvwprintw(side->window, dot_count, x_pos, "%d", line_count);

        dot_count++;
        line_count++;
    }

    for (size_t y = dot_count; y < (side->coords.height); y++) {
        mvwprintw(side->window, y, 3, "~");
    }

    wattroff(side->window, COLOR_PAIR(3));
    wnoutrefresh(side->window);
}

void sidebar_setup(Window *side, Window *scroll, bool display_line_numbers) {
    wattron(scroll->window, COLOR_PAIR(6));
    wbkgd(scroll->window, COLOR_PAIR(6));
    wrefresh(scroll->window);
    wattroff(scroll->window, COLOR_PAIR(6));

    if (display_line_numbers == true)
        sidebar_count(side, 20);

    wnoutrefresh(side->window);
}

void command_setup(Window *cmd) {
    wattron(cmd->window, COLOR_PAIR(4));
    wbkgd(cmd->window, COLOR_PAIR(4));
    cmd->has_subs = true;
    cmd->num_subs = 2;
    cmd->subs = calloc(cmd->num_subs, sizeof(SubWin *));
    if (cmd->subs == NULL) {
        log_error("cmd_sub: memory allocation failed");
        return;
    }

    for (uint8_t i = 0; i < cmd->num_subs; i++) {
        cmd->subs[i] = calloc(1, sizeof(SubWin));
        if (cmd->subs[i] == NULL) {
            log_error("cmd->subs[%d]: could not allocate memory", i);
            return;
        }
    }

    FILL_SUB_WIN_STRUCT(cmd, 0, 1, 7, LINES - 1, 0);
    cmd->subs[0]->window = subwin(cmd->window, cmd->subs[0]->coords.height, cmd->subs[0]->coords.width,
                                  cmd->subs[0]->coords.start_y, cmd->subs[0]->coords.start_x);
    if (cmd->subs[0]->window == NULL) {
        log_error("cmd->subs[0]: memory allocation failed");
        return;
    }

    wattron(cmd->subs[0]->window, COLOR_PAIR(5) | A_BOLD);
    wbkgd(cmd->subs[0]->window, COLOR_PAIR(5));
    wnoutrefresh(cmd->subs[0]->window); // Update the subwindow without immediate refresh
    wattroff(cmd->subs[0]->window, COLOR_PAIR(5) | A_BOLD);

    FILL_SUB_WIN_STRUCT(cmd, 1, 1, COLS - 7, LINES - 1, 7);
    cmd->subs[1]->window = subwin(cmd->window, cmd->subs[1]->coords.height, cmd->subs[1]->coords.width,
                                  cmd->subs[1]->coords.start_y, cmd->subs[1]->coords.start_x);
    if (cmd->subs[1]->window == NULL) {
        log_error("cmd->subs[1]: failed to initilize window");
        return;
    }

    wnoutrefresh(cmd->window); // Update the parent window without immediate refresh
    doupdate(); // Refresh the screen with all updates
}

void status_setup(Window *status) {
    int y = LINES - 2;
    int available_width = COLS;

    wattron(status->window, COLOR_PAIR(7));
    wbkgd(status->window, COLOR_PAIR(7));
    status->has_subs = true;
    status->num_subs = 0; // Initialize to 0, as we'll add subwindows dynamically

    int mode_width = 10;
    int cursor_position_width = 14;
    int unchanging_variables_width = 30;
    int time_width = 8;

    do {
        // Condition for available_width < 35
        if (available_width < 35) {
            status->num_subs = 2;
            status->subs = calloc(status->num_subs, sizeof(SubWin *));
            if (status->subs == NULL) {
                log_error("status->subs: memory allocation failed");
                return;
            }

            for (uint8_t i = 0; i < status->num_subs; i++) {
                status->subs[i] = calloc(1, sizeof(SubWin));
                if (status->subs[i] == NULL) {
                    log_error("status->subs[%d]: could not allocate memory", i);
                    return;
                }
            }

            // Mode subwin
            FILL_SUB_WIN_STRUCT(status, 0, 1, mode_width, y, 0);
            status->subs[0]->window = subwin(status->window, status->subs[0]->coords.height, status->subs[0]->coords.width,
                                             status->subs[0]->coords.start_y, status->subs[0]->coords.start_x);
            if (status->subs[0]->window == NULL) {
                log_error("status->subs[0]: could not inilize the window");
                return;
            }

            wattron(status->subs[0]->window, COLOR_PAIR(8));
            wbkgd(status->subs[0]->window, COLOR_PAIR(8));
            mvwprintw(status->subs[0]->window, 0, 0, "%s", cmd_info->mode);
            wnoutrefresh(status->subs[0]->window); // Update the subwindow without immediate refresh

            // Time subwin
            FILL_SUB_WIN_STRUCT(status, 1, 1, time_width, y, COLS - time_width);
            status->subs[1]->window = subwin(status->window, status->subs[1]->coords.height, status->subs[1]->coords.width,
                                             status->subs[1]->coords.start_y, status->subs[1]->coords.start_x);
            if (status->subs[1]->window == NULL) {
                log_error("status->subs[1]: could not inilize the window");
                return;
            }

            mvwprintw(status->subs[1]->window, 0, 0, "%s", "00:00");
            wnoutrefresh(status->subs[1]->window); // Update the subwindow without immediate refresh
        }

        // Condition for 35 <= available_width < 67
        else if (IN_RANGE(available_width, 35, 67)) {
            status->num_subs = 3;
            status->subs = calloc(status->num_subs, sizeof(SubWin *));
            if (status->subs == NULL) {
                log_error("status->subs: memory allocation failed");
                return;
            }

            for (uint8_t i = 0; i < status->num_subs; i++) {
                status->subs[i] = calloc(1, sizeof(SubWin));
                if (status->subs[i] == NULL) {
                    log_error("status->subs[%d]: could not allocate memory", i);
                    return;
                }
            }

            // Mode subwin
            FILL_SUB_WIN_STRUCT(status, 0, 1, mode_width, y, 0);
            status->subs[0]->window = subwin(status->window, status->subs[0]->coords.height, status->subs[0]->coords.width,
                                             status->subs[0]->coords.start_y, status->subs[0]->coords.start_x);
            if (status->subs[0]->window == NULL) {
                log_error("status->subs[0]: could not inilize the window");
                return;
            }

            wattron(status->subs[0]->window, COLOR_PAIR(8));
            wbkgd(status->subs[0]->window, COLOR_PAIR(8));
            mvwprintw(status->subs[0]->window, 0, 0, "%s", cmd_info->mode);
            wnoutrefresh(status->subs[0]->window); // Update the subwindow without immediate refresh

            // Time subwin
            FILL_SUB_WIN_STRUCT(status, 1, 1, time_width, y, COLS - time_width);
            status->subs[1]->window = subwin(status->window, status->subs[1]->coords.height, status->subs[1]->coords.width,
                                             status->subs[1]->coords.start_y, status->subs[1]->coords.start_x);
            if (status->subs[1]->window == NULL) {
                log_error("status->subs[1]: could not inilize the window");
                return;
            }

            mvwprintw(status->subs[1]->window, 0, 0, "%s", "00:00");
            wnoutrefresh(status->subs[1]->window); // Update the subwindow without immediate refresh

            // Cursor position subwin
            int start_x = (((COLS - (time_width + mode_width)) / 2) - (cursor_position_width / 2)) + mode_width;
            FILL_SUB_WIN_STRUCT(status, 2, 1, cursor_position_width, y, start_x);
            status->subs[2]->window = subwin(status->window, status->subs[2]->coords.height, status->subs[2]->coords.width,
                                             status->subs[2]->coords.start_y, status->subs[2]->coords.start_x);
            if (status->subs[2]->window == NULL) {
                log_error("status->subs[2]: could not inilize the window");
                return;
            }

            mvwprintw(status->subs[2]->window, 0, 0, "ln %d, col %d", 0, 0);
            wnoutrefresh(status->subs[2]->window); // Update the subwindow without immediate refresh
        }

        // Condition for available_width >= 67
        else if (available_width > 67) {
            status->num_subs = 4;
            status->subs = calloc(status->num_subs, sizeof(SubWin *));
            if (status->subs == NULL) {
                log_error("status->subs: memory allocation failed");
                return;
            }

            for (uint8_t i = 0; i < status->num_subs; i++) {
                status->subs[i] = calloc(1, sizeof(SubWin));
                if (status->subs[i] == NULL) {
                    log_error("status->subs[%d]: could not allocate memory", i);
                    return;
                }
            }

            // Mode subwin
            FILL_SUB_WIN_STRUCT(status, 0, 1, mode_width, y, 0);
            status->subs[0]->window = subwin(status->window, status->subs[0]->coords.height, status->subs[0]->coords.width,
                                             status->subs[0]->coords.start_y, status->subs[0]->coords.start_x);
            if (status->subs[0]->window == NULL) {
                log_error("status->subs[0]: could not inilize the window");
                return;
            }

            wattron(status->subs[0]->window, COLOR_PAIR(8));
            wbkgd(status->subs[0]->window, COLOR_PAIR(8));
            mvwprintw(status->subs[0]->window, 0, 0, "%s", cmd_info->mode);
            wnoutrefresh(status->subs[0]->window); // Update the subwindow without immediate refresh

            // Time subwin
            FILL_SUB_WIN_STRUCT(status, 1, 1, time_width, y, COLS - time_width);
            status->subs[1]->window = subwin(status->window, status->subs[1]->coords.height, status->subs[1]->coords.width,
                                             status->subs[1]->coords.start_y, status->subs[1]->coords.start_x);
            if (status->subs[1]->window == NULL) {
                log_error("status->subs[1]: could not inilize the window");
                return;
            }

            mvwprintw(status->subs[1]->window, 0, 0, "%s", "00:00");
            wnoutrefresh(status->subs[1]->window); // Update the subwindow without immediate refresh

            int pudding = cursor_position_width + 5;
            // Cursor position subwin
            FILL_SUB_WIN_STRUCT(status, 2, 1, cursor_position_width, y, pudding);
            status->subs[2]->window = subwin(status->window, status->subs[2]->coords.height, status->subs[2]->coords.width,
                                             status->subs[2]->coords.start_y, status->subs[2]->coords.start_x);
            if (status->subs[2]->window == NULL) {
                log_error("status->subs[2]: could not inilize the window");
                return;
            }

            mvwprintw(status->subs[2]->window, 0, 0, "ln %d, col %d", 0, 0);
            wnoutrefresh(status->subs[2]->window); // Update the subwindow without immediate refresh

            // Unchanging variables subwin
            int start_x = (((COLS - (time_width + mode_width + pudding)) / 2) - (unchanging_variables_width / 2)) + (mode_width + pudding);
            FILL_SUB_WIN_STRUCT(status, 3, 1, unchanging_variables_width, y, start_x);
            status->subs[3]->window = subwin(status->window, status->subs[3]->coords.height, status->subs[3]->coords.width,
                                             status->subs[3]->coords.start_y, status->subs[3]->coords.start_x);
            if (status->subs[3]->window == NULL) {
                log_error("status->subs[3]: could not inilize the window");
                return;
            }

            mvwprintw(status->subs[3]->window, 0, 0, "  Tab_space %d | %s | %s  ", 4, "ASCII", "TXT");
            wnoutrefresh(status->subs[3]->window); // Update the subwindow without immediate refresh
        }
    } while (0); // This loop executes only once

    wnoutrefresh(status->window); // Update the parent window without immediate refresh
    doupdate(); // Refresh the screen with all updates

    wattroff(status->window, COLOR_PAIR(7));
}

PRIVATE void display_setup(Window *windows){
    title_setup(&windows[TITLE]);
    sidebar_setup(&windows[SIDE], &windows[SCROOL], cmd_info->display_line_numbers);
    status_setup(&windows[STATUS]);
    command_setup(&windows[CMD]);
    wbkgd(windows[MAIN].window, COLOR_PAIR(6));
    wrefresh(windows[MAIN].window);

    return;

}

void setup_screen(Window *windows, bool display_line_numbers, int win_num) {
    for (int i = 0; i < win_num; i++) {
        FILL_WIN_STRUCT(&windows[i], i, 0, 0, 0, 0);
    }

    FILL_WIN_STRUCT(&windows[TITLE], 0, 1, COLS, 0, 0);                      // TITLE
    FILL_WIN_STRUCT(&windows[STATUS], 1, 1, COLS, LINES - 2, 0);              // STATUS
    FILL_WIN_STRUCT(&windows[CMD], 2, 1, COLS, LINES - 1 , 0);             // CMD

    if (display_line_numbers == true) { 
        FILL_WIN_STRUCT(&windows[SCROOL], 3, LINES - 3, 1, 1, 4);             // SCROLL
        FILL_WIN_STRUCT(&windows[MAIN], 4, LINES - 3, COLS - 5, 1, 5);      // MAIN
        FILL_WIN_STRUCT(&windows[SIDE], 5, LINES - 3, 4, 1, 0);             // SIDE
    } else {
        FILL_WIN_STRUCT(&windows[SCROOL], 3,  LINES - 3, 1, 1, 0);             //scrool
        FILL_WIN_STRUCT(&windows[MAIN], 4,LINES - 3, COLS - 1, 1, 1);      // MAIN
    }

    for (int i = 0; i < win_num; i++) {
        windows[i].window = newwin(windows[i].coords.height, windows[i].coords.width,
                                   windows[i].coords.start_y, windows[i].coords.start_x);
        if (windows[i].window == NULL) {
            log_error("could not create the window %d", i);
        }
    }
    return;
} 

PUBLIC int start_display_processes(){
    terminal_start();
    if(!cmd_info->already_active)
        init_config();

    cmd_info->already_active = true;
    setup_color_display( );
    int win_num = WIN_NUM;
    cmd_info->display_line_numbers = true;
    
    if(cmd_info->display_line_numbers == false)
        win_num = WIN_NUM_LESS;


    Window *windows = malloc(win_num * sizeof(Window));
    if (windows == NULL) {
        log_error("Failed to allocate memory for windows");
        return 1;
    }
     windows_info = windows;
    
    setup_screen(windows,cmd_info->display_line_numbers,win_num);
    display_setup(windows);
    return EXIT_SUCCESS;
}


void sig_handler(int sig)
{
  if (SIGWINCH == sig) {
    cmd_info->resize_param = true;
    terminal_stop();
    start_display_processes();
}
}