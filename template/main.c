#include <stdio.h>
#include <stdlib.h>

int partone (char *filename) {
    int ans = -1;

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

int parttwo (char *filename) {
    int ans = -1;

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

int main( int argc, char *argv[] ) {
    char *filename;
    if( argc == 2) {
        filename = argv[1];
    }
    else if( argc > 2) {
        fprintf(stderr, "Too many arguments");
        return (-1);
    }
    else {
        filename = "input.txt";
    }
    int p1 = partone(filename);
    printf("Part 1: %d\n", p1);
    int p2 = parttwo(filename);
    printf("Part 2: %d\n", p2);
}
