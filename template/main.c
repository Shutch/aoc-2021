#include <stdio.h>
#include <stdlib.h>

int partone () {
    int ans = -1;
    char *filename;
    filename = "test.txt";

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file

    // close file
    fclose(fp);

    return ans;
}

int parttwo () {
    int ans = -1;
    char *filename;
    filename = "test.txt";

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file

    // close file
    fclose(fp);

    return ans;
}

int main () {
    int p1 = partone();
    printf("Part 1: %d\n", p1);
    int p2 = parttwo();
    printf("Part 2: %d\n", p2);
}
