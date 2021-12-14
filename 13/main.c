#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include "helpers.h"

int partone (char *filename) {
    int ans = -1;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file
    int linesize = 20;
    char line[linesize];
    static int dots[1000][2] = { };
    static int grid[1500][1500] = { };
    int numdots = 0;
    int x, y;

    // parsing coordinates
    fgets(line, linesize, fp);
    while( strcmp(line, "\n") != 0 ) {
        sscanf( line, "%d,%d\n", &x, &y);
        dots[numdots][0] = x; dots[numdots][1] = y;
        grid[x][y] = 1;
        numdots++;
        fgets(line, linesize, fp);
        //printf("New point added at %d, %d\n", x, y);
    }

    // parsing folds
    char axis;
    int folds[20][2]; // fold axis is first value. x = 0, y = 1. Value is 2nd
    int numfolds = 0;
    while( fgets(line, linesize, fp) != NULL ) {
        // throw away 'Fold line: fold along '
        sscanf( line, "fold along %c=%d\n", &axis, &folds[numfolds][1]);
        // ascii x = 120, y = 121
        folds[numfolds][0] = axis - 120;
        //printf("Fold line: %d along %d\n", folds[numfolds][1], folds[numfolds][0]);
        numfolds++;
    }

    // close file
    fclose(fp);

    // folding the first fold for part 1
    int origx, origy, flippedx, flippedy, foldline;
    int dotsshown = numdots;
    for( int f = 0; f < 1; f++ ) {
        if( folds[f][0] == 0 ) {  // x axis fold
            foldline = folds[f][1];
            //printf("Folding at %d along x axis\n", foldline);
            for( int i = 0; i < numdots; i++ ) {
                origx = dots[i][0]; origy = dots[i][1];
                if( origx > foldline ) {  // check if flip is valid (on the right side of the flip line)
                    flippedx = foldline - (origx - foldline);
                    if( grid[flippedx][origy] > 0 ) {
                        dotsshown--; 
                        //printf("Point %d, %d overlaps, total dots shown: %d\n", flippedx, origy, dotsshown);
                    }
                    grid[flippedx][origy]++;
                    grid[origx][origy] = 0;
                    dots[i][0] = flippedx;
                    //printf("Dot flipped from %d, %d to %d, %d. Current value: %d\n", origx, origy, flippedx, origy, grid[flippedx][origy]);
                }
            }
        } else {  // y axis fold
            foldline = folds[f][1];
            //printf("Folding at %d along y axis\n", foldline);
            for( int i = 0; i < numdots; i++ ) {
                origx = dots[i][0]; origy = dots[i][1];
                if( origy > foldline ) {  // check if flip is valid (on the bottom side of the flip line)
                    flippedy = foldline - (origy - foldline);
                    if( grid[origx][flippedy] > 0 ) {
                        dotsshown--; 
                        //printf("Point %d, %d overlaps, total dots shown: %d\n", origx, flippedy, dotsshown);
                    }
                    grid[origx][flippedy]++;
                    grid[origx][origy] = 0;
                    dots[i][1] = flippedy;
                    //printf("Dot flipped from %d, %d to %d, %d. Current value: %d\n", origx, origy, origx, flippedy, grid[origx][flippedy]);
                }
            }
        }
    }
    ans = dotsshown;
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
    int linesize = 20;
    char line[linesize];
    static int dots[1000][2] = { };
    static int grid[1500][1500] = { };
    int numdots = 0;
    int x, y;

    // parsing coordinates
    fgets(line, linesize, fp);
    while( strcmp(line, "\n") != 0 ) {
        sscanf( line, "%d,%d\n", &x, &y);
        dots[numdots][0] = x; dots[numdots][1] = y;
        grid[x][y] = 1;
        numdots++;
        fgets(line, linesize, fp);
        //printf("New point added at %d, %d\n", x, y);
    }

    // parsing folds
    char axis;
    int folds[20][2]; // fold axis is first value. x = 0, y = 1. Value is 2nd
    int numfolds = 0;
    while( fgets(line, linesize, fp) != NULL ) {
        // throw away 'Fold line: fold along '
        sscanf( line, "fold along %c=%d\n", &axis, &folds[numfolds][1]);
        // ascii x = 120, y = 121
        folds[numfolds][0] = axis - 120;
        //printf("Fold line: %d along %d\n", folds[numfolds][1], folds[numfolds][0]);
        numfolds++;
    }

    // close file
    fclose(fp);

    // folding the first fold for part 1
    int origx, origy, flippedx, flippedy, foldline;
    int dotsshown = numdots;
    for( int f = 0; f < numfolds; f++ ) {
        if( folds[f][0] == 0 ) {  // x axis fold
            foldline = folds[f][1];
            //printf("Folding at %d along x axis\n", foldline);
            for( int i = 0; i < numdots; i++ ) {
                origx = dots[i][0]; origy = dots[i][1];
                if( origx > foldline ) {  // check if flip is valid (on the right side of the flip line)
                    flippedx = foldline - (origx - foldline);
                    if( grid[flippedx][origy] > 0 ) {
                        dotsshown--; 
                        //printf("Point %d, %d overlaps, total dots shown: %d\n", flippedx, origy, dotsshown);
                    }
                    grid[flippedx][origy]++;
                    grid[origx][origy] = 0;
                    dots[i][0] = flippedx;
                    //printf("Dot flipped from %d, %d to %d, %d. Current value: %d\n", origx, origy, flippedx, origy, grid[flippedx][origy]);
                }
            }
        } else {  // y axis fold
            foldline = folds[f][1];
            //printf("Folding at %d along y axis\n", foldline);
            for( int i = 0; i < numdots; i++ ) {
                origx = dots[i][0]; origy = dots[i][1];
                if( origy > foldline ) {  // check if flip is valid (on the bottom side of the flip line)
                    flippedy = foldline - (origy - foldline);
                    if( grid[origx][flippedy] > 0 ) {
                        dotsshown--; 
                        //printf("Point %d, %d overlaps, total dots shown: %d\n", origx, flippedy, dotsshown);
                    }
                    grid[origx][flippedy]++;
                    grid[origx][origy] = 0;
                    dots[i][1] = flippedy;
                    //printf("Dot flipped from %d, %d to %d, %d. Current value: %d\n", origx, origy, origx, flippedy, grid[origx][flippedy]);
                }
            }
        }
    }

    // answer must be printed and read
    for( int x = 0; x < 7; x++ ) {
        for( int y = 0; y < 50; y++ ) {
            char c = (grid[y][x] > 0) ? 'X' : ' ';
            printf("%c", c);
        }
        printf("\n");
    }

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
        filename = "test.txt";
    }

    clock_t start = clock();

    int p1 = partone(filename);
    clock_t p1time = clock();
    printf("Part 1 (%lf): %d\n", (double)(p1time - start) / CLOCKS_PER_SEC, p1);

    int p2 = parttwo(filename);
    clock_t p2time = clock();
    printf("Part 2: (%lf): %d\n", (double)(p2time - start) / CLOCKS_PER_SEC, p2);
}
