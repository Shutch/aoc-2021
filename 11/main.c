#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
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
    char ch;
    int grid[10][10];

    ch = fgetc( fp );
    for( int y = 0; y < 10; y++ ) {
        for( int x = 0; x < 10; x++ ) {
            //printf("%c", ch);
            grid[x][y] = atoi(&ch);
            ch = fgetc( fp );
        }
        ch = fgetc( fp ); // skips newline
        //printf("\nNewline: %c\n", ch);
    }

    // close file
    fclose(fp);

    int totalsteps = 100;
    int numflashes = 0;
    int flashlist[100][2] = { };

    for( int s = 0; s < totalsteps; s++ ) {
        int flashhead = 0;
        int flashtail = 0;
        // increase all values by 1
        for( int y = 0; y < 10; y++ ) {
            for( int x = 0; x < 10; x++ ) {
                grid[x][y]++;
                if( grid[x][y] == 10) {
                    //printf("Initial Prop %d: %d, %d\n", flashtail, x, y);
                    flashlist[flashtail][0] = x; flashlist[flashtail][1] = y;
                    flashtail++;
                }
            }
        }
        
        // propagate flashes 
        int x, y;
        while( flashhead != flashtail ) {
            x = flashlist[flashhead][0]; y = flashlist[flashhead][1];
            //printf("Propogating %d: %d, %d\n", flashhead, x, y);
            // taking the head octopus first, adding to surrounding octos
            // bound checking
            int dxstart = -1, dxend = 1, dystart = -1, dyend = 1;
            if( x == 0 ) { dxstart = 0; }
            if( x == 9 ) { dxend = 0; }
            if( y == 0 ) { dystart = 0; }
            if( y == 9 ) { dyend = 0; }
            for( int dx = dxstart; dx <= dxend; dx++ ) {
                for( int dy = dystart; dy <= dyend; dy++ ) {
                    //printf("%d, %d with %d, %d\n", x, y, dx, dy);
                    if( !(dx == 0 && dy == 0) ) {
                        grid[x+dx][y+dy]++;
                        //printf("Incrementing %d, %d to %d\n", x+dx, y+dy, grid[x+dx][y+dy]);
                        if( grid[x+dx][y+dy] == 10) {
                            //printf("Secondary Prop %d: %d, %d\n", flashtail, x+dx, y+dy);
                            flashlist[flashtail][0] = x+dx; flashlist[flashtail][1] = y+dy;
                            flashtail++;
                        }
                    }
                }
            }
            grid[x][y]++;  // adding 1 to flashhead so it isn't triggered again
            flashhead++;
            numflashes++;
        }
        // set flashes octopuses to 0
        for( int y = 0; y < 10; y++ ) {
            for( int x = 0; x < 10; x++ ) {
                if( grid[x][y] > 9 ) {
                    grid[x][y] = 0;
                }
            }
        }

        //printing full board
        //printf("\nComplete Step %d:\n", s+1);
        //for( int py = 0; py < 10; py++ ) {
        //    for( int px = 0; px < 10; px++ ) {
        //        printf("%d", grid[px][py]);
        //    }
        //    printf("\n");
        //}
    }

    ans = numflashes;

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
    char ch;
    int grid[10][10];

    ch = fgetc( fp );
    for( int y = 0; y < 10; y++ ) {
        for( int x = 0; x < 10; x++ ) {
            //printf("%c", ch);
            grid[x][y] = atoi(&ch);
            ch = fgetc( fp );
        }
        ch = fgetc( fp ); // skips newline
        //printf("\nNewline: %c\n", ch);
    }

    // close file
    fclose(fp);

    int totalsteps = 0;
    int numflashes = 0;
    int flashlist[100][2] = { };

    while( 1 ) {
        int flashhead = 0;
        int flashtail = 0;
        // increase all values by 1
        for( int y = 0; y < 10; y++ ) {
            for( int x = 0; x < 10; x++ ) {
                grid[x][y]++;
                if( grid[x][y] == 10) {
                    //printf("Initial Prop %d: %d, %d\n", flashtail, x, y);
                    flashlist[flashtail][0] = x; flashlist[flashtail][1] = y;
                    flashtail++;
                }
            }
        }
        
        // propagate flashes 
        int x, y;
        while( flashhead != flashtail ) {
            x = flashlist[flashhead][0]; y = flashlist[flashhead][1];
            //printf("Propogating %d: %d, %d\n", flashhead, x, y);
            // taking the head octopus first, adding to surrounding octos
            // bound checking
            int dxstart = -1, dxend = 1, dystart = -1, dyend = 1;
            if( x == 0 ) { dxstart = 0; }
            if( x == 9 ) { dxend = 0; }
            if( y == 0 ) { dystart = 0; }
            if( y == 9 ) { dyend = 0; }
            for( int dx = dxstart; dx <= dxend; dx++ ) {
                for( int dy = dystart; dy <= dyend; dy++ ) {
                    //printf("%d, %d with %d, %d\n", x, y, dx, dy);
                    if( !(dx == 0 && dy == 0) ) {
                        grid[x+dx][y+dy]++;
                        //printf("Incrementing %d, %d to %d\n", x+dx, y+dy, grid[x+dx][y+dy]);
                        if( grid[x+dx][y+dy] == 10) {
                            //printf("Secondary Prop %d: %d, %d\n", flashtail, x+dx, y+dy);
                            flashlist[flashtail][0] = x+dx; flashlist[flashtail][1] = y+dy;
                            flashtail++;
                        }
                    }
                }
            }
            grid[x][y]++;  // adding 1 to flashhead so it isn't triggered again
            flashhead++;
            numflashes++;
        }

        if( flashhead == 100 ) {
            return totalsteps+1;
        }
        // set flashes octopuses to 0
        for( int y = 0; y < 10; y++ ) {
            for( int x = 0; x < 10; x++ ) {
                if( grid[x][y] > 9 ) {
                    grid[x][y] = 0;
                }
            }
        }

        //printing full board
        //printf("\nComplete Step %d:\n", totalsteps+1);
        //for( int py = 0; py < 10; py++ ) {
        //    for( int px = 0; px < 10; px++ ) {
        //        printf("%d", grid[px][py]);
        //    }
        //    printf("\n");
        //}
        totalsteps++;
    }
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
