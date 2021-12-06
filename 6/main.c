#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
    int linesize = 1000;
    char line[linesize];
    char *token;
    // Array index is number of days until reproduction
    // value is pop at that number of days
    int fishes[9] = { };

    fgets(line, linesize, fp);
    token = strtok(line, ",");
    while( token != NULL ) {
        fishes[atoi(token)]++;
        token = strtok(NULL, ",");
    };

    // close file
    fclose(fp);

    // stepping through 80 days

    int reproducers;

    for( int i = 0; i < 80; i++ ) { 
        reproducers = fishes[0];
        for( int j = 1; j <= 8; j++ ) {
            // moving every number down one
            fishes[j-1] = fishes[j];
        }
        fishes[8] = 0;
        // adding numbers for reproduction
        fishes[6] += reproducers;
        fishes[8] += reproducers;
        printf("Day %d: %d, %d, %d, %d, %d, %d, %d, %d, %d\n", i, fishes[0], fishes[1], fishes[2], fishes[3], fishes[4], fishes[5], fishes[6], fishes[7], fishes[8]);
    }
    
    // summing fish numbers
    int fishsum = 0;
    for( int i = 0; i <= 8; i++ ) {
        fishsum += fishes[i];
    }

    ans = fishsum;
    return ans;
}

long unsigned int parttwo (char *filename) {
    long unsigned int ans = -1;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file
    int linesize = 1000;
    char line[linesize];
    char *token;
    // Array index is number of days until reproduction
    // value is pop at that number of days
    long unsigned int fishes[9] = { };

    fgets(line, linesize, fp);
    token = strtok(line, ",");
    while( token != NULL ) {
        fishes[atoi(token)]++;
        token = strtok(NULL, ",");
    };

    // close file
    fclose(fp);

    // stepping through 256 days
    long unsigned int reproducers;
    for( int i = 0; i < 256; i++ ) { 
        reproducers = fishes[0];
        for( int j = 1; j <= 8; j++ ) {
            // moving every number down one
            fishes[j-1] = fishes[j];
        }
        fishes[8] = 0;
        // adding numbers for reproduction
        fishes[6] += reproducers;
        fishes[8] += reproducers;
        printf("Day %u: %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu\n", i, fishes[0], fishes[1], fishes[2], fishes[3], fishes[4], fishes[5], fishes[6], fishes[7], fishes[8]);
    }
    
    // summing fish numbers
    long unsigned int fishsum = 0;
    for( int i = 0; i <= 8; i++ ) {
        fishsum += fishes[i];
    }

    ans = fishsum;
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
    int p1 = partone(filename);
    printf("Part 1: %d\n", p1);
    long unsigned int p2 = parttwo(filename);
    printf("Part 2: %lu\n", p2);
}
