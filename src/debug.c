
#include "voom_main.h"


static const char* log_level_strings[] = {
    "ERROR",
    "WARNING",
    "INFO",
    "DEBUG"
};
LogLevel current_log_level = LOG_LEVEL_INFO;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool log_file_opened = false;
static FILE* log_file = NULL;
static size_t log_file_size = 0;

void set_log_level(LogLevel level) {
    pthread_mutex_lock(&log_mutex);
    printf("debug 1");
    current_log_level = level;
    pthread_mutex_unlock(&log_mutex);
}

void log_message(const char* file, int line, LogLevel level, const char* format, ...) {
    pthread_mutex_lock(&log_mutex);
    printf("debug 2");
    if (level < current_log_level) {
         printf("debug 3");
        pthread_mutex_unlock(&log_mutex);
        return;
    }
     printf("debug 4");

    if (!log_file_opened) {
         printf("debug 2");
        log_file = fopen(LOG_FILE, "w");
        printf("opened file %s\n",LOG_FILE);
        if (log_file == NULL) {
            pthread_mutex_unlock(&log_mutex);
            printf("w2 opened file %s\n",LOG_FILE);
            return;
        }
        log_file_opened = true;
    }

    if (log_file_size >= MAX_LOG_SIZE) {
        fclose(log_file);
        log_file = fopen(LOG_FILE, "w");
        if (log_file == NULL) {
            pthread_mutex_unlock(&log_mutex);
            return;
        }
        log_file_size = 0;
    }

    time_t current_time;
    time(&current_time);
    struct tm* time_info = localtime(&current_time);

    fprintf(log_file, "[%s %02d:%02d:%02d] [%s:%d] %s: ",
            log_level_strings[level],
            time_info->tm_hour, time_info->tm_min, time_info->tm_sec,
            file, line,
            level == LOG_LEVEL_ERROR ? "ERROR" : "");

    va_list args;
    va_start(args, format);
    int written = vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");
    fflush(log_file);

    log_file_size += written + strlen("[TIMESTAMP] [FILE:LINE] MESSAGE\n");

    pthread_mutex_unlock(&log_mutex);
}

void close_log_file() {
    pthread_mutex_lock(&log_mutex);
    if (log_file_opened) {
        fclose(log_file);
        log_file_opened = false;
    }
    pthread_mutex_unlock(&log_mutex);

}