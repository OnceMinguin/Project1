#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // open file
#include <ctype.h>  // isspace
#include <unistd.h>  // read file

int main(int argc, char **argv) {
    char fname[143];
    //printf("Enter filename :");
    //scanf("%s", fname);

    int maxPerLine = atoi(argv[1]);
    fname = argv[2];
    int fd = open(fname, O_RDONLY);  // Open file
    if (fd == -1) { // File dead
        return EXIT_FAILURE;
    }

    char c;
    char word[30];
    int count = 0, line = 0;
    while (read(fd, &c, 1) != '\0') { // read file
        word[count] = c;    // create word
        if (isspace(c) != 0) { // if space go in here
            line += count;  // total of chars in the line
            if (line > maxPerLine) {// if exceeds limit, go to next line
                printf("\n");
                line = 0;
            }
            for (int i = 0; i < count; i++) { // print word
                printf("%c", word[i]);
            }
            printf("%c", c); // print space
            line++; // add to chars in line to account for space
            count = 0; // reset counter for word
            continue;
        }
        count++;
    }

    return EXIT_SUCCESS;
}