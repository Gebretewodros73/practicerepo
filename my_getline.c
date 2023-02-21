#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

char *my_getline(void)
{
    static char buffer[BUFFER_SIZE];
    static int pos = 0;
    static int size = 0;

    char *line = NULL;
    int line_pos = 0;
    int line_size = BUFFER_SIZE;

    while (1) {
        // Check if we need to read more data into the buffer
        if (pos >= size) {
            size = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            if (size <= 0) {
                return NULL; // end of file or error
            }
            pos = 0;
        }

        // Copy characters from the buffer to the output line
        char c = buffer[pos++];
        if (c == '\n') {
            break; // end of line
        }
        if (line_pos >= line_size - 1) {
            line_size += BUFFER_SIZE;
            line = realloc(line, line_size);
        }
        line[line_pos++] = c;
    }

    // Add null terminator to the output line
    if (line_pos >= line_size) {
        line_size += 1;
        line = realloc(line, line_size);
    }
    line[line_pos] = '\0';

    return line;
}
