#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>

int partone (char *filename) {
    int ans = -1;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -2;
    }

    // ascii values
    // < 60 > 62
    // [ 91 ] 93
    // ( 40 ) 41
    // { 123 } 125

    // read file
    char ch;
    int corruptcount = 0;
    bool corruptedline = false;
    char line[100];
    int linehead = 0;
    int points = 0;

    ch = fgetc( fp );
    while( !feof(fp) ) {  // full file
        if ( ch == '\n' ) {  // single line
            linehead = 0;
            if( corruptedline ) {
                corruptcount++;
                corruptedline = false;
            }
        } else if( !corruptedline ) {
            if( ch == '<' || ch == '[' || ch == '(' || ch == '{' ) {
                line[linehead] = ch;
                linehead++;
                // printf("New pair %c\n", ch);
            } else if( ch == '>' || ch == ']' || ch == '}' ) {
                if( line[linehead-1] == (ch - 2) ) {
                    linehead--; 
                    // printf("Matched %c with %c\n", ch-2, ch);
                } else {
                    corruptedline = true;
                    switch (ch) {
                        case ']': points += 57; break;
                        case '}': points += 1197; break;
                        case '>': points += 25137; break;
                    }
                    // printf("Expected %c, found %c\n", ch-2, ch);
                }
            } else if( ch == ')' ) {
                if( line[linehead-1] == (ch - 1) ) {
                    linehead--; 
                    // printf("Matched %c with %c\n", ch-1, ch);
                } else {
                    corruptedline = true;
                    points += 3;
                    // printf("Expected %c, found %c\n", ch-1, ch);
                }
            } else {
                fprintf(stderr, "Unrecognized character: %c (%d)\n", ch, ch);
                return -3;
            }
        }
        ch = fgetc( fp );
    }

    // close file
    fclose(fp);

    ans = points;
    return ans;
}


int sortscore( const void * a, const void * b ) {
    return ( *(long*)a < *(long*)b );
}

long long parttwo (char *filename) {
    long ans = -1;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -2;
    }

    // ascii values
    // < 60 > 62
    // [ 91 ] 93
    // ( 40 ) 41
    // { 123 } 125

    // read file
    char ch;
    bool corruptedline = false;
    char line[100];
    int linehead = 0;
    long scores[100] = { };
    long score = 0;
    int numscores = 0;

    ch = fgetc( fp );
    while( !feof(fp) ) {  // full file
        if ( ch == '\n' ) {  // single line
            if( !corruptedline ) { // if line is incomplete
                for( int i = 1; i <= linehead ; i++ ) {
                    score = score * 5;
                    switch (line[linehead-i]) {
                        case '(': score += 1; break;
                        case '[': score += 2; break;
                        case '{': score += 3; break;
                        case '<': score += 4; break;
                    }
                }
                //printf("New Score: %lld, Tot: %d\n", score, numscores+1);
                scores[numscores] = score;
                numscores++;
                score = 0;
            }
            corruptedline = false;
            linehead = 0;
        } else if( !corruptedline ) {
            if( ch == '<' || ch == '[' || ch == '(' || ch == '{' ) {
                line[linehead] = ch;
                linehead++;
                // printf("New pair %c\n", ch);
            } else if( ch == '>' || ch == ']' || ch == '}' ) {
                if( line[linehead-1] == (ch - 2) ) {
                    linehead--; 
                } else { corruptedline = true; }
            } else if( ch == ')' ) {
                if( line[linehead-1] == (ch - 1) ) {
                    linehead--; 
                    // printf("Matched %c with %c\n", ch-1, ch);
                } else { corruptedline = true; }
            } else {
                fprintf(stderr, "Unrecognized character: %c (%d)\n", ch, ch);
                return -3;
            }
        }
        ch = fgetc( fp );
    }

    // close file
    fclose(fp);

    // Finding mid value
    qsort(scores, numscores, sizeof(long), sortscore);
    ans = scores[numscores/2];

    for( int i = 0; i < numscores; i++ ) {
        printf("Score %d: %ld\n", i, scores[i]);
    }

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

    long long p2 = parttwo(filename);
    clock_t p2time = clock();
    printf("Part 2: (%lf): %lld\n", (double)(p2time - start) / CLOCKS_PER_SEC, p2);
}
