#include "../voom_main.h"
#include "parser.h"

void free_config(config_t* config) {
    if (config != NULL) {
        // Free allocated memory within the config structure
        free(config);
    }
}

int split_line(char* line, char* key, char* value) {
    char* separator = strchr(line, ':');
    if (!separator) {
        return -1; // Invalid line format (missing colon)
    }

    // Extract key (trim whitespace and remove extra spaces)
    char* key_start = line;
    char* key_end = separator - 1; // Include the last non-space character before the colon
    while (isspace(*key_start)) key_start++; // Skip leading spaces
    while (isspace(*key_end)) key_end--; // Skip trailing spaces
    size_t key_len = key_end - key_start + 1;

    // Remove internal spaces in key
    size_t i, j = 0;
    for (i = 0; i < key_len; i++) {
        if (!isspace(key_start[i])) {
            key[j++] = key_start[i];
        }
    }
    key[j] = '\0'; // Add null terminator

    // Extract value (trim whitespace and remove extra spaces)
    char* value_start = separator + 1;
    char* value_end = line + strlen(line) - 1; // Include the last non-space character of the line
    while (isspace(*value_end)) value_end--; // Skip trailing spaces
    size_t value_len = value_end - value_start + 1;

    // Remove internal spaces in value
    j = 0;
    for (i = 0; i < value_len; i++) {
        if (!isspace(value_start[i])) {
            value[j++] = value_start[i];
        }
    }
    value[j] = '\0'; // Add null terminator

    return 0;
}



int is_valid_rgb(const char *str) {
    // Check if the string starts with '#('
    if (str[0] != '#' || str[1] != '(') {
        return 0;
    }

    // Split the string into individual color components
    char *token, *endptr;
    const char *delim = ",)";
    char *str_copy = strdup(str + 2); // Skip the "#(" prefix
    token = strtok(str_copy, delim);

    int r, g, b;
    if (token == NULL || !isdigit(*token)) {
        free(str_copy);
        return 0;
    }

    // Parse the red component
    r = strtol(token, &endptr, 10);
    if (*endptr != '\0' || r < 0 || r > 255) {
        free(str_copy);
        return 0;
    }

    // Parse the green component
    token = strtok(NULL, delim);
    if (token == NULL || !isdigit(*token)) {
        free(str_copy);
        return 0;
    }
    g = strtol(token, &endptr, 10);
    if (*endptr != '\0' || g < 0 || g > 255) {
        free(str_copy);
        return 0;
    }

    // Parse the blue component
    token = strtok(NULL, delim);
    if (token == NULL || !isdigit(*token)) {
        free(str_copy);
        return 0;
    }
    b = strtol(token, &endptr, 10);
    if (*endptr != '\0' || b < 0 || b > 255) {
        free(str_copy);
        return 0;
    }

    free(str_copy);
    return 1;
}
char* string_value_lowerCase(const char* str) {
    if (str == NULL) {
        return NULL; // Return NULL for a NULL input
    }

    size_t len = strlen(str);
    char* result = malloc(len + 1); // Allocate memory for the new string
    if (result == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }

    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]); // Convert to lowercase
    }
    result[len] = '\0'; // Null-terminate the string

    return result;
}

static char *validate_string(const char *value_arg) {

    char *value = string_value_lowerCase(value_arg);

    if (value[0] == '#') {
        if (!is_valid_rgb(value)) {
            fprintf(stderr, "%s  is an invalid RGB value. Please check the format.\n", value);
            return NULL;
        }
        return strdup(value);
    }

    for (size_t i = 0; i < COLOR_NUM; i++) {
        if (strcmp(value, voom_colors[i]) == 0) {
            return strdup(value);
        }
    }

    fprintf(stderr, "%s: is not a valid color. Check the spelling or supported colors list.\n", value);
    return NULL;
}

static int is_tab(short tab) {
    static const short tab_widths[] = {4, 2, 6, 8};
    if (tab >= 0 && tab < (short)(sizeof(tab_widths) / sizeof(*tab_widths))) {
        return tab_widths[tab];
    }
    return 4;  // Default tab width
}

