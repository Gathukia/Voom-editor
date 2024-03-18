#ifndef VOOM_HEADER
#define VOOM_HEADER

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <strings.h>
#include <stdarg.h>
#include<time.h>
#include<ctype.h>
#include <stdbool.h>
#include<signal.h>
#include <pthread.h>
#include<ncurses.h>
#include<setjmp.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <setjmp.h>
#include <limits.h>
#include <wchar.h>
#include<ncursesw/ncurses.h>
#include "properties.h"

#ifndef PRIVATE
# define EXTERN extern
#define PRIVATE static  /* PRIVATE x limits the scope of x */
#define PUBLIC 
#define EXIT_SUCCESS 0
# define INIT(x)
#endif

/*** debugging **/

#ifndef LOG_H
#define LOG_H


#ifdef NDEBUG
#define log_error(...)
#define log_warning(...)
#define log_info(...)
#define log_debug(...)
#else
#define log_error(fmt, ...) log_message(__FILE__, __LINE__, LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) log_message(__FILE__, __LINE__, LOG_LEVEL_WARNING, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) log_message(__FILE__, __LINE__, LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define log_debug(fmt, ...) log_message(__FILE__, __LINE__, LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#endif



#endif /* LOG_H */

#define IN_RANGE(x, min, max)  ((x) >= (min)) && ((x) <= (max))
#define OUT_RANGE(x, min, max) ((x) < (min)) || ((x) > (max))
#define arg_max 10
#define arg_length 255

#define MAXIMUM_LENGTH 128
#define SMALL_BUF 30
#define INITIAL_LINE_CAPACITY 128
#define LINE_CAPACITY_MULTIPLIER 2

#ifdef VOOM_SECTION
/* global variables */
static char voom[5][50] = {"_ _    _ _ _ __    _________      ___",
        "\\  \\  /  //    \\  /    \\ |  \\    /   |",
        " \\  \\/  //  __  \\/  __  \\|   \\></    |",
        "  \\    / \\ (__) /\\ (__) /|  *\\   *\\  |",
        "   \\__/   \\_ __/  \\____/ |_/  \\_/  \\_|"};

#endif

/* structs */
typedef struct{
    bool any_files;
    bool text_mode;
    bool binary_mode;
    bool read_only;
    bool has_colors;
    bool supports_UTF8; //true if terminal has utf8 support
    bool resize_param;
    bool already_active;
    short tab_width;
    bool text_autowrap;
    bool display_encoding;
    bool display_lang_server;
    bool display_tabspace;
    bool display_line_numbers;
    bool display_cursor_position;
    bool indent_using_spaces;
    bool highlight_current_line;
    bool enable_spellcheck;
    bool enable_autocorrect;
    bool enable_mouse;
    bool display_time;
    int default_file;  //file to work on first!!
    int file_no;      //number of valid files passed
    char *mode;
    char file_names[arg_max][arg_length]; //valid filenames passed
}INFO;


typedef struct{
	uint cur_y;
	uint cur_x;
}Pos;


typedef struct{
      Pos *cursor_line_position;
	  Pos *cursor_home_position;
	  Pos *cursor_end_position;
      char *encoding;   //support only ascii and utf-8
      char *lang_server; //supports TXT,C and ASM language servers...not yet implemented!!

}Status; //passed to the status window


typedef struct LineNode {
    uint32_t capacity;
    uint32_t length;
    Pos current;
    char *content;
} LineNode;

typedef struct Buffer {
    uint8_t file_id;
    bool modified;
    bool read_only;
    uint32_t line_count;
    Status *Status_info;
    LineNode *lines;
    char *filename;
} Buffer;

typedef struct Handle {
    Buffer data;
    struct Handle *next;
    struct Handle *prev;
} Handle;

/**voom_main.c **/
void usage(int err);

/* file_ops.c */

bool file_exists(const char *filename);
Buffer init_buffer(uint8_t id, bool read_only, const char *filename);
Buffer read_file_to_buffer(const char *filename, bool read_only, uint8_t id);


/** line.c **/

LineNode *init_line(void);
void free_line(LineNode *line);

/** buffer_list.c **/

Handle *create_buffer_list(Buffer buf);
void insert_after(Handle *node, Buffer buf);
void free_buffer_list(Handle *head);
void remove_node(Handle *node);

/** config_parse.c  **/
#define MAX_LINE_LENGTH 128
#define MAX_KEY_LENGTH 32
#define MAX_VALUE_LENGTH 64
#define SECTION_NUM 2
#define SUB_SECTION_NUM 7
#define COLOR_NUM 8
#define COMMENT "//"
        
#define NAME_LEN 100
#define CONFIG ".config"


// Structure to store configuration options
typedef struct {
    short tab_width;

    bool text_autowrap;
    bool display_encoding;
    bool display_lang_server;
    bool display_tabspace;
    bool display_line_numbers;
    bool display_cursor_position;
    bool indent_using_spaces;
    bool highlight_current_line;
    bool enable_spellcheck;
    bool enable_autocorrect;
    bool enable_mouse;
    bool display_time;

    char title_bg[MAX_VALUE_LENGTH];
    char title_fg[MAX_VALUE_LENGTH];
    char title_bg_voom[MAX_VALUE_LENGTH];
    char title_fg_voom[MAX_VALUE_LENGTH];
    char side_bg[MAX_VALUE_LENGTH];
    char side_fg[MAX_VALUE_LENGTH];
    char command_bg[MAX_VALUE_LENGTH];
    char command_fg[MAX_VALUE_LENGTH];
    char command_bg_cmd[MAX_VALUE_LENGTH];
    char command_fg_cmd[MAX_VALUE_LENGTH];
    char main_bg[MAX_VALUE_LENGTH];
    char main_fg[MAX_VALUE_LENGTH];
    char status_bg[MAX_VALUE_LENGTH];
    char status_fg[MAX_VALUE_LENGTH];
    char status_bg_status[MAX_VALUE_LENGTH];
    char status_fg_status[MAX_VALUE_LENGTH];

} config_t;

// Function prototypes
config_t *parse_config(const char* filename);
void free_config(config_t* config);
int split_line(char* line, char* key, char* value);

/** debug.c **/

#define LOG_FILE "voom.log"
#define MAX_LOG_SIZE (10 * 1024 * 1024) // 10 MB

typedef enum {
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel;

void set_log_level(LogLevel level);
void log_message(const char* file, int line, LogLevel level, const char* format, ...);
void close_log_file();

/** utils.c ***/

char *choose_lang_server(const char *filename);
char *make_path(const char *path);
char *get_extension(const char *filename);
char *get_current_time();
int print_file_to_terminal(const char* filename);
void delay(int milliseconds);
void string_lowerCase(char* str);
int file_capitalToSmall(FILE* temp);
void error(char *key, char *sub_section_name);

/** keypad.c **/

int edit_files(Handle** file_handle);

/** display_ops.c **/
#define TITLE 0
#define STATUS 1
#define CMD 2
#define SCROOL 3
#define MAIN 4
#define SIDE 5



typedef struct {
    size_t height;
    size_t width;
    size_t start_y;
    size_t start_x;
} Coords;

typedef struct {
    Coords coords;
    WINDOW *window;
} SubWin;

typedef struct {
    bool has_subs;
    uint8_t id;
    uint8_t num_subs;
    SubWin **subs;
    Coords coords;
    WINDOW *window;
} Window;

int start_display_processes();
void sig_handler(int sig);
void terminal_stop();
int startVoom();


/** external variables **/
extern LogLevel current_log_level;
extern config_t *config;
extern INFO *cmd_info;
extern Window *windows_info;
#endif