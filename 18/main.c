#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

enum PAIRTYPE {
    NONE = 0,
    INT = 1,
    PAIR = 2
};

struct Pair {
    union {
        int i;
        struct Pair *p;
    } left;
    enum PAIRTYPE lefttype;

    union {
        int i;
        struct Pair *p;
    } right;
    enum PAIRTYPE righttype;

    struct Pair *parent;
};

void printpair(struct Pair *p) {
    printf("[");
    switch( p->lefttype ) {
        case INT:
            printf("%d", p->left.i);
            break;
        case PAIR:
            printpair(p->left.p);
            break;
        case NONE:
            fprintf(stderr, "Left type NONE for pair %p\n", p);
            break;
    }
    printf(",");
    switch( p->righttype ) {
        case INT:
            printf("%d", p->right.i);
            break;
        case PAIR:
            printpair(p->right.p);
            break;
        case NONE:
            fprintf(stderr, "Right type NONE for pair %p\n", p);
            break;
    }
    printf("]");
    if( p->parent == NULL ) {
        printf("\n");
    }
}

struct Pair *getexplosivepair(struct Pair *p, int level, int explodelevel) {
    struct Pair *explosivepair;

    if( level >= explodelevel ) {
        return p;
    }
    if( p->lefttype == PAIR ) {
        explosivepair = getexplosivepair(p->left.p, level+1, explodelevel);
        if( explosivepair != NULL ) {
            return explosivepair;
        }
    }
    if ( p->righttype == PAIR ) {
        explosivepair = getexplosivepair(p->right.p, level+1, explodelevel);
        if( explosivepair != NULL ) {
            return explosivepair;
        }
    }
    return NULL;  // max depth, not deep enough to explode
}

void explode(struct Pair *p) {
    struct Pair *currpair;

    currpair = p;
    // find first int value to 'right' of pair right (in-order successor that's an INT)
    while( 1 ){
        // The start node is in the far right position (we are just below the root)
        if( currpair->parent->right.p == currpair && currpair->parent->parent == NULL) {
            //printf("No right value to add explosion to\n");
            break;
        } 
        // current node is right node of parent, go up one more level
        else if( currpair->parent->right.p == currpair ) {
            currpair = currpair->parent;
        }
        // current node is left node of parent, go down right node of parent
        else if( currpair->parent->left.p == currpair ) {
            // parent right node is int, this is the in-order successor
            if( currpair->parent->righttype == INT ) {
                //printf("Adding right exploded value %d to %d\n", p->right.i, currpair->parent->right.i);
                currpair->parent->right.i += p->right.i;
                break;
            }
            // parent right node is pair, this branch will contain the in-order successor
            else if( currpair->parent->righttype == PAIR ) {
                currpair = currpair->parent->right.p;
                // keep going down the left side until you hit an int
                while( currpair->lefttype == PAIR ) {
                    currpair = currpair->left.p;
                }
                //printf("Adding right exploded value %d to %d\n", p->right.i, currpair->left.i);
                currpair->left.i += p->right.i;
                break;
            }
        }
    }

    currpair = p;
    // find first int value to 'left' of pair left (in-order predeccessor that is an INT)
    while( 1 ){
        // The start node is in the far left position (we are just below the root)
        if( currpair->parent->left.p == currpair && currpair->parent->parent == NULL) {
            //printf("No left value to add explosion to\n");
            break;
        } 
        // current node is left node of parent, go up one more level
        else if( currpair->parent->left.p == currpair ) {
            currpair = currpair->parent;
        }
        // current node is right node of parent, go down left node of parent
        else if( currpair->parent->right.p == currpair ) {
            // parent left node is int, this is the in-order predeccessor
            if( currpair->parent->lefttype == INT ) {
                //printf("Adding left exploded value %d to %d\n", p->left.i, currpair->parent->left.i);
                currpair->parent->left.i += p->left.i;
                break;
            }
            // parent left node is pair, this branch will contain the in-order predeccessor
            else if( currpair->parent->lefttype == PAIR ) {
                currpair = currpair->parent->left.p;
                // keep going down the right side until you hit an int
                while( currpair->righttype == PAIR ) {
                    currpair = currpair->right.p;
                }
                //printf("Adding left exploded value %d to %d\n", p->left.i, currpair->right.i);
                currpair->right.i += p->left.i;
                break;
            }
        }
    }

    // replace pair with 0
    if( p->parent->left.p == p ) {
        p->parent->left.i = 0; 
        p->parent->lefttype = INT;
    } else if( p->parent->right.p == p ) {
        p->parent->right.i = 0; 
        p->parent->righttype = INT;
    } else {
        fprintf(stderr, "Exploded pair not found as left or right of parent\n");
    }

    free(p);
}

