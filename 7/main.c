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
    int linesize = 10000;
    char line[linesize];
    char *token;
    int crabs[1000];
    int numcrabs = 0;
    int min = INT_MAX, max = INT_MIN;
    int pos;

    // reading line
    fgets(line, linesize, fp);
    token = strtok(line, ",");
    while( token != NULL ) {
        pos = atoi(token);
        crabs[numcrabs] = pos;
        if( pos < min ) { min = pos; }
        if( pos > max ) { max = pos; }
        token = strtok(NULL, ",");
        numcrabs++;
    }
    //printf("%d %d %d\n", numcrabs, min, max);

    int sum, lowest = INT_MAX, lowestpos = 0;
    for( int i = min; i < max; i++ ) {
        sum = 0;
        for( int j = 0; j < numcrabs; j++ ) {
            sum += abs(crabs[j] - i);
            //printf("%d %d %d %d %d\n", i, j, crabs[j], lowest, sum);
        }
        if( lowest > sum ) { 
            lowest = sum;
            lowestpos = i;
        } else{ break; }
        //printf("%d %d %d %d\n", i, sum, lowest, lowestpos);
    }

    // close file
    fclose(fp);

    //printf("%d @ %d\n", lowest, lowestpos);
    ans = lowest;
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
    int linesize = 10000;
    char line[linesize];
    char *token;
    int crabs[1000];
    int numcrabs = 0;
    int min = INT_MAX, max = INT_MIN;
    int pos;

    // reading line
    fgets(line, linesize, fp);
    token = strtok(line, ",");
    while( token != NULL ) {
        pos = atoi(token);
        crabs[numcrabs] = pos;
        if( pos < min ) { min = pos; }
        if( pos > max ) { max = pos; }
        token = strtok(NULL, ",");
        numcrabs++;
    }
    //printf("%d %d %d\n", numcrabs, min, max);

    int sum, n, lowest = INT_MAX, lowestpos = 0;
    for( int i = min; i < max; i++ ) {
        sum = 0;
        for( int j = 0; j < numcrabs; j++ ) {
            n = abs(crabs[j] - i);
            sum += (n * (n + 1)) / 2;
            //printf("%d %d %d %d %d %d\n", i, j, crabs[j], lowest, n, sum);
        }
        if( lowest > sum ) { 
            lowest = sum;
            lowestpos = i;
        } //else{ break; }
        //printf("%d %d %d %d\n", i, sum, lowest, lowestpos);
    }

    // close file
    fclose(fp);

    //printf("%d @ %d\n", lowest, lowestpos);
    ans = lowest;
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
