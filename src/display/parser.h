
#define HANDLE_STRING_KEY(key_name, member_name) \
    if (!strcmp(key, key_name)) { \
        char* string_value = parse_value(value); \
        if (string_value) { \
            strcpy(config->member_name, string_value); \
            free(string_value); \
        } \
    }

#define HANDLE_BOOL_KEY(key_name, member_name) \
    if (!strcmp(key, key_name)) { \
        bool* bool_value = parse_value(value); \
        if (bool_value) { \
            config->member_name = *bool_value; \
            free(bool_value); \
        } \
    }

#define HANDLE_INT_KEY(key_name, member_name) \
    if (!strcmp(key, key_name)) { \
        int* int_value = parse_value(value); \
        if (int_value) { \
            config->member_name = *int_value; \
            free(int_value); \
        } \
    }



char sections[][MAX_VALUE_LENGTH] = {
    "global",
    "display"
};

char sub_sections[][MAX_VALUE_LENGTH] = {
    "title",
    "side",
    "main",
    "status",
    "command",
    "display_info",
    "settings"
};

/****config_parse****/


char voom_colors[][MAX_VALUE_LENGTH] = {
    "black",
    "red",
    "green",
    "yellow",
    "blue",
    "magenta",
    "cyan",
    "white"
};


typedef struct{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}rgb;


