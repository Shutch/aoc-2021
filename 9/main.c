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
    int grid[100][100];

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file
    char ch;
    int x = 0;
    int y = 0;
    int val;
    int xmax;
    int ymax;

    ch = fgetc( fp );
    while( !feof(fp) ) {
        if( ch == "\n"[0] ) {
            y++;
            xmax = x;
            x = 0;
        } else {
            val = (int)ch - 48;  // whatever
            grid[x][y] = val;
            //printf("%d, %d: %c -> %d\n", x, y, ch, val);
            x++;
        }
        ch = fgetc( fp );
    }
    ymax = y;
    //printf("%d, %d\n", xmax, ymax);

    // close file
    fclose(fp);


    // Finding low spots the hard way
    int sumlowspots = 0;
    for( y = 0; y < ymax; y++ ) {
        for( x = 0; x < xmax; x++ ) {
            // checking top edge
            if( y != 0 ) {
                if( grid[x][y-1] <= grid[x][y] ) { continue; }
            }
            // checking bottom edge
            if( y != ymax-1 ) { 
                if( grid[x][y+1] <= grid[x][y] ) { continue; }
            }
            // checking left edge
            if( x != 0 ) {
                if( grid[x-1][y] <= grid[x][y] ) { continue; }
            }
            // checking right edge
            if( x != xmax-1 ) {
                if( grid[x+1][y] <= grid[x][y] ) { continue; }
            }
            // lowest spot
            sumlowspots += grid[x][y] + 1;
            //printf("Low Spot: %d, %d, %d %d\n", x, y, grid[x][y]+1, sumlowspots);
        }
    }

    ans = sumlowspots;
    return ans;
}

int compar( const void * a, const void * b ) {
    return ( *(int*)a < *(int*)b );
}

int parttwo (char *filename) {
    int ans = -1;
    int grid[100][100];

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file
    char ch;
    int x = 0;
    int y = 0;
    int val;
    int xmax;
    int ymax;

    ch = fgetc( fp );
    while( !feof(fp) ) {
        if( ch == "\n"[0] ) {
            y++;
            xmax = x;
            x = 0;
        } else {
            val = (int)ch - 48;  // whatever
            grid[x][y] = val;
            //printf("%d, %d: %c -> %d\n", x, y, ch, val);
            x++;
        }
        ch = fgetc( fp );
    }
    ymax = y;
    //printf("%d, %d\n", xmax, ymax);

    // close file
    fclose(fp);


    // Finding low spots the hard way
    int basinsize;
    int tosearch[1000][2];
    int searchstart;
    int searchend;
    int bx, by;
    int numbasins = 0;
    int basins[1000];

    for( y = 0; y < ymax; y++ ) {
        for( x = 0; x < xmax; x++ ) {
            // checking top edge
            if( y != 0 ) {
                if( grid[x][y-1] <= grid[x][y] ) { continue; }
            }
            // checking bottom edge
            if( y != ymax-1 ) { 
                if( grid[x][y+1] <= grid[x][y] ) { continue; }
            }
            // checking left edge
            if( x != 0 ) {
                if( grid[x-1][y] <= grid[x][y] ) { continue; }
            }
            // checking right edge
            if( x != xmax-1 ) {
                if( grid[x+1][y] <= grid[x][y] ) { continue; }
            }
            // lowest spot, bfs through the basin
            // To mark a spot marked, we subtract 10 to make it negative for a quick check
            basinsize = 0;
            searchstart = 0;
            searchend = 1;
            bx = x; by = y;
            grid[bx][by] -= 10;
            tosearch[0][0] = bx; tosearch[0][y] = by;

            //printf("Found basin at %d, %d\n", bx, by);
            while( searchstart-searchend != 0 ) {
                if( by != 0 && grid[bx][by-1] != 9 && grid[bx][by-1] > -1 ) { // top
                    //printf("adding %d at %d, %d (%d)\n", grid[bx][by-1], bx, by-1, searchend);
                    tosearch[searchend][0] = bx;
                    tosearch[searchend][1] = by-1;
                    grid[bx][by-1] -= 10;
                    searchend++;
                }
                if( by != ymax-1 && grid[bx][by+1] != 9 && grid[bx][by+1] > -1 ) { // bottom
                    //printf("adding %d at %d, %d (%d)\n", grid[bx][by+1], bx, by+1, searchend);
                    tosearch[searchend][0] = bx;
                    tosearch[searchend][1] = by+1;
                    grid[bx][by+1] -= 10;
                    searchend++;
                }
                if( bx != 0 && grid[bx-1][by] != 9 && grid[bx-1][by] > -1 ) { // left
                    //printf("adding %d at %d, %d (%d)\n", grid[bx-1][by], bx-1, by, searchend);
                    tosearch[searchend][0] = bx-1;
                    tosearch[searchend][1] = by;
                    grid[bx-1][by] -= 10;
                    searchend++;
                }
                if( bx != xmax-1 && grid[bx+1][by] != 9 && grid[bx+1][by] > -1 ) { // right
                    //printf("adding %d at (%d, %d (%d)\n", grid[bx+1][by], bx+1, by, searchend);
                    tosearch[searchend][0] = bx+1;
                    tosearch[searchend][1] = by;
                    grid[bx+1][by] -= 10;
                    searchend++;
                }
                basinsize++;
                searchstart++;
                bx = tosearch[searchstart][0];
                by = tosearch[searchstart][1];
            }
            printf("Basin size %d at %d, %d\n", basinsize, x, y);
            basins[numbasins] = basinsize;
            numbasins++;
        }
    }
    printf("Total basins: %d\n", numbasins);
    // finding largest 3

    qsort(basins, numbasins, sizeof(int), compar);
    ans = basins[0] * basins[1] * basins[2];
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
