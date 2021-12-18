#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include "helpers.h"

enum simresults{ Hit=1, Left=-1, Right=-2, Pass=-3, Null=-10};

enum simresults runsim(int vx, int vy, int xmin, int xmax, int ymin, int ymax) {
    int x = 0, y = 0, t = 0;
    bool hit = false;

    //printf("Starting sim with v: (%d, %d)\n", vx, vy);
    while( (x < xmax) && (y > ymin) ) {
        x += vx;
        y += vy;
        if( vx > 0 ) { vx -= 1; }
        else if ( vx < 0 ) { vx += 1; }
        vy -= 1;
        t++;
        //printf("Time %d, p: (%d, %d), v: (%d, %d)\n", t, x, y, vx, vy);
        // check for hit
        if( (x >= xmin) && (x <= xmax) && (y >= ymin) && (y <= ymax) ) {
            hit = true;
        }
    }
    // determine results 
    enum simresults result;
    if( hit ) { result = Hit; } // hit
    else if( (x <= xmax) && (x >= xmin) && (y < ymin) ) { result = Pass; } // passes through vertically
    else if( (y <= ymax) && (y >= ymin) && (x > xmax) ) { result = Pass; } // passes through horizontally
    else if( (x > xmax) || (y > ymax) ) { result = Right; } // misses high/far
    else if( (x < xmin) || (y < ymin) ) { result = Left; } // misses low/close
    else { fprintf(stderr, "Sim result inconclusive"); result = Null; }
    //printf("Result: %d @ (%d, %d)\n", result, x, y);

    return result;
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
    int xmin, xmax, ymin, ymax;
    fscanf(fp, "target area: x=%d..%d, y=%d..%d", &xmin, &xmax, &ymin, &ymax);
    //printf("X range: [%d, %d], Y range: [%d, %d]\n", xmin, xmax, ymin, ymax);

    // close file
    fclose(fp);

    // minvx is the lowest initial x velocity required to reach the
    // xmin value before x velocity reaches zero
    // minvx is the first positive integer satisfy to n^2 + n - 2*xmin >= 0
    int minvx = 0;
    while( 1 ) {
        if( (minvx * minvx + minvx - 2 * xmin) >= 0 ) { break; }
        minvx++;
    }
    // maxvx is the same process
    int maxvx = minvx;
    while( 1 ) {
        if( (maxvx * maxvx + maxvx - 2 * xmax) >= 0 ) { break; }
        maxvx++;
    }
    maxvx--;
    //printf("Min vx: %d, Max vx: %d\n", minvx, maxvx);
    
    // maxvy is the distance between the starting y (0) and ymin
    // The projectile vy will be -(starting vy) when passing y = 0
    // maxvy is the maximum distance it can cover in one step before completely passing
    // the lower edge of the target area in a single step
    int maxvy = -ymin-1;

    // sanity check, should be Hit
    //enum simresults result;
    //result = runsim(minvx, maxvy, xmin, xmax, ymin, ymax);
    
    // finding max height from maxvy
    // summation of integers from 0 to maxvy = (maxvy^2 + maxvy)/2
    ans = (maxvy*maxvy + maxvy)/2;
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
    int xmin, xmax, ymin, ymax;
    fscanf(fp, "target area: x=%d..%d, y=%d..%d", &xmin, &xmax, &ymin, &ymax);
    //printf("X range: [%d, %d], Y range: [%d, %d]\n", xmin, xmax, ymin, ymax);

    // close file
    fclose(fp);

    // minvx is the lowest initial x velocity required to reach the
    // xmin value before x velocity reaches zero
    // minvx is the first positive integer satisfy to n^2 + n - 2*xmin >= 0
    int minvx = 0;
    while( 1 ) {
        if( (minvx * minvx + minvx - 2 * xmin) >= 0 ) { break; }
        minvx++;
    }

    // maxvx is the distance between the starting x (0) and xmax
    // maxvx is the maximum distance the projectile can cover in one step before
    // completely passing the right edge of the target area in a single step
    int maxvx = xmax;
    //printf("Min vx: %d, Max vx: %d\n", minvx, maxvx);
    
    // maxvy is the distance between the starting y (0) and ymin
    // The projectile vy will be -(starting vy) when passing y = 0
    // maxvy is the maximum distance it can cover in one step before completely passing
    // the lower edge of the target area in a single step
    int maxvy = -ymin;

    enum simresults result;
    int hitsum = 0;

    for( int vx = minvx; vx <= maxvx; vx++ ) {
        for( int vy = -maxvy; vy <= maxvy; vy++ ) {
            result = runsim(vx, vy, xmin, xmax, ymin, ymax);
            if( result == Hit ) { hitsum++; }
        }
    }
    // finding max height from maxvy
    // summation of integers from 0 to maxvy = (maxvy^2 + maxvy)/2

    ans = hitsum;
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
