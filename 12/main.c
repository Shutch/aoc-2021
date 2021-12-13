#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include "helpers.h"

struct Cave {
    char id[6];  // longest id is "start"
    bool big;
    struct Cave *connections[30];
    int numconnections;
    bool visitedonce;
};

int dfsp1(struct Cave *currentcave, char visitedcaveids[][6], int numcavesvisited) {
    //printf("Continuing dfs in '%s': %d\n", currentcave->id, numcavesvisited);

    // adding current cave to visited caves
    strcpy(visitedcaveids[numcavesvisited], currentcave->id);
    numcavesvisited++;
    
    if( strcmp(currentcave->id, "end") == 0 ) {
        //printf("Reached end in %d\n", numcavesvisited);
        return (1); 
    }
    
    int numpaths = 0;
    // diving to next cave
    for( int i = 0; i < currentcave->numconnections; i++ ) {
        struct Cave *nextcave = currentcave->connections[i];
        bool validnextcave = true;
        for( int j = 0; j < numcavesvisited; j++ ) {
            // check if  it's a small cave and already visited
            if( (strcmp(nextcave->id, visitedcaveids[j]) == 0) && !(nextcave->big) ) {
                validnextcave = false;
                //printf("Next cave '%s' has already been visited\n", nextcave->id);
            }
        }
        if( validnextcave ) {
            numpaths += dfsp1(nextcave, visitedcaveids, numcavesvisited);
        }
    }
    //printf("Returning from '%s' with %d results\n", currentcave->id, numpaths);
    return numpaths;
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
    int linesize = 20;
    char line[linesize];
    char *token;
    struct Cave *head = NULL;
    struct Cave *tail = NULL;
    struct Cave caves[100];
    int numcaves = 0;
    struct Cave *firstcave = NULL;
    struct Cave *secondcave = NULL;

    while( (fgets(line, linesize, fp)) != NULL ) {
        // First cave
        token = strtok(line, "-\n");
        for( int i = 0; i < numcaves; i++ ) {  // searching for existing cave
            if( (strcmp(token, caves[i].id)) == 0 ) {
                //printf("Found existing first cave '%s' at index %d\n", token, i);
                firstcave = &caves[i];
            }
        }
        if( firstcave == NULL ) {  // make new cave
            strcpy(caves[numcaves].id, token);
            // upper case = big cave. upper case ascii code 65-90, lower case 97 - 122
            caves[numcaves].big = (token[0] < 91) ? true : false;
            caves[numcaves].numconnections = 0;
            //printf("Created new first cave '%s' at index %d\n", token, numcaves); 
            firstcave = &caves[numcaves];
            numcaves++;
        }
        // special check for start/end
        if( (head == NULL) && (strcmp(firstcave->id, "start") == 0) ) { head = firstcave; }
        if( (tail == NULL) && (strcmp(firstcave->id, "end") == 0) ) { tail = firstcave; }

        // Second cave
        token = strtok(NULL, "-\n");
        for( int i = 0; i < numcaves; i++ ) {  // searching for existing cave
            if( (strcmp(token, caves[i].id)) == 0) {
                //printf("Found existing second cave '%s' at index %d\n", token, i);
                secondcave = &caves[i];
            }
        }
        if( secondcave == NULL ) {  // make new cave
            strcpy(caves[numcaves].id, token);
            // upper case = big cave. upper case ascii code 65-90, lower case 97 - 122
            caves[numcaves].big = (token[0] < 91) ? true : false;
            caves[numcaves].numconnections = 0;
            //printf("Created new second cave '%s' at index %d\n", token, numcaves); 
            secondcave = &caves[numcaves];
            numcaves++;
        }

        // special check for start/end
        if( (head == NULL) && (strcmp(secondcave->id, "start") == 0) ) { head = secondcave; }
        if( (tail == NULL) && (strcmp(secondcave->id, "end") == 0) ) { tail = secondcave; }

        // Adding relationships
        firstcave->connections[firstcave->numconnections] = secondcave;
        firstcave->numconnections++;
        secondcave->connections[secondcave->numconnections] = firstcave;
        secondcave->numconnections++;
        //printf("Added connections between %s and %s\n", firstcave->id, secondcave->id);
        firstcave = NULL;
        secondcave = NULL;
    }

    // close file
    fclose(fp);
    // lets use some dfs to find all viable paths
    char visitedcaveids[100][6];
    ans = dfsp1(head, visitedcaveids, 0);

    return ans;
}

