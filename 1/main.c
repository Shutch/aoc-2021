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
    // open input file twice
    char *filename;
    filename = "input.txt";

    FILE *fp1 = fopen(filename, "r");
    if (!fp1) {
        fprintf(stderr, "Error opening %s once\n", filename);
        return -1;
    }
    FILE *fp2 = fopen(filename, "r");
    if (!fp2) {
        fprintf(stderr, "Error opening %s twice\n", filename);
        return -1;
    }

    // get first line and fourth line
    char buffer[64];
    int BUFFER_SIZE = 64;
    int lastval, currval;
    int incs = 0;

    fgets(buffer, BUFFER_SIZE, fp1);
    lastval = atoi(buffer);
    fgets(buffer, BUFFER_SIZE, fp2);
    fgets(buffer, BUFFER_SIZE, fp2);
    fgets(buffer, BUFFER_SIZE, fp2);
    fgets(buffer, BUFFER_SIZE, fp2);
    currval = atoi(buffer);
    while (fgets(buffer, BUFFER_SIZE, fp2) != NULL) {
        if (currval > lastval) {
            incs++;
        }
        currval = atoi(buffer);
        fgets(buffer, BUFFER_SIZE, fp1);
        lastval = atoi(buffer);
    }
    // last check
    if (currval > lastval) {
        incs++;
    }

    // close files
    fclose(fp1);
    fclose(fp2);

    return incs;
}

int main () {
    int p1 = partone();
    printf("Part 1: %d\n", p1);
    int p2 = parttwo();
    printf("Part 2: %d\n", p2);
}

