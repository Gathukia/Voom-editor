#include "voom_main.h"

LineNode *init_line(void) {
    LineNode *line = malloc(sizeof(LineNode));
    if (line) {
        line->capacity = INITIAL_LINE_CAPACITY;
        line->length = 0;
        line->current.cur_x = 0;
        line->current.cur_y = 0;
        line->content = calloc(INITIAL_LINE_CAPACITY, sizeof(char));
        *line->content = '\0';
    }
    return line;
}

void free_line(LineNode *line) {
    if (line) {
        free(line->content);
        free(line);
    }
}
