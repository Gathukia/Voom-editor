#include <langinfo.h>
#define VOOM_SECTION
#include "voom_main.h"
INFO session_info;
INFO *cmd_info;
config_t *config;


void voomHelp(){
    printf("starting voom....\n\n");
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("                     VOOM EDITOR                         \n");
    printf("                                     version %f          \n", version);
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    print_file_to_terminal("voomh.txt");
    
exit(0);
}
void voom_version(){
    printf("VOOM version: %f \n\n",version);
    short c = 0;
    while(c != 5){
        printf("%s\n",voom[c]);
        c++;
    }
    printf("\nCompiled on (%s ~%s)\n",compile_Date,compile_Time);
    printf("Environment: %s\n",environment);
    printf("Modified by %s\n",author_m);
    printf("Compiled by %s\n",author_c);

exit(0);
}

int setting_up(INFO *cmd_info, Handle **file_handle){
    char ch;
    start_display_processes();
    ch = wgetch(windows_info[MAIN].window);
    if(cmd_info->any_files == true){
        edit_files(file_handle);
    }else{
        startVoom(&windows_info[MAIN]);
        while(1){
        ch = wgetch(windows_info[MAIN].window);
        if(ch == 'i'){
            edit_files(file_handle);
            break;
        }else if(ch == 'q'){
            terminal_stop();
            exit(0);
        }
    }
    }

return EXIT_SUCCESS;
}



bool append_file(INFO *file_info, const char* filename, uint8_t slot) {
    if (file_exists(filename)) {
        strcpy(cmd_info->file_names[slot], filename);
        if(file_info->any_files == false)
            file_info->file_no = 1;
        else
            file_info->file_no++;

        return true;
        }else {
        printf("Error: Could not open file [%s]: %d\n", filename, errno);
        return false;
    }
}


int main(int argc, char **argv) {
    cmd_info = &session_info;
    cmd_info->supports_UTF8 = false;

    //debugging setup
    set_log_level(LOG_LEVEL_ERROR);

    //load configuration file;
    config = parse_config("config/main.config");
    if(config == NULL)printf("load configuration error");

    /* Check for UTF-8 support by terminal */
    char *codeset = nl_langinfo(CODESET);
    if (strcmp(codeset, "UTF-8") == 0) {
        cmd_info->supports_UTF8 = true;
        printf("Terminal supports UTF-8 encoding.\n");
    } else {
        printf("Terminal does not support UTF-8 encoding. Encoding: %s\n", codeset);
    }

    signal(SIGWINCH, sig_handler); // Handles screen resizing

    cmd_info->already_active = false;
    cmd_info->read_only = false;
    cmd_info->mode = malloc(SMALL_BUF * sizeof(char));
    if (cmd_info->mode == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for mode.\n");
        return 1;
    }
    strncpy(cmd_info->mode, "INSERT", SMALL_BUF - 1);
    cmd_info->mode[SMALL_BUF - 1] = '\0'; // Ensure null termination

    cmd_info->default_file = 0; // File to be opened to the visible screen

    struct stat status = { 0 };
    char *dconfig = make_path(".config/");
    if (dconfig == NULL) {
        fprintf(stderr, "Error: Failed to create path for .config directory.\n");
        return 1;
    }

    if (stat(dconfig, &status) == -1) {
        if (mkdir(dconfig, 0777) == -1) {
            fprintf(stderr, "Error: Failed to create .config directory.\n");
            free(dconfig);
            return 1;
        }
    }

    char *voom_config = make_path(".config/voom/");
    if (voom_config == NULL) {
        fprintf(stderr, "Error: Failed to create path for .config/voom directory.\n");
        free(dconfig);
        return 1;
    }

    if (stat(voom_config, &status) == -1) {
        if (mkdir(voom_config, 0777) == -1) {
            fprintf(stderr, "Error: Failed to create .config/voom directory.\n");
            free(dconfig);
            free(voom_config);
            return 1;
        }
    }

    free(dconfig);
    free(voom_config);

    cmd_info->file_no = 0;

    if (argc < 2) {
        char *filename = make_path(".config/voom/temp");
        if (filename == NULL) {
            fprintf(stderr, "Error: Failed to create path for temporary file.\n");
            return 1;
        }

        Buffer buf = read_file_to_buffer(filename, cmd_info->read_only, 0);
        free(filename);

        Handle *file_handle = create_buffer_list(buf);
        if (file_handle == NULL) {
            fprintf(stderr, "Error: Failed to create buffer list.\n");
            return 1;
        }

        setting_up(cmd_info, &file_handle);
    } else {
        argv++;
        int i = 0;
        while (--argc > 0) {
            char *ptr = *argv++;
            if (*ptr != '-') {
                if (append_file(cmd_info, ptr, i++)) {
                    cmd_info->any_files = true;
                }
                continue;
            }

            // Handle command-line flags
            switch (tolower(*(++ptr))) {
                case 'h':
                    voomHelp();
                    break;
                case 'v':
                    voom_version();
                    break;
                case 'r':
                    cmd_info->read_only = true;
                    break;
                case 't':
                    cmd_info->text_mode = true;
                    break;
                case 'b':
                    cmd_info->binary_mode = true;
                    break;
                case 'l':
                    if (*argv == NULL || !file_exists(*argv)) {
                        printf("File name: %s does not exist\n", *argv ? *argv : "(null)");
                        usage(3);
                        break;
                    }
                    parse_config(*argv);
                    break;
                default:
                    printf("%s is not a valid voom flag\n", *argv);
                    usage(3);
                    break;
            }
        }
    }
Handle *file_handle = NULL;
setting_up(cmd_info, &file_handle);
    // if (cmd_info->any_files) {
    //     Handle *file_handle = NULL;
    //     for (int j = 0, i = 1; j != cmd_info->file_no; j++) {
    //         Buffer buf = read_file_to_buffer(cmd_info->file_names[j], cmd_info->read_only, j);
    //         if (i == 1) {
    //             file_handle = create_buffer_list(buf);
    //             i++;
    //         } else {
    //             insert_after(file_handle, buf);
    //         }
    //     }
    //     setting_up(cmd_info, &file_handle);
    // }

    terminal_stop();
    close_log_file();
    free(cmd_info->mode);
    return 0;
}


void usage(int exit_code)
{
    switch (exit_code) {

        case 1:
            printf("Usage: voom [-r] [-t] [-b]  <filename>\n");
            exit(0);
        case 2:
            printf("Usage: voom [-load]  <filename.config>\n");
            exit(0);
        case 3:
            printf("use voom -h || -help ");
            exit(0);
    
}
}