struct Pair *getsplitpair(struct Pair *p, int splitthresh) {
    struct Pair *splitpair;

    if( p->lefttype == INT && p->left.i >= splitthresh) {
        return p;
    } else if( p->lefttype == PAIR) {
        splitpair = getsplitpair(p->left.p, splitthresh);
        if( splitpair != NULL ) {
            return splitpair;
        }
    }

    if( p->righttype == INT && p->right.i >= splitthresh) {
        return p;
    } else if( p->righttype == PAIR) {
        splitpair = getsplitpair(p->right.p, splitthresh);
        if( splitpair != NULL ) {
            return splitpair;
        }
    }
    return NULL;
}

void split(struct Pair *p, int splitthresh) {
    // find the branch (left or right) that needs to be split
    if( p->lefttype == INT && p->left.i >= splitthresh ) {
        // init new pair
        struct Pair *newpair = (struct Pair *) malloc(sizeof(struct Pair));
        newpair->parent = p;
        newpair->lefttype = INT;
        newpair->righttype = INT;

        // determining split values (left is halved and rounded down, right is halved and rounded up)
        newpair->left.i = p->left.i / 2;
        newpair->right.i = (p->left.i / 2) + p->left.i % 2;
        //printf("Split left node from %d to pair %d, %d\n", p->left.i, newpair->left.i, newpair->right.i);

        // setting link to previous pair
        p->lefttype = PAIR;
        p->left.p = newpair;
    } else if( p->righttype == INT && p->right.i >= splitthresh ) {
        // init new pair
        struct Pair *newpair = (struct Pair *) malloc(sizeof(struct Pair));
        newpair->parent = p;
        newpair->lefttype = INT;
        newpair->righttype = INT;

        // determining split values (left is halved and rounded down, right is halved and rounded up)
        newpair->left.i = p->right.i / 2;
        newpair->right.i = (p->right.i / 2) + p->right.i % 2;
        //printf("Split right node from %d to pair %d, %d\n", p->right.i, newpair->left.i, newpair->right.i);

        // setting link to previous pair
        p->righttype = PAIR;
        p->right.p = newpair;
    } else {
        fprintf(stderr, "No valid split found\n");
    }
}

int calculatemagnitude(struct Pair *p) {
    int leftval, rightval;
    switch (p->lefttype) {
        case INT:
            leftval = p->left.i * 3;
            break;
        case PAIR:
            leftval = calculatemagnitude(p->left.p) * 3;
            break;
        case NONE:
            leftval = INT_MIN;
            fprintf(stderr, "Left type is None\n");
            break;
    }

    switch (p->righttype) {
        case INT:
            rightval = p->right.i * 2;
            break;
        case PAIR:
            rightval = calculatemagnitude(p->right.p) * 2;
            break;
        case NONE:
            fprintf(stderr, "Right type is None\n");
            rightval = INT_MIN;
            break;
    }

    free(p);
    //printf("Magnitude: Left %d, Right %d, Sum %d\n", leftval, rightval, leftval+rightval);
    return( leftval + rightval );
}

