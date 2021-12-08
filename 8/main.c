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
    char *first;
    char *second;
    char *entry;
    int l;


    while(( fgets(line, linesize, fp)) != NULL ) {
        int segmentmapping[7] = { };
        int digitmapping[10] = { };
        char strings[10][8] = { };
        int scrambledsums[10] = { };
        int lettersums[10] = { };
        int lettersum = 0;
        int fives[3];
        int fivecount = 0;
        int sixes[3];
        int sixcount = 0;

        first = strtok(line, "|");
        second = strtok(NULL, "|");

        entry = strtok(first, " \n\r\t");
        for ( int i = 0; i < 10; i++ ) { 
            lettersum = 0;
            l = strlen(entry);
            for( int j = 0; j < l; j++ ) {
                // a = 97, g = 103
                lettersum += entry[j];
            }
            // Categorizing digits by number of segments
            // printf("%s %d", entry, l);
            switch(l) {
                case 2:
                    lettersums[1] = lettersum;
                    digitmapping[1] = i;
                    break;
                case 3:
                    lettersums[7] = lettersum;
                    digitmapping[7] = i;
                    break;
                case 4:
                    lettersums[4] = lettersum;
                    digitmapping[4] = i;
                    break;
                case 7:
                    lettersums[8] = lettersum;
                    digitmapping[8] = i;
                    break;
                case 5:
                    fives[fivecount] = i;
                    fivecount++;
                    break;
                case 6:
                    sixes[sixcount] = i;
                    sixcount++;
                    break;

            }
            // displays[10], a = 0, g = 6
            scrambledsums[i] = lettersum;
            strcpy(strings[i], entry);
            //printf("%d: %s, %d\n", i, entry, lettersum);
            entry = strtok(NULL, " \n\r\t");
        }
        // segmentmapping[7], 0 = a, 6 = g, maps ideal segments to actual segments, ascii values 97 to 103
        // digitmapping[10], 0 = 0, 10 = 10 maps digits to order of scrambled digits provided
        // 1, 4, 7, 8 are known

        // 6 = 6 digit that doesn't contain '1' segments
        for( int si = 0; si < 3; si++ ) {
            if( (strchr(strings[sixes[si]], strings[digitmapping[1]][0]) == NULL) ||
                (strchr(strings[sixes[si]], strings[digitmapping[1]][1]) == NULL) ) {
                    digitmapping[6] = sixes[si];
                    lettersums[6] = scrambledsums[sixes[si]];
                    break;
            }
        }

        // 9 = 6 digit that contains '4' segments
        for( int si = 0; si < 3; si++ ) {
            if( (strchr(strings[sixes[si]], strings[digitmapping[4]][0]) != NULL) && 
                (strchr(strings[sixes[si]], strings[digitmapping[4]][1]) != NULL) &&
                (strchr(strings[sixes[si]], strings[digitmapping[4]][2]) != NULL) &&
                (strchr(strings[sixes[si]], strings[digitmapping[4]][3]) != NULL) ) {
                    digitmapping[9] = sixes[si];
                    lettersums[9] = scrambledsums[sixes[si]];
                    break;
            }
        }

        // 0 = remaining 6 digit (not 6 or 9)
        for( int si = 0; si < 3; si++ ) {
            if( sixes[si] != digitmapping[9] && sixes[si] != digitmapping[6] ) {
                digitmapping[0] = sixes[si];
                lettersums[0] = scrambledsums[sixes[si]];
                break;
            }
        }

        // 3 = 5 digit that contains '1' segments
        for( int fi = 0; fi < 3; fi++ ) {
            if( (strchr(strings[fives[fi]], strings[digitmapping[1]][0]) != NULL) && 
                (strchr(strings[fives[fi]], strings[digitmapping[1]][1]) != NULL) ) {
                    digitmapping[3] = fives[fi];
                    lettersums[3] = scrambledsums[fives[fi]];
                    break;
            }
        }

        // A = 7-1
        segmentmapping[0] = lettersums[7] - lettersums[1];

        // C = 8 - 6
        segmentmapping[2] = lettersums[8] - lettersums[6];

        // F = 1 - C
        segmentmapping[5] = lettersums[1] - segmentmapping[2];

        // E = 8 - 9
        segmentmapping[4] = lettersums[8] - lettersums[9];

        // D = 8 - 0
        segmentmapping[3] = lettersums[8] - lettersums[0];

        // 5 = 8 - C - E
        for( int j = 0; j < 10; j++ ) {
            int sum = lettersums[8] - segmentmapping[2] - segmentmapping[4];
            if( scrambledsums[j] == sum ) {
                digitmapping[5] = j;
                lettersums[5] = scrambledsums[j];
            }
        }

        // 2 = remaining 5 digit (not 3 or 5)
        for( int fi = 0; fi < 3; fi++ ) {
            if( fives[fi] != digitmapping[3] && fives[fi] != digitmapping[5] ) {
                digitmapping[2] = fives[fi];
                lettersums[2] = scrambledsums[fives[fi]];
                break;
            }
        }

        // G = 2 - A - C - D - E
        segmentmapping[6] = lettersums[2] - segmentmapping[0] - segmentmapping[2] - segmentmapping[3] - segmentmapping[4];

        // B = 5 - A - D - F - G
        segmentmapping[1] = lettersums[5] - segmentmapping[0] - segmentmapping[3] - segmentmapping[5] - segmentmapping[6];

        // Applying the solution to the 4 7segs at the end
        int segmentsvalue = 0;
        entry = strtok(second, " \n\r\t");
        //printf("%s: ", entry);
        // starts at first digit (1000s place)
        for( int j = 1000; j != 0; j = j / 10 ) {
            // comparing each character in the solution digit to the original digits
            // can't use lettersums here as there can be duplicates
            // Then map the scrambled digit index to the digitmapping index to find the value.
            for( int si = 0; si < 10; si++ ) {  // original list of digits
                //printf("%s: %lu, %s %lu\n", strings[si], strlen(strings[si]), entry, strlen(entry));
                if( strlen(strings[si]) == strlen(entry) ) {
                    for( int ei = 0; ei < strlen(entry); ei++ ) { // iterating though entry chars
                        if( strchr( strings[si], entry[ei] ) == NULL ) { goto NEXTITER; } // skip to next original value
                    }
                    // all digits match, save and skip to next entry
                    // The index of the digitmapping array is the actual value of the digit
                    for( int di = 0; di < 10; di++ ) {
                        if( digitmapping[di] == si ) {
                            segmentsvalue += j * di;
                            goto NEXTENTRY;
                        }
                    }
                }
                NEXTITER: ;
            }
            NEXTENTRY: ;
            //printf("%s, %d, %d\n", entry, j, segmentsvalue);
            entry = strtok(NULL, " \n\r\t");
        }
        //printf("%d\n", segmentsvalue);
        ans += segmentsvalue;
    }

    // close file
    fclose(fp);

    return ans + 1; // for the initial offset of -1
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
