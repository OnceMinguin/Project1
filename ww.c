#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // open file
#include <ctype.h>  // isspace
#include <unistd.h>  // read file
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

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
    struct dirent *pDirent;
    char fname[143];
    int ftype = 100;

    int maxPerLine = atoi(argv[1]);
    strcpy(fname, argv[2]);
    int fd = open(fname, O_RDONLY);  // check file
    DIR* directory = opendir(fname); // check dir

    if (argc > 3) {
        ftype = 0; // neither
    } else {
        if (directory != NULL) { // file found
            ftype = 2;
        } else {
            if (fd == -1){
                return EXIT_FAILURE;
            }
            ftype = 1;
        }
    }

    char c;
    char word[30];
    int count = 0, line = 0;
    if (ftype == 0){    // FOR COMMAND LINE
        for (int i = 2; i < argc; i++) {
            strcpy(word, argv[i]);
            line += strlen(word);  // total of chars in the line
            if (line > maxPerLine) {    // if exceeds limit, go to next line
                printf("\n");
                line = strlen(word);
            }
            printf("%s", argv[i]);
            printf(" ");
            line++;
        }
    } else if (ftype == 1) { // FOR FILES
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
    } else if (ftype == 2) {
        while ((pDirent = readdir(directory)) != NULL) {
            printf("[%s]\n", pDirent->d_name);

            char temp[100];
            strcpy(temp, fname);
            strcat(temp, "/");
            strcat(temp, pDirent->d_name)
            fd = open(temp, O_RDONLY);
            if (fd == -1)
                continue;

            char temp2[100];
            strcpy(temp2, fname);
            strcat(temp2, "/wrap.");
            strcat(temp2, pDirent->d_name);
            int fd1 = open(temp2, O_WRONLY | O_CREAT | O_TRUNC, 0777);


            while (read(fd, &c, 1) != '\0') { // read file
                    if (count == 0 && c == '\n') { // retains empty line
                        line = 0;
                        write(fd1,"\n\n", 2);
                        continue;
                    }
                    word[count] = c;    // create word
                    if (isspace(c) != 0) { // if space go in here
                        line += count;  // total of chars in the line
                        if (line > maxPerLine) {// if exceeds limit, go to next line
                            write(fd1, "\n", 1);
                            line = count;
                        }
                        for (int i = 0; i < count; i++) { // print word
                            write(fd1, word[i], 1);
                        }
                        if(c != '\n' && isspace(c) != 0)
                            write(fd1, c, 1); // print space character
                        else if (isspace(c) != 0)
                            write(fd1, " ", 1);
                        line++; // add to chars in line
                        count = 0; // reset counter for word
                        continue;
                    }
                    count++;
                }
                line += count;  // total of chars in the line
                if (line > maxPerLine) {// if exceeds limit, go to next line
                    write(fd1, "\n", 1);
                    line = count;
                }
                for (int i = 0; i < count; i++) { // print word
                    write(fd1, c, 1)
                }
            }
        }
    }
    close(fd);
    close(fd1);
    closedir(directory);
    return EXIT_SUCCESS;
}

// make sure we print last word based on parameter whether that be with the line before or on a new line after DONE
// when combining lines, need to make sure we add a space since we are getting rid of the \n DONE
// somehow keep track of full line breaks and retain them DONE