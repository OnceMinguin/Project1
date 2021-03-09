#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // open file
#include <ctype.h>  // isspace
#include <unistd.h>  // read file
#include <sys/types.h>
//#include <dirent.h>
//#include <errno.h>

int printword(int line, int count, int maxPerLine, char* word, char c) {
    line += count;  // total of chars in the line
    if (line > maxPerLine) {// if exceeds limit, go to next line
        printf("\n");
        line = count;
    }
    for (int i = 0; i < count; i++) { // print word
        printf("%c", word[i]);
    }
    if(c != '\n' && isspace(c) != 0)
        printf("%c", c); // print space character
    else if (isspace(c) != 0)
        printf(" ");
    return (line++); // add to chars in line to account for space
}

int main(int argc, char **argv) {
    char fname[143];
    //printf("Enter filename :");
    //scanf("%s", fname);

    int maxPerLine = atoi(argv[1]);
    strcpy(fname, argv[2]);
    int fd = open(fname, O_RDONLY);  // Open file
    if (fd == -1) { // File dead
        return EXIT_FAILURE;
    }
    DIR* directory = opendir(fname);
    if (directory == NULL){
        return EXIT_FAILURE;
    }

    char c;
    char word[30];
    int count = 0, line = 0;
    while (read(fd, &c, 1) != '\0') { // read file
        if (count == 0 && c == '\n') { // retains empty line
            line = 0;
            printf("\n\n");
            continue;
        }
        word[count] = c;    // create word
        if (isspace(c) != 0) { // if space go in here
            line = printword(line, count, maxPerLine, word, c);
            count = 0; // reset counter for word
            continue;
        }
        count++;
    }
    line = printword(line, count, maxPerLine, word, c);

    return EXIT_SUCCESS;
}

// make sure we print last word based on parameter whether that be with the line before or on a new line after DONE
// when combining lines, need to make sure we add a space since we are getting rid of the \n DONE
// somehow keep track of full line breaks and retain them DONE