#include <stdio.h>
#include "file_utils.h"

size_t get_file_size(FILE* const fp) {
    size_t current_position = ftell(fp);
    fseek(fp, 0L, SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp, 0, current_position);
    return file_size;
}
