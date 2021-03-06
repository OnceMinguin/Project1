#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char fname[143];
    printf("Enter filename :");
    gets(fname);
    int fd = open(fname, O_RDONLY | O_CREAT);  // Open file
    if (fd == -1) { // File dead
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}