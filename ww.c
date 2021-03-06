#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

int main(int argc, char **argv) {
    char fname[143];
    printf("Enter filename :");
    scanf("%s", fname);
    int fd = open(fname, O_RDONLY);  // Open file
    if (fd == -1) { // File dead
        return EXIT_FAILURE;
    }

    char c;
    while (read(fname, &c, 1) != '\0') {
        printf("%c", c);
        if (isspace(c) == 0)
            continue;
    }

    return EXIT_SUCCESS;
}