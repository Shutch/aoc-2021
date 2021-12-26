#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

typedef char Grid[150][150];

void printgrid(Grid grid, int sizes[2]) {
    for( int y = 0; y < sizes[1]; y++ ) {
        for( int x = 0; x < sizes[0]; x++ ) {
            printf("%c", grid[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}
// parses the input into a grid, the chars are preserved
// sizes are the grid sizes along the [x, y] axis
void parseinput(FILE *fp, Grid grid1, Grid grid2, int sizes[2]) {

    int x = 0;
    int y = 0;
    char c = fgetc(fp);
    while( !feof(fp) ) {
        x = 0;
        while( c != '\n' ) {
            grid1[x][y] = c;
            grid2[x][y] = c;
            x++;
            c = fgetc(fp);
        }
        grid1[x][y] = c;
        grid2[x][y] = c;
        y++;
        c = fgetc(fp);
    }
    sizes[0] = x;
    sizes[1] = y;
    return;
}

int partone (char *filename) {
    int ans = -1;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file
    static Grid grid1 = { };
    static Grid (*ptr1) = &grid1;

    static Grid grid2 = { };
    static Grid (*ptr2) = &grid2;

    static Grid (*temp);

    static int sizes[2];
    parseinput(fp, grid1, grid2, sizes);

    // close file
    fclose(fp);

    //printgrid(grid1, sizes);

    int turnmoves = 0;
    int numturns = 0;
    do {
        turnmoves = 0;

        //printf("Turn %d, East\n", numturns);
        //printgrid(*ptr1, sizes);

        // east facing
        for( int y = 0; y < sizes[1]; y++ ) {
            for( int x = 0; x < sizes[0]; x++ ) {
                // check for current space to be east cucumber and blank space to east with wraparound
                if( ((*ptr1)[x][y] == '>') && 
                    (((*ptr1)[(x + 1) % sizes[0]][y]) == '.') ) {
                        (*ptr2)[x][y] = '.';    // backfill with blank
                        (*ptr2)[(x + 1) % sizes[0]][y] = '>';   // move cuce
                        turnmoves++;
                }
            }
        }

        // copy grids
        for( int y = 0; y < sizes[1]; y++ ) {
            for( int x = 0; x < sizes[0]; x++ ) {
                (*ptr1)[x][y] = (*ptr2)[x][y];
            }
        }

        //printf("Turn %d, South\n", numturns);
        //printgrid(*ptr1, sizes);

        // south facing
        for( int y = 0; y < sizes[1]; y++ ) {
            for( int x = 0; x < sizes[0]; x++ ) {
                // check for current space to be south cucumber and blank space to south with wraparound
                if( ((*ptr1)[x][y] == 'v') && 
                    (((*ptr1)[x][(y + 1) % sizes[1]]) == '.') ) {
                        (*ptr2)[x][y] = '.';    // backfill with blank
                        (*ptr2)[x][(y + 1) % sizes[1]] = 'v';   // move cuce
                        turnmoves++;
                }
            }
        }

        // copy grids
        for( int y = 0; y < sizes[1]; y++ ) {
            for( int x = 0; x < sizes[0]; x++ ) {
                (*ptr1)[x][y] = (*ptr2)[x][y];
            }
        }
        
        numturns++;
    } while( turnmoves > 0 );

    ans = numturns;
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
        filename = "test.txt";
    }

    clock_t start = clock();
    int p1 = partone(filename);
    clock_t p1time = clock();
    printf("Part 1 (%lf): %d\n", (double)(p1time - start) / CLOCKS_PER_SEC, p1);

    start = clock();
    int p2 = parttwo(filename);
    clock_t p2time = clock();
    printf("Part 2: (%lf): %d\n", (double)(p2time - start) / CLOCKS_PER_SEC, p2);
}
