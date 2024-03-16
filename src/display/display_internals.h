
#define WIN_NUM 6
#define WIN_NUM_LESS 5
#define MAX_LINE_DIGITS 4


#define FILL_WIN_STRUCT(win, vid, h, w, y, x) \
    do { \
        (win)->has_subs = false; \
        (win)->id = (vid); \
        (win)->num_subs = 0; \
        (win)->subs = NULL; \
        (win)->coords.height = (h); \
        (win)->coords.width = (w); \
        (win)->coords.start_y = (y); \
        (win)->coords.start_x = (x); \
    } while (0)

#define FILL_SUB_WIN_STRUCT(win, id, h, w, y, x) \
do { \
    (win)->subs[(id)]->coords.height = (h); \
    (win)->subs[(id)]->coords.width = (w); \
    (win)->subs[(id)]->coords.start_y = (y); \
    (win)->subs[(id)]->coords.start_x = (x); \
} while (0)


struct {
    const char *color;
    const char *code;
}color_codes[] = {
    {"black", "#(0,0,0)"},
    {"red", "#(255,0,0)"},
    {"green", "#(0,255,0))"},
    {"yellow", "#(255,255,0)"},
    {"blue", "#(0,0,255)"},
    {"magenta", "#(255,0,255)"},
    {"cyan", "#(0,255,255)"},
    {"white", "#(255,255,255)"}
    };
        
        // (win)->subs[(id)] = malloc(sizeof(SubWin)); \
        // if ((win)->subs[(id)] == NULL) { \
        //     log_error("status->subs[%d]: could not inilize the window",id); \
        // } \

