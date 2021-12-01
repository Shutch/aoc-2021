#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int partone () {
    // open input file
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        fprintf(stderr, "Error opening input.txt");
        return -1;
    }

    // get first line
    char buffer[64];
    int BUFFER_SIZE = 64;
    int lastval, currval;
    int incs = 0;

    // reading first line
    fgets(buffer, BUFFER_SIZE, fp);
    lastval = atoi(buffer);
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        currval = atoi(buffer);
        if (currval > lastval) {
            incs++;
        }
        lastval = currval;
    }

    // close file
    fclose(fp);

    return incs;
}

int parttwo () {
    // open input file
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        fprintf(stderr, "Error opening input.txt");
        return -1;
    }

    // get first line
    char buffer[64];
    int BUFFER_SIZE = 64;
    int lastwin[3], currwin[3];
    int incs = 0;

    // reading first line
    fgets(buffer, BUFFER_SIZE, fp);
    lastval = atoi(buffer);
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        currval = atoi(buffer);
        if (currval > lastval) {
            incs++;
        }
        lastval = currval;
    }

    // close file
    fclose(fp);

    return incs;
}

int main () {
    int p1 = partone();
    printf("Part 1: %d\n", p1);
    int p2 = parttwo();
    printf("Part 2: %d\n", p2);
}

