#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // open file
#include <ctype.h>  // isspace
#include <unistd.h>  // read file

int main(int argc, char **argv) {
    char fname[143];
    printf("Enter filename :");
    scanf("%s", fname);
    int fd = open(fname, O_RDONLY);  // Open file
    if (fd == -1) { // File dead
        return EXIT_FAILURE;
    }

    char c;
    while (read(fd, &c, 1) != '\0') { // read file
        printf("%c", c);
        if (isspace(c) == 0)
            continue;
    }

    return EXIT_SUCCESS;
}