void* parse_value(const char* value) {
    // Integer check (assuming 'int' member type)
    if (isdigit(*value)) {
        int* int_ptr = malloc(sizeof(int));
        if (int_ptr) {
            *int_ptr = is_tab((short)strtol(value, NULL, 10));
            return int_ptr;
        }
        return NULL;
    }

    // Boolean check (assuming 'bool' member type)
    char lower_value[strlen(value) + 1];
    strcpy(lower_value, value);
    for (char* p = lower_value; *p; ++p) {
        *p = tolower(*p);
    }
    if (!strcmp(lower_value, "true") || !strcmp(lower_value, "yes") || !strcmp(lower_value, "1")) {
        bool* bool_ptr = malloc(sizeof(bool));
        if (bool_ptr) {
            *bool_ptr = true;
            return bool_ptr;
        }
    } else if (!strcmp(lower_value, "false") || !strcmp(lower_value, "no") || !strcmp(lower_value, "0")) {
        bool* bool_ptr = malloc(sizeof(bool));
        if (bool_ptr) {
            *bool_ptr = false;
            return bool_ptr;
        }
    }

    char* string_value = validate_string(value);
    return string_value;
}

int parse_sub_section(FILE* fp, const char* sub_section_name, config_t* config) {
    char line[MAX_LINE_LENGTH];
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];

    while (fgets(line, sizeof(line), fp)) {
        if (isspace(*line) || line[0] == '#') {
            continue; // Skip empty lines, lines starting with whitespace, and comments
        }

        if (line[0] == '}') {
            return 0; // End of section reached
        }

        if (split_line(line, key, value)) {
            fprintf(stderr, "Error parsing line: %s\n", line);
            return -1; // Error parsing the line
        }

        if (!strcmp(sub_section_name, "main")) {
            HANDLE_STRING_KEY("bg", main_bg);
            HANDLE_STRING_KEY("fg", main_fg);
        } else if (!strcmp(sub_section_name, "title")) {
            HANDLE_STRING_KEY("bg", title_bg);
            HANDLE_STRING_KEY("fg", title_fg);
            HANDLE_STRING_KEY("bg_voom", title_bg_voom);
            HANDLE_STRING_KEY("fg_voom", title_fg_voom);
        } else if (!strcmp(sub_section_name, "side")) {
            HANDLE_STRING_KEY("bg", side_bg);
            HANDLE_STRING_KEY("fg", side_fg);
        } else if (!strcmp(sub_section_name, "status")) {
            HANDLE_STRING_KEY("bg", status_bg);
            HANDLE_STRING_KEY("fg", status_fg);
            HANDLE_STRING_KEY("bg_status", status_bg_status);
            HANDLE_STRING_KEY("fg_status", status_fg_status);
        } else if (!strcmp(sub_section_name, "command")) {
            HANDLE_STRING_KEY("bg", command_bg);
            HANDLE_STRING_KEY("fg", command_fg);
            HANDLE_STRING_KEY("bg_command", command_bg_cmd);
            HANDLE_STRING_KEY("fg_command", command_fg_cmd);
        } else if (!strcmp(sub_section_name, "display_info")) {
            HANDLE_BOOL_KEY("display_encoding", display_encoding);
            HANDLE_BOOL_KEY("display_lang_server", display_lang_server);
            HANDLE_BOOL_KEY("display_tabspace", display_tabspace);
            HANDLE_BOOL_KEY("display_line_numbers", display_line_numbers);
            HANDLE_BOOL_KEY("display_cursor_position", display_cursor_position);
            HANDLE_BOOL_KEY("display_time", display_time);
        } else if (!strcmp(sub_section_name, "settings")) {
            HANDLE_INT_KEY("tab_width", tab_width);
            HANDLE_BOOL_KEY("indent_using_spaces", indent_using_spaces);
            HANDLE_BOOL_KEY("highlight_current_line", highlight_current_line);
            HANDLE_BOOL_KEY("text_autowrap", text_autowrap);
            HANDLE_BOOL_KEY("enable_spellcheck", enable_spellcheck);
            HANDLE_BOOL_KEY("enable_autocorrect", enable_autocorrect);
            HANDLE_BOOL_KEY("enable_mouse", enable_mouse);
        } else {
            fprintf(stderr, "Error: Unknown sub-section '%s'\n", sub_section_name);
            return -1;
        }
    }

    return -1; // Unexpected end of file while parsing section
}



