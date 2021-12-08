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
    int linesize = 1000;
    char line[linesize];
    char *token;
    char *entry;
    int entries = 0;
    int sum = 0;

    while(( fgets(line, linesize, fp)) != NULL ) {
        token = strtok(line, "|"); // first part
        token = strtok(NULL, "|"); // second part
        entry = strtok(token, " \n\r\t");
        while( entry != NULL ) { 
            if ( strlen(entry) == 2 ||
                 strlen(entry) == 4 ||
                 strlen(entry) == 3 ||
                 strlen(entry) == 7 ) { sum++; }
            entry = strtok(NULL, " \n\r\t");
        }
    }

    // close file
    fclose(fp);

    ans = sum;
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
    int linesize = 1000;
    char line[linesize];
    char *token;
    char *first;
    char *second;
    char *entry;
    int scrambled[10];
    int lengths[10];
    int mappings[7];
    int letters[10];
    int lettersum = 0;
    int l;
    int fives[3];
    int fivecount = 0;
    int sixes[3];
    int sixcount = 0;
    int perms[6][3] = {{1, 2, 3},
                       {1, 3, 2},
                       {2, 1, 3},
                       {2, 3, 1},
                       {3, 1, 2},
                       {3, 2, 1}};

    // 0 through 9, contains which segments are 'on' for each digit
    int digits[10][7] = {{0, 1, 2, 4, 5, 6},
                         {3, 5},
                         {0, 2, 4, 6},
                         {0, 2, 3, 4, 6},
                         {1, 2, 3, 5},
                         {0, 1, 3, 5, 6},
                         {0, 1, 3, 4, 5, 6},
                         {0, 2, 5},
                         {0, 1, 2, 3, 4, 5, 6},
                         {0, 1, 2, 3, 5, 6}};

    while(( fgets(line, linesize, fp)) != NULL ) {
        first = strtok(line, "|");
        second = strtok(NULL, "|");

        entry = strtok(first, " \n\r\t");
        for ( int i = 0; i < 10; i++ ) { 
            lettersum = 0;
            l = strlen(entry);
            for( int j = 0; j < l; j++ ) {
                // a = 97, g = 103
                lettersum += entry[j] - 97;
            }
            // letters[10], 0 through 9
            // Categorizing letters
            if( l == 2 ) { letters[1] = lettersum; }
            else if( l == 3 ) { letters[7] = lettersum; }
            else if( l == 4 ) { letters[4] = lettersum; }
            else if( l == 7 ) { letters[8] = lettersum; }
            else if( l == 5 ) { fives[fivecount] = i; fivecount++; } 
            else if( l == 6 ) { sixes[sixcount] = i;  sixcount++;
            }

            // displays[10], a = 0, g = 6
            scrambled[i] = lettersum;
            lengths[i] = l;
            printf("%d: %s, %d\n", i, entry, lettersum);
            entry = strtok(NULL, " \n\r\t");
        }
        // mapping[7], 0 = A, 6 = G
        // 1, 4, 7, 8 are known
        // working through every permutation of 5 and 6 segment letters
        // Determining if it's a match or not
        // for fives: 0=2, 1=3, 2=5
        // for sixes: 0=0, 1=6, 2=9
        // Need a matching of actual segments [a-g] to ideal segments [a-g]
        mappings[0] = letters[7] - letters[1];
        for( int fi = 0; fi < 6; fi++ ) {
            for( int si = 0; si < 6; si++ ) {
                for( int fj = 0; fj < 3; fj++ ) {
                    
                }
                for( int sj = 0; sj < 3; sj++ ) {

                }
                // goto SKIPPERM;
            }
        }
        entry = strtok(second, " \n\r\t");
        while( entry != NULL ) { 
            // printf("%s\n", entry);
            entry = strtok(NULL, " \n\r\t");
        }
    }

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

    int p2 = parttwo(filename);
    clock_t p2time = clock();
    printf("Part 2: (%lf): %d\n", (double)(p2time - start) / CLOCKS_PER_SEC, p2);
}
