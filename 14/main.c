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
    int linesize = 100;
    char line[linesize];
    char *template;
    static int elementcount[100] = { };
    static char oldsequence[20000] = { };
    int oldsequencecount = 0;

    // ascii codes A = 65, Z = 90
    fgets(line, linesize, fp);
    template = strtok(line, "\n");
    for( int i = 0; i < strlen(template); i++ ) {
        elementcount[template[i]]++;
        oldsequence[i] = template[i];
        oldsequencecount++;
    }
    //printf("Template: %s\n", template);

    //int maxpolymerlength = strlen(template);
    //for( int i = 1; i <= numsteps; i++ ) {
    //    printf("Max length at step %d: %d\n", i, maxpolymerlength);
    //    maxpolymerlength += (maxpolymerlength - 1);
    //}

    fgets(line, linesize, fp); // blank line

    // pair insertions
    char a, b, c;  // AB -> C
    static char pairs[100][100] = { };  // lookup table for A and B

    while( fgets(line, linesize, fp) != NULL ) {
        sscanf( line, "%c%c -> %c\n", &a, &b, &c);
        pairs[a][b] = c;
        //printf("New pair added: %c and %c -> %c\n", a, b, pairs[a][b]);
    }

    // close file
    fclose(fp);

    static char newsequence[20000] = { };
    int newsequencecount = 0;
    char insert;
    char *oldptr = oldsequence;
    char *newptr = newsequence;

    int numsteps = 10;
    for( int s = 1; s <= numsteps; s++ ) {
        //printf("Starting step %d\n", s);
        for( int i = 1; i < oldsequencecount; i++ ) {
            insert = pairs[oldptr[i-1]][oldptr[i]];
            if( insert != 0 ) {
                newptr[newsequencecount] = oldptr[i-1]; newsequencecount++;
                newptr[newsequencecount] = insert; newsequencecount++;
                elementcount[insert]++;
                newptr[newsequencecount] = oldptr[i];
            } else { fprintf(stderr, "Implement no sequence found logic.\n"); }
        }
        oldsequencecount = newsequencecount + 1;
        //printf("Sequence length after step %d: %d\n", s, oldsequencecount);
        newsequencecount = 0;
        char *temp = oldptr;
        oldptr = newptr;
        newptr = temp;
    }

    // Finding least and most element count
    int min = INT_MAX; int max = INT_MIN;
    for( int i = 0; i <= 100; i++ ) {
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

int parttwo (char *filename) {
    int ans = -1;

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
    static int elementcount[100] = { };
    static char oldsequence[20000] = { };
    int oldsequencecount = 0;

    // ascii codes A = 65, Z = 90
    fgets(line, linesize, fp);
    template = strtok(line, "\n");
    for( int i = 0; i < strlen(template); i++ ) {
        elementcount[template[i]]++;
        oldsequence[i] = template[i];
        oldsequencecount++;
    }
    //printf("Template: %s\n", template);

    long long maxpolymerlength = strlen(template);
    maxpolymerlength = 2;
    for( int i = 0; i <= 40; i++ ) {
        printf("Max length at step %d: %lld\n", i, maxpolymerlength);
        maxpolymerlength += (maxpolymerlength - 1);
    }

    fgets(line, linesize, fp); // blank line

    // pair insertions
    char a, b, c;  // AB -> C
    static char pairs[100][100] = { };  // lookup table for A and B

    while( fgets(line, linesize, fp) != NULL ) {
        sscanf( line, "%c%c -> %c\n", &a, &b, &c);
        pairs[a][b] = c;
        //printf("New pair added: %c and %c -> %c\n", a, b, pairs[a][b]);
    }

    // close file
    fclose(fp);

    static char newsequence[20000] = { };
    int newsequencecount = 0;
    char insert;
    char *oldptr = oldsequence;
    char *newptr = newsequence;

    int numsteps = 10;
    for( int s = 1; s <= numsteps; s++ ) {
        //printf("Starting step %d\n", s);
        for( int i = 1; i < oldsequencecount; i++ ) {
            insert = pairs[oldptr[i-1]][oldptr[i]];
            if( insert != 0 ) {
                newptr[newsequencecount] = oldptr[i-1]; newsequencecount++;
                newptr[newsequencecount] = insert; newsequencecount++;
                elementcount[insert]++;
                newptr[newsequencecount] = oldptr[i];
            } else { fprintf(stderr, "Implement no sequence found logic.\n"); }
        }
        oldsequencecount = newsequencecount + 1;
        //printf("Sequence length after step %d: %d\n", s, oldsequencecount);
        newsequencecount = 0;
        char *temp = oldptr;
        oldptr = newptr;
        newptr = temp;
    }

    // Finding least and most element count
    int min = INT_MAX; int max = INT_MIN;
    for( int i = 0; i <= 100; i++ ) {
        if( (elementcount[i] < min) && (elementcount[i] != 0) ) {
            min = elementcount[i];
        }
        if( (elementcount[i] > max) ) { 
            max = elementcount[i];
        }
    }

    printf("Max: %d, Min: %d\n", max, min);
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

    int p1 = partone(filename);
    clock_t p1time = clock();
    printf("Part 1 (%lf): %d\n", (double)(p1time - start) / CLOCKS_PER_SEC, p1);

    int p2 = parttwo(filename);
    clock_t p2time = clock();
    printf("Part 2: (%lf): %d\n", (double)(p2time - start) / CLOCKS_PER_SEC, p2);
}