int parse_section(FILE* fp,  config_t* config) {
    char line[MAX_LINE_LENGTH];
    int len;
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Skip empty lines and lines starting with whitespace
        if (isspace(*line)) {
            continue;
        }


        // Check for closing brace of the section
        if (line[0] == '}') {
            return 0; // End of section reached
        }
        // Check if the line starts with a subsection name
        if (line[0] == '.') {
            char sub_section_name[MAX_KEY_LENGTH];
            if (sscanf(line + 1, "%[^]]", sub_section_name) != 1) {
                fprintf(stderr, "Error parsing section line: %s\n", line);
                return -1;
            }
            sub_section_name[strcspn(sub_section_name, "\n")] = '\0'; // Remove trailing newline

            len = strlen(sub_section_name);
            sub_section_name[len - 1] = '\0';

            // Find matching subsection in the list
            bool found = false;
            for (int i = 0; i < SUB_SECTION_NUM; ++i) {
                if (!strcmp(sub_sections[i], sub_section_name)) {
                    found = true;
                    if (parse_sub_section(fp, sub_section_name, config) != 0) {
                        return -1;
                    }
                    break;
                }
            }

            if (!found) {
                fprintf(stderr, "Error: '%s' is not a valid subsection\n", sub_section_name);
                return -1;
            }
        } else {
            fprintf(stderr, "Error parsing section line: %s\n", line);
            return -1;
        }
    }

    return -1; // Unexpected end of file while parsing section
}


//function removes comments

FILE* remove_comments(FILE* fp){
    FILE* temp = tmpfile();
    int in_comment = 0;
    char ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '/') {
            in_comment = 1;
        } else if (ch == '\n') {
            in_comment = 0;
        }

        if (!in_comment && ch != ' ' && ch != '\t') {
            fputc(ch, temp);
        }
    }

    fclose(fp);
    fseek(temp, 0, SEEK_SET);
    return temp;
}

// Function to parse the entire configuration file
config_t* parse_config(const char* filename) {
    
    char *file_extension = get_extension(filename);

    if(strcmp(file_extension, CONFIG))
        usage(2);

    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }

    FILE *temp = remove_comments(fp);

    config_t* config = malloc(sizeof(config_t));
    memset(config, 0, sizeof(config_t)); //initilize the whole structure to zeros

    if(config == NULL) {
        fprintf(stderr, "Error allocating memory for config\n");
        fclose(temp);
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    char section_name[MAX_KEY_LENGTH];
    int len;

    while (fgets(line, MAX_LINE_LENGTH, temp) != NULL) {
        if (strlen(line) == 0 || line[0] == '\n') { // Skip empty lines and comments
            continue;
        }

        // Check if the line starts with a section name
        if (line[0] == '#') {
            if (sscanf(line + 1, "%[^]]", section_name) == 1) { // Read section name
                section_name[strcspn(section_name, "\n")] = '\0'; // Remove trailing newline

                    len = strlen(section_name);
                    section_name[len - 1] = '\0';

                    for(short i = 0; i != SECTION_NUM; i++){
                        if(!strcmp(sections[i], section_name)){
                            if (parse_section(temp, config) != 0) {
                                free(config);
                                fclose(temp);
                                return 0;
                            }  
                        }
                    }
            } else {
                fprintf(stderr, "Error parsing section line: %s\n", line);
                free(config);
                fclose(temp);
                return 0;
            }
        } else {
            fprintf(stderr, "Error: Unexpected line outside of section: %s\n", line);
            free(config);
            fclose(temp);
            return 0;
        }
    }

    fclose(temp);
    return config;
}