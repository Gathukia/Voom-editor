#include "voom_main.h"


FILE *open_file(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        fprintf(stderr, "Error opening file '%s': %s\n", filename, strerror(errno));
    }
    return fp;
}

bool file_exists(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        fclose(fp);
        return true;
    }
    return false;
}



Buffer init_buffer(uint8_t id, bool read_only, const char *filename) {
    Buffer buf = {0};
    buf.file_id = id;
    buf.modified = false;
    buf.read_only = read_only;
    buf.line_count = 1;
    buf.lines = malloc(sizeof(LineNode));
    if (buf.lines) {
        buf.lines[0] = *init_line();
    } else {
        fprintf(stderr, "Failed to allocate memory\n");
    }
    buf.filename = malloc(strlen(filename) + 1);
    if (buf.filename) {
        strcpy(buf.filename, filename);
    } else {
        fprintf(stderr, "Failed to allocate memory\n");
    }
    return buf;
}

void free_buffer(Buffer *buf) {
    if (buf->lines) {
        for (uint32_t i = 0; i < buf->line_count; i++) {
            free_line(&buf->lines[i]);
        }
        free(buf->lines);
    }
    free(buf->filename);
}

Buffer read_file_to_buffer(const char *filename, bool read_only, uint8_t id) {
    FILE *handle = fopen(filename, "r");
    if (!handle) {
        return init_buffer(id, read_only, filename);
    }

    Buffer buf = init_buffer(id, read_only, filename);
    uint32_t line_count = 0;
    int ch;

    while ((ch = fgetc(handle)) != EOF) {
        if (ch == '\n') {
            buf.lines = realloc(buf.lines, (line_count + 1) * sizeof(LineNode));
            if (buf.lines) {
                buf.lines[line_count] = *init_line();
                line_count++;
            } else {
                fprintf(stderr, "Failed to allocate memory\n");
                break;
            }
            continue;
        }

        LineNode *current_line = &buf.lines[line_count];
        if (current_line->length >= current_line->capacity) {
            current_line->capacity *= LINE_CAPACITY_MULTIPLIER;
            current_line->content = realloc(current_line->content, current_line->capacity * sizeof(char));
        }
        current_line->content[current_line->length++] = ch;
    }

    buf.line_count = line_count;
    fclose(handle);
    return buf;
}