void copypair(struct Pair *p, struct Pair *newp) {
    switch (p->lefttype) {
        case INT: ;
            newp->lefttype = INT;
            newp->left.i = p->left.i;
            break;
        case PAIR: ;
            struct Pair *nextp = (struct Pair *) malloc(sizeof(struct Pair));
            newp->lefttype = PAIR;
            newp->left.p = nextp;
            nextp->parent = newp;
            copypair(p->left.p, newp->left.p);
            break;
        case NONE: ;
            fprintf(stderr, "Left type is None\n");
            break;
    }

    switch (p->righttype) {
        case INT: ;
            newp->righttype = INT;
            newp->right.i = p->right.i;
            break;
        case PAIR: ;
            struct Pair *nextp = (struct Pair *) malloc(sizeof(struct Pair));
            newp->righttype = PAIR;
            newp->right.p = nextp;
            nextp->parent = newp;
            copypair(p->right.p, newp->right.p);
            break;
        case NONE: ;
            fprintf(stderr, "Left type is None\n");
            break;
    }
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
    char c;
    struct Pair *currpair = NULL, *newpair = NULL, *heads[100];
    int headcount = 0;

    c = fgetc(fp);
    while( !feof(fp) ) {
        while( c != '\n' ) {  // single line
            switch (c) {
                case '[':  // start of a new pair, go down one level
                    // init new pair
                    newpair = (struct Pair *) malloc(sizeof(struct Pair));
                    newpair->parent = currpair;
                    newpair->lefttype = NONE;
                    newpair->righttype = NONE;

                    // setting first or second value of parent
                    if( currpair == NULL ) {
                        heads[headcount] = newpair;
                        headcount++;
                        //printf("Created new HEAD pair at %p\n", newpair);
                    } else if( currpair->lefttype == NONE ) { // first val
                        currpair->left.p = newpair;
                        currpair->lefttype = PAIR;
                        //printf("Created new child pair at %p on left of parent at %p\n", newpair, currpair);
                    } else {  // second val
                        currpair->right.p = newpair;
                        currpair->righttype = PAIR;
                        //printf("Created new child pair at %p on right of parent at %p\n", newpair, currpair);
                    }
                    currpair = newpair;
                    break;
                case ']':  // completion of current pair, move back up a level
                    //printf("Completed pair at %p, moving to parent at %p\n", currpair, currpair->parent);
                    currpair = currpair->parent;
                    break;
                case ',':
                    break;
                default:  // int
                    if( currpair == NULL ) {
                        fprintf(stderr, "Int before HEAD");
                    } else if( currpair->lefttype == NONE ) { // first val
                        currpair->left.i = c - 48; // ascii 0 = 48, 9 = 57
                        currpair->lefttype = INT;
                        //printf("Set left val of pair at %p to %d\n", newpair, currpair->left.i);
                    } else {  // second val
                        currpair->right.i = c - 48; // ascii 0 = 48, 9 = 57
                        currpair->righttype = INT;
                        //printf("Set right val of pair at %p to %d\n", newpair, currpair->right.i);
                    }
                    break;
            }
            c = fgetc(fp);
        }
        //printf("Number complete\n");
        currpair = NULL;
        c = fgetc(fp);
    }

    // close file
    fclose(fp);

    currpair = heads[0];
    for( int i = 1; i < headcount; i++ ) {
        //printf("Adding next value: "); printpair(heads[i]);
        // create top level node and add next head to it
        struct Pair *nextpair = (struct Pair *) malloc(sizeof(struct Pair));
        nextpair->parent = NULL;
        nextpair->lefttype = PAIR;
        nextpair->left.p = currpair;
        currpair->parent = nextpair;
        nextpair->righttype = PAIR;
        nextpair->right.p = heads[i];
        heads[i]->parent = nextpair;

        currpair = nextpair;

        // reduction
        while( 1 ) {
            //printpair(currpair);
            // check explosions
            struct Pair *explosivepair = getexplosivepair(currpair, 0, 4);
            if( explosivepair != NULL ) {
                explode(explosivepair);
                continue;
            }

            // check splits
            struct Pair *splitpair = getsplitpair(currpair, 10);
            if( splitpair != NULL ) {
                split(splitpair, 10);
                continue;
            }
            break;
        }
    }

    // calculating magnitude
    ans = calculatemagnitude(currpair);

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
    char c;
    struct Pair *currpair = NULL, *newpair = NULL, *heads[100];
    int headcount = 0;

    c = fgetc(fp);
    while( !feof(fp) ) {
        while( c != '\n' ) {  // single line
            switch (c) {
                case '[':  // start of a new pair, go down one level
                    // init new pair
                    newpair = (struct Pair *) malloc(sizeof(struct Pair));
                    newpair->parent = currpair;
                    newpair->lefttype = NONE;
                    newpair->righttype = NONE;

                    // setting first or second value of parent
                    if( currpair == NULL ) {
                        heads[headcount] = newpair;
                        headcount++;
                        //printf("Created new HEAD pair at %p\n", newpair);
                    } else if( currpair->lefttype == NONE ) { // first val
                        currpair->left.p = newpair;
                        currpair->lefttype = PAIR;
                        //printf("Created new child pair at %p on left of parent at %p\n", newpair, currpair);
                    } else {  // second val
                        currpair->right.p = newpair;
                        currpair->righttype = PAIR;
                        //printf("Created new child pair at %p on right of parent at %p\n", newpair, currpair);
                    }
                    currpair = newpair;
                    break;
                case ']':  // completion of current pair, move back up a level
                    //printf("Completed pair at %p, moving to parent at %p\n", currpair, currpair->parent);
                    currpair = currpair->parent;
                    break;
                case ',':
                    break;
                default:  // int
                    if( currpair == NULL ) {
                        fprintf(stderr, "Int before HEAD");
                    } else if( currpair->lefttype == NONE ) { // first val
                        currpair->left.i = c - 48; // ascii 0 = 48, 9 = 57
                        currpair->lefttype = INT;
                        //printf("Set left val of pair at %p to %d\n", newpair, currpair->left.i);
                    } else {  // second val
                        currpair->right.i = c - 48; // ascii 0 = 48, 9 = 57
                        currpair->righttype = INT;
                        //printf("Set right val of pair at %p to %d\n", newpair, currpair->right.i);
                    }
                    break;
            }
            c = fgetc(fp);
        }
        //printf("Number complete\n");
        currpair = NULL;
        c = fgetc(fp);
    }

    // close file
    fclose(fp);

    int maxmagnitude = 0, magnitude;
    for( int i = 0; i < headcount-1; i++ ) {
        for( int j = i+1; j < headcount; j++) {
            struct Pair *currpair = (struct Pair *) malloc(sizeof(struct Pair));
            currpair->parent = NULL;

            struct Pair *leftpair = (struct Pair *) malloc(sizeof(struct Pair));
            copypair(heads[i], leftpair);
            currpair->lefttype = PAIR;
            currpair->left.p = leftpair;
            leftpair->parent = currpair;

            struct Pair *rightpair = (struct Pair *) malloc(sizeof(struct Pair));
            copypair(heads[j], rightpair);
            currpair->righttype = PAIR;
            currpair->right.p = rightpair;
            rightpair->parent = currpair;

            //printpair(currpair);
            // reduction
            while( 1 ) {
                //printpair(currpair);
                // check explosions
                struct Pair *explosivepair = getexplosivepair(currpair, 0, 4);
                if( explosivepair != NULL ) {
                    explode(explosivepair);
                    continue;
                }

                // check splits
                struct Pair *splitpair = getsplitpair(currpair, 10);
                if( splitpair != NULL ) {
                    split(splitpair, 10);
                    continue;
                }
                break;
            }

            //printpair(currpair);
            magnitude = calculatemagnitude(currpair);
            //printf("Total magnitude of %d and %d: %d\n", i, j, magnitude);
            if( magnitude > maxmagnitude ) {
                maxmagnitude = magnitude;
            }
        }
    }

    // lazy
    for( int i = headcount-1; i > 0; i-- ) {
        for( int j = i-1; j >= 0; j--) {
            struct Pair *currpair = (struct Pair *) malloc(sizeof(struct Pair));
            currpair->parent = NULL;

            struct Pair *leftpair = (struct Pair *) malloc(sizeof(struct Pair));
            copypair(heads[i], leftpair);
            currpair->lefttype = PAIR;
            currpair->left.p = leftpair;
            leftpair->parent = currpair;

            struct Pair *rightpair = (struct Pair *) malloc(sizeof(struct Pair));
            copypair(heads[j], rightpair);
            currpair->righttype = PAIR;
            currpair->right.p = rightpair;
            rightpair->parent = currpair;

            //printpair(currpair);
            // reduction
            while( 1 ) {
                //printpair(currpair);
                // check explosions
                struct Pair *explosivepair = getexplosivepair(currpair, 0, 4);
                if( explosivepair != NULL ) {
                    explode(explosivepair);
                    continue;
                }

                // check splits
                struct Pair *splitpair = getsplitpair(currpair, 10);
                if( splitpair != NULL ) {
                    split(splitpair, 10);
                    continue;
                }
                break;
            }

            //printpair(currpair);
            magnitude = calculatemagnitude(currpair);
            //printf("Total magnitude of %d and %d: %d\n", i, j, magnitude);
            if( magnitude > maxmagnitude ) {
                maxmagnitude = magnitude;
            }
        }
    }

    ans = maxmagnitude;
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
