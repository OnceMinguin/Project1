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
	if (ftype == 0){	// FOR COMMAND LINE
    	for (int i = 2; i < argc; i++) {
        	strcpy(word, argv[i]);
        	line += strlen(word);  // total of chars in the line
        	if (line > maxPerLine) {	// if exceeds limit, go to next line
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
        	word[count] = c;	// create word
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
        	char temp[100];
        	strcpy(temp, fname);
        	strcat(temp, "/");
        	strcat(temp, pDirent->d_name);
        	fd = open(temp, O_RDONLY);
        	if ((memcmp(pDirent->d_name, ".", 1)==0) || (memcmp(pDirent->d_name, "wrap", 4) == 0)){
            	continue;
        	}
        	char temp2[100];
        	strcpy(temp2, fname);
        	strcat(temp2, "/wrap.");
        	strcat(temp2, pDirent->d_name);

    	int newLine = 0;
        	int fd1 = open(temp2, O_WRONLY| O_TRUNC | O_CREAT, 0777);
        	count = 0; line = 0;
        	while (read(fd, &c, 1) != '\0') { // read file
            	if (count == 0 && c == '\n') { // retains empty line
                	newLine++;
                	continue;
            	}
            	if (newLine > 1) {
                	line = 0;
                	write(fd1,"\n\n", 2);
            	}
            	newLine = 0;
            	word[count] = c;	// create word
            	if (isspace(c) != 0) { // if space go in here
                	line += count;  // total of chars in the line
                	if (line > maxPerLine) {// if exceeds limit, go to next line
                    	char mychar = '\n';
                    	char str[2];
                    	str[0] = mychar;
                    	str[1] = '\0';
                    	write(fd1, str, 1);
                    	line = count;
                	}
                	write(fd1, word, count); // print word
                	if(c != '\n' && isspace(c) != 0){
                    	char mychar = c; // print space character
                    	char str[2];
                    	str[0] = mychar;
                    	str[1] = '\0';
                    	write(fd1, str, 1);
                	}
                	else if (isspace(c) != 0){
                    	char mychar = ' ';
                    	char str[2];
                    	str[0] = mychar;
                    	str[1] = '\0';
                    	write(fd1, str, 1);
                    	newLine = 1;
                	}
                	line++; // add to chars in line
                	count = 0; // reset counter for word
                	continue;
            	}
            	count++;
        	}
        	line += count;  // total of chars in the line
        	if (line > maxPerLine) {// if exceeds limit, go to next line
            	char mychar = '\n';
            	char str[2];
            	str[0] = mychar;
            	str[1] = '\0';
            	write(fd1, str, 1);
            	line = count;
        	}
        	write(fd1, word, count); // print word
        	close(fd1);
        	close(fd);
    	}
	}
	closedir(directory);
	return EXIT_SUCCESS;
}