#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include "helpers.h"

struct Square {
    uint16_t x;
    uint16_t y;
    uint16_t val;
    uint16_t dist;
    struct Square *prev;
    bool seen;
};

struct Queue {
    int numitems;
    struct Square *items[3000];
};

void insert_item(struct Queue *q, struct Square *s) { // inserts so that highest = first
    int i = 0;
    if( q->numitems == 3000 ) {  // full
        fprintf(stderr, "Queue full, make bigger\n");
    } else if( q->numitems > 0 ) { // partially full
        for( i = q->numitems - 1; i >= 0; i-- ) {
            if( s->dist > q->items[i]->dist ) {
                q->items[i+1] = q->items[i];
            } else {
                break;
            }
        }
        //printf("Inserted %d,%d with dist %d into pos %d\n", s->x, s->y, s->dist, i+1);
        q->items[i+1] = s;
        q->numitems++;
    } else { // empty
        //printf("Inserted %d,%d with dist %d into pos 0\n", s->x, s->y, s->dist);
        q->items[0] = s;
        q->numitems++;
    }
}

struct Square *remove_item(struct Queue *q) { // removes the last (lowest) item
    q->numitems--;
    struct Square *s = q->items[q->numitems];
    //printf("Removed %d,%d with dist %d from pos %d\n", s->x, s->y, s->dist, q->numitems);
    return s;
}

int djikstras(struct Square grid[500][500], int maxsize) {
    // Building queue
    struct Queue q;
    q.numitems = 0;

    // starting square
    grid[0][0].dist = 0;
    grid[0][0].seen = true;
    insert_item(&q, &grid[0][0]);

    struct Square *curr, *next;
    while( q.numitems > 0 ) {
        curr = remove_item(&q);

        // return condition
        if( (curr->x == maxsize-1) && (curr->y == maxsize-1) ) {
            return curr->dist;
        }

        // finding neighbors of current square
        if( (curr->x > 0) && !(grid[curr->x-1][curr->y].seen) ) {  // -x
            next = &grid[curr->x-1][curr->y];
            next->dist = curr->dist + next->val;
            next->seen = true;
            insert_item(&q, next);
        }
        if( (curr->x < maxsize-1) && !(grid[curr->x+1][curr->y].seen) ) {  // +x
            next = &grid[curr->x+1][curr->y];
            next->dist = curr->dist + next->val;
            next->seen = true;
            insert_item(&q, next);
        }
        if( (curr->y > 0) && !(grid[curr->x][curr->y-1].seen) ) {  // -y
            next = &grid[curr->x][curr->y-1];
            next->dist = curr->dist + next->val;
            next->seen = true;
            insert_item(&q, next);
        }
        if( (curr->y < maxsize-1) && !(grid[curr->x][curr->y+1].seen) ) {  // +y
            next = &grid[curr->x][curr->y+1];
            next->dist = curr->dist + next->val;
            next->seen = true;
            insert_item(&q, next);
        }
    }
    return -2;
};

int partone (char *filename) {
    int ans = -1;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file
    int linesize = 200;
    char line[linesize];
    static struct Square grid[500][500];
    int y = 0;
    
    while( (fgets(line, linesize, fp) != NULL) ) {
        for( int x = 0; x < strlen(line)-1; x++) {
            grid[x][y].x = x;
            grid[x][y].y = y;
            grid[x][y].val = line[x] - 48; // ascii 0 = 48
            grid[x][y].dist = SHRT_MAX;
            grid[x][y].prev = NULL;
            grid[x][y].seen = false;
        }
        y++;
    }

    // close file
    fclose(fp);

    ans = djikstras(grid, y);

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
    int linesize = 200;
    char line[linesize];
    static struct Square grid[500][500];
    int y = 0;
    
    while( (fgets(line, linesize, fp) != NULL) ) {
        for( int x = 0; x < strlen(line)-1; x++) {
            grid[x][y].x = x;
            grid[x][y].y = y;
            grid[x][y].val = line[x] - 48; // ascii 0 = 48
            grid[x][y].dist = UINT16_MAX;
            grid[x][y].prev = NULL;
            grid[x][y].seen = false;
        }
        y++;
    }
    // close file
    fclose(fp);

    int maxsize = y;
    // x and y repeats of the grid, now that the grid size is known
    for( int x = 0; x < maxsize; x++) {
        for( int y = 0; y < maxsize; y++) {
            for( int dx = 0; dx < 5; dx++) {
                for( int dy = 0; dy < 5; dy++) {
                    grid[x + maxsize*dx][y + maxsize*dy].x = x + maxsize*dx;
                    grid[x + maxsize*dx][y + maxsize*dy].y = y + maxsize*dy;
                    grid[x + maxsize*dx][y + maxsize*dy].val = grid[x][y].val + dx + dy;
                    if ( grid[x + maxsize*dx][y + maxsize*dy].val > 9 ) { // wraparound 9 -> 1
                        grid[x + maxsize*dx][y + maxsize*dy].val = (grid[x + maxsize*dx][y + maxsize*dy].val + 1) % 10;
                    }
                    grid[x + maxsize*dx][y + maxsize*dy].dist = UINT16_MAX;
                    grid[x + maxsize*dx][y + maxsize*dy].prev = NULL;
                    grid[x + maxsize*dx][y + maxsize*dy].seen = false;
                }
            }
        }
    }

    ans = djikstras(grid, maxsize*5);

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
