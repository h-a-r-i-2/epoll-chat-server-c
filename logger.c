#include "common.h"

FILE *log_file;

void log_message(const char *msg) {

    if (!log_file)
        return;

    fprintf(log_file,
            "%s",
            msg);

    fflush(log_file);
}