int dfsp2(struct Cave *currentcave, char visitedcaveids[][6], int numcavesvisited, bool secondburned) {
    //printf("Continuing dfs in '%s': %d\n", currentcave->id, numcavesvisited);

    // adding current cave to visited caves
    strcpy(visitedcaveids[numcavesvisited], currentcave->id);
    numcavesvisited++;
    
    if( strcmp(currentcave->id, "end") == 0 ) {
        //printf("Reached end in %d\n", numcavesvisited);
        return (1); 
    }
    
    int numpaths = 0;
    // diving to next cave
    for( int i = 0; i < currentcave->numconnections; i++ ) {
        struct Cave *nextcave = currentcave->connections[i];
        int numtimesvisited = 0;
        for( int j = 0; j < numcavesvisited; j++ ) {
            if( (strcmp(nextcave->id, visitedcaveids[j]) == 0) ) { numtimesvisited++; }
        }
        //printf("Cave %s has been visited %d times\n", nextcave->id, numtimesvisited);

        // valid next cave: large OR small and not visisted
        if( (nextcave->big) || (!nextcave->big && numtimesvisited == 0) ) {
            numpaths += dfsp2(nextcave, visitedcaveids, numcavesvisited, secondburned);
        }
        // if cave small, visited once, secondburned false. 'start' and 'end' can't be visited more than once
        else if( (!nextcave->big && numtimesvisited == 1 && !secondburned &&
                  strcmp(nextcave->id, "start") != 0 && strcmp(nextcave->id, "end") != 0) ){
            //printf("Burning second visit to %s\n", nextcave->id);
            numpaths += dfsp2(nextcave, visitedcaveids, numcavesvisited, true);
        } else {
            //printf("Next cave '%s' is not valid\n", nextcave->id);
        }
    }
    //printf("Returning from '%s' with %d results\n", currentcave->id, numpaths);
    return numpaths;
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
    int linesize = 20;
    char line[linesize];
    char *token;
    struct Cave *head = NULL;
    struct Cave *tail = NULL;
    struct Cave caves[100];
    int numcaves = 0;
    struct Cave *firstcave = NULL;
    struct Cave *secondcave = NULL;

    while( (fgets(line, linesize, fp)) != NULL ) {
        // First cave
        token = strtok(line, "-\n");
        for( int i = 0; i < numcaves; i++ ) {  // searching for existing cave
            if( (strcmp(token, caves[i].id)) == 0 ) {
                //printf("Found existing first cave '%s' at index %d\n", token, i);
                firstcave = &caves[i];
            }
        }
        if( firstcave == NULL ) {  // make new cave
            strcpy(caves[numcaves].id, token);
            // upper case = big cave. upper case ascii code 65-90, lower case 97 - 122
            caves[numcaves].big = (token[0] < 91) ? true : false;
            caves[numcaves].numconnections = 0;
            //printf("Created new first cave '%s' at index %d\n", token, numcaves); 
            firstcave = &caves[numcaves];
            numcaves++;
        }
        // special check for start/end
        if( (head == NULL) && (strcmp(firstcave->id, "start") == 0) ) { head = firstcave; }
        if( (tail == NULL) && (strcmp(firstcave->id, "end") == 0) ) { tail = firstcave; }

        // Second cave
        token = strtok(NULL, "-\n");
        for( int i = 0; i < numcaves; i++ ) {  // searching for existing cave
            if( (strcmp(token, caves[i].id)) == 0) {
                //printf("Found existing second cave '%s' at index %d\n", token, i);
                secondcave = &caves[i];
            }
        }
        if( secondcave == NULL ) {  // make new cave
            strcpy(caves[numcaves].id, token);
            // upper case = big cave. upper case ascii code 65-90, lower case 97 - 122
            caves[numcaves].big = (token[0] < 91) ? true : false;
            caves[numcaves].numconnections = 0;
            //printf("Created new second cave '%s' at index %d\n", token, numcaves); 
            secondcave = &caves[numcaves];
            numcaves++;
        }

        // special check for start/end
        if( (head == NULL) && (strcmp(secondcave->id, "start") == 0) ) { head = secondcave; }
        if( (tail == NULL) && (strcmp(secondcave->id, "end") == 0) ) { tail = secondcave; }

        // Adding relationships
        firstcave->connections[firstcave->numconnections] = secondcave;
        firstcave->numconnections++;
        secondcave->connections[secondcave->numconnections] = firstcave;
        secondcave->numconnections++;
        //printf("Added connections between %s and %s\n", firstcave->id, secondcave->id);
        firstcave = NULL;
        secondcave = NULL;
    }

    // close file
    fclose(fp);
    // lets use some dfs to find all viable paths
    char visitedcaveids[100][6];
    ans = dfsp2(head, visitedcaveids, 0, false);

    return ans;
}

int main( int argc, char *argv[] ) {
    char *filename;
    if( argc == 2) {
        filename = argv[1];
    }
    else if( argc > 2) {
        fprintf(stderr, "Too many arguments\n");
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
