#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdint.h>
#include "helpers.h"

void dfs(int a, int b, uint64_t *elementcount, int pairs[100][100],
         int depth, int maxdepth, uint64_t pairmemos[100][100][100]) {
    if( depth == maxdepth/2 ) {
        //printf("Memo time %c %c\n", a, b);
        for( int i = 0; i < 100; i++ ) {
            elementcount[i] += pairmemos[a][b][i];
        }
    } else if( depth < maxdepth ) {
        //printf("DFS time %c %c\n", a, b);
        int c = pairs[a][b];
        elementcount[c]++;
        dfs(a, c, elementcount, pairs, depth+1, maxdepth, pairmemos);
        dfs(c, b, elementcount, pairs, depth+1, maxdepth, pairmemos);
    }
}

void precomputedfs(int a, int b, uint64_t *elementcount, int pairs[100][100],
                   int depth, int maxdepth, uint64_t pairmemos[100][100][100]) {
    if( depth < maxdepth ) {
        //printf("DFS time %c %c\n", a, b);
        int c = pairs[a][b];
        elementcount[c]++;
        precomputedfs(a, c, elementcount, pairs, depth+1, maxdepth, pairmemos);
        precomputedfs(c, b, elementcount, pairs, depth+1, maxdepth, pairmemos);
    }
}

uint64_t partone (char *filename) {
    uint64_t ans = -1;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file
    int linesize = 100;
    char line[linesize];
    char *template;
    static uint64_t elementcount[100] = { };
    static char sequence[20] = { };
    int sequencecount = 0;

    // ascii codes A = 65, Z = 90
    fgets(line, linesize, fp);
    template = strtok(line, "\n");
    for( int i = 0; i < strlen(template); i++ ) {
        elementcount[template[i]]++;
        sequence[i] = template[i];
        sequencecount++;
    }

    fgets(line, linesize, fp); // blank line

    // pair insertions
    char a, b, c;  // AB -> C
    static int pairs[100][100] = { };  // lookup table for A and B
    static uint64_t pairmemos[100][100][100] = { }; // memoization table for 20 levels
    static int pairlist[200][2] = { };
    int numpairs = 0;

    while( fgets(line, linesize, fp) != NULL ) {
        sscanf( line, "%c%c -> %c\n", &a, &b, &c);
        pairs[a][b] = c;
        //printf("New pair added: %c and %c -> %c\n", a, b, pairs[a][b]);
        pairlist[numpairs][0] = a;
        pairlist[numpairs][1] = b;
        numpairs++;
    }

    // close file
    fclose(fp);

    // precomputing 5 levels of elementcounts for each pair
    const int maxdepth = 10;
    int precompdepth = maxdepth / 2;
    for( int i = 0; i < numpairs; i++ ) {
        char a = pairlist[i][0];
        char b = pairlist[i][1];
        //printf("Precomputing %d levels of %c and %c\n", precompdepth, a, b);
        precomputedfs(a, b, pairmemos[a][b], pairs, 0, precompdepth, pairmemos);
    }

    // depth first character summing
    for( int s = 0; s < sequencecount-1; s++ ) {
        //printf("Running %d levels of %c and %c\n", maxdepth, a, b);
        dfs(sequence[s], sequence[s+1], elementcount, pairs, 0, maxdepth, pairmemos);
    }

    // Finding least and most element count
    uint64_t min = UINT64_MAX; uint64_t max = 0;
    for( int i = 0; i < 100; i++ ) {
        if( (elementcount[i] < min) && (elementcount[i] != 0) ) {
            min = elementcount[i];
        }
        if( (elementcount[i] > max) ) { 
            max = elementcount[i];
        }
    }

    //printf("Max: %d, Min: %d\n", max, min);
    ans = max-min;
    return ans;
}

uint64_t parttwo (char *filename) {
    uint64_t ans = -1;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file
    int linesize = 100;
    char line[linesize];
    char *template;
    static uint64_t elementcount[100] = { };
    static char sequence[20] = { };
    int sequencecount = 0;

    // ascii codes A = 65, Z = 90
    fgets(line, linesize, fp);
    template = strtok(line, "\n");
    for( int i = 0; i < strlen(template); i++ ) {
        elementcount[template[i]]++;
        sequence[i] = template[i];
        sequencecount++;
    }

    fgets(line, linesize, fp); // blank line

    // pair insertions
    char a, b, c;  // AB -> C
    static int pairs[100][100] = { };  // lookup table for A and B
    static uint64_t pairmemos[100][100][100] = { }; // memoization table for 20 levels
    static int pairlist[200][2];
    int numpairs = 0;

    while( fgets(line, linesize, fp) != NULL ) {
        sscanf( line, "%c%c -> %c\n", &a, &b, &c);
        pairs[a][b] = c;
        //printf("New pair added: %c and %c -> %c\n", a, b, pairs[a][b]);
        pairlist[numpairs][0] = a;
        pairlist[numpairs][1] = b;
        numpairs++;
    }

    // close file
    fclose(fp);

    // precomputing 20 levels of elementcounts for each pair
    const int maxdepth = 40;
    int precompdepth = maxdepth / 2;
    for( int i = 0; i < numpairs; i++ ) {
        char a = pairlist[i][0];
        char b = pairlist[i][1];
        precomputedfs(a, b, pairmemos[a][b], pairs, 0, precompdepth, pairmemos);
    }

    // depth first character summing
    for( int s = 0; s < sequencecount-1; s++ ) {
        //printf("Starting dfs for chars %d and %d\n", s, s+1);
        dfs(sequence[s], sequence[s+1], elementcount, pairs, 0, maxdepth, pairmemos);
    }

    // Finding least and most element count
    uint64_t min = UINT64_MAX; uint64_t max = 0;
    for( int i = 0; i < 100; i++ ) {
        if( (elementcount[i] < min) && (elementcount[i] != 0) ) {
            min = elementcount[i];
        }
        if( (elementcount[i] > max) ) { 
            max = elementcount[i];
        }
    }

    //printf("Max: %d, Min: %d\n", max, min);
    ans = max-min;
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

    uint64_t p1 = partone(filename);
    clock_t p1time = clock();
    printf("Part 1 (%lf): %lu\n", (double)(p1time - start) / CLOCKS_PER_SEC, p1);

    uint64_t p2 = parttwo(filename);
    clock_t p2time = clock();
    printf("Part 2: (%lf): %lu\n", (double)(p2time - start) / CLOCKS_PER_SEC, p2);
}
