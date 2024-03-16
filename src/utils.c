#include <regex.h>
#include "voom_main.h"

#define RGB_REGEX "^#[0-9a-fA-F]{3}(?:, [0-9a-fA-F]{3}){1,2}$"

char *make_path(const char *path) {
    char *str = malloc(1000 * sizeof *str);
    snprintf(str, 1000, "%s/%s", getenv("HOME"), path);
    return str;
}




char *choose_lang_server(const char *filename) {
    if (strcmp(filename, "makefile") == 0) {
        return "MAKEFILE";
    }

    const char *extension = get_extension(filename);
    if (!extension) {
        return NULL; // Error handling for missing extension
    }

    // Map extensions to language servers
    static const struct {
        const char *extension;
        const char *lang_server;
    } extensions[] = {
        {".c", "C"},
        {".h", "C"},
        {".asm", "ASM"},
        {".txt", "TXT"},
        {".md", "Markdown"},
        {".config", "CONFIG"}
    };

    for (size_t i = 0; i < sizeof(extensions) / sizeof(extensions[0]); i++) {
        if (strcmp(extension, extensions[i].extension) == 0) {
            return extensions[i].lang_server;
        }
    }

    return "NONE";
}


char *get_extension(const char *filename) {
    const char *dot = strchr(filename, '.');
    if (!dot) {
        return NULL; // No extension found
    }
    
    size_t extension_len = strlen(dot) + 1;
    char *extension = malloc(extension_len * sizeof(char));
    if (!extension) {
        printf("memory allocation failed=> [func: extension]\n");
        exit(0);
        return NULL;
    }
    
    strcpy(extension, dot);
    return extension;
}



int fcopy(FILE* tmp,FILE* Temp){
    char ch = 0;
    while(ch != EOF){
        ch = fgetc(tmp);
        fputc(ch,Temp);
    }
return 0;
}

//converts string characters to lowercase

void string_lowerCase(char* str) {
    size_t len  = strlen(str);
    for (size_t i = 0; i < len; i++) {
        tolower((unsigned char)str[i]); // Convert to lowercase
    }

    return;
}


int file_capitalToSmall(FILE* temp){
    char ch;
    //FILE* tmp = tmpfile();
    while(ch != EOF){
        ch = fgetc(temp);
        if(ch == EOF)break;
        fseek(temp,-1,SEEK_CUR);
        if(ch >= 65 && ch <= 90){
            fputc((ch += 32),temp);
        }else fputc(ch,temp);
        
        fseek(temp,0,SEEK_CUR);;
    }
//Temp = fopen(argv[2],"w+");
// fseek(Temp,0,SEEK_SET);
// rewind(tmp);
// fcopy(tmp,Temp);
return EXIT_SUCCESS;
}

int full_string_length(char *str){
    int length = (strlen(str) + 1);
return length;
}

int string_length(char *str){
    int length = strlen(str);  
return length;
}



void delay(int milliseconds) {
    usleep(milliseconds * 1000); // usleep takes microseconds as input
}

// Print file contents to terminal

int print_file_to_terminal(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Failed to open file %s\n", filename);
        return EXIT_FAILURE;
    }

    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c != ' ') {
            delay(1); // Delay for non-space characters
        }
        putchar(c);
    }

    fclose(file);
    return EXIT_SUCCESS;
}


char *get_current_time() {
    time_t current_time;
    struct tm* time_info;
    char time_str[9];  // Enough space for "HH:MM:SS" and null terminator

    // Get the current time
    current_time = time(NULL);

    // Convert the time_t value to a struct tm
    time_info = localtime(&current_time);

    // Format the time as "HH:MM"
    strftime(time_str, sizeof(time_str), "%H:%M", time_info);

    return time_str;
}

void error(char *key, char *sub_section_name){
    fprintf(stderr, "[%s]: is not a valid keyword in %s\n", key, sub_section_name);
}



