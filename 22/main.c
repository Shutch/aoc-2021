#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool state;
    int x0, x1, y0, y1, z0, z1;
} Action;

typedef struct aabb {
    struct aabb *next;
    struct aabb *prev;
    int x0, x1, y0, y1, z0, z1;
} AABB;

AABB *HEAD = NULL;

// Add new AABB before the given AABB
void addAABB(AABB *curr, AABB *new) {
    AABB *prev = curr->prev;
    if( prev == NULL ) {
        HEAD = new;
        //printf("New head (%d, %d) (%d, %d) (%d, %d)\n", new->x0, new->x1, new->y0, new->y1, new->z0, new->z1);
    } else {
        prev->next = new;
    }
    new->prev = prev;
    new->next = curr;
    curr->prev = new;
    //printf("Added AABB (%d, %d) (%d, %d) (%d, %d)\n", new->x0, new->x1, new->y0, new->y1, new->z0, new->z1);
}

// Allocates and initializes new AABB on the heap, then returns a pointer to it
AABB *createAABB(int x0, int x1, int y0, int y1, int z0, int z1) {
    AABB *new = (AABB *)malloc(sizeof(AABB));
    new->x0 = x0;
    new->x1 = x1;
    new->y0 = y0;
    new->y1 = y1;
    new->z0 = z0;
    new->z1 = z1;
    new->next = NULL;
    new->prev = NULL;
    //printf("Created AABB (%d, %d) (%d, %d) (%d, %d)\n", new->x0, new->x1, new->y0, new->y1, new->z0, new->z1);
    return new;
}

// Allocates and initializes new AABB on the heap, then returns a pointer to it
AABB *createAABBfromaction(Action *a) {
    AABB *new = (AABB *)malloc(sizeof(AABB));
    new->x0 = a->x0;
    new->x1 = a->x1;
    new->y0 = a->y0;
    new->y1 = a->y1;
    new->z0 = a->z0;
    new->z1 = a->z1;
    new->next = NULL;
    new->prev = NULL;
    //printf("Created AABB (%d, %d) (%d, %d) (%d, %d)\n", new->x0, new->x1, new->y0, new->y1, new->z0, new->z1);
    return new;
}

// removes AABB from list, fixes links, frees AABB
void removeAABB(AABB *r) {
    AABB *prev = r->prev;
    AABB *next = r->next;
    if( next != NULL ) {
        next->prev = prev;
    }
    if( prev != NULL ) {
        prev->next = next;
    } else {
        HEAD = next;
        //printf("New head %p\n", HEAD);
    }

    //printf("Removed AABB (%d, %d) (%d, %d) (%d, %d)\n", r->x0, r->x1, r->y0, r->y1, r->z0, r->z1);
    free(r);
}

// returns true if the action intersects or borders the aabb, otherwise returns false
bool actionintersects(Action *action, AABB *aabb) {
    // action and AABB do not intersect
    if( (action->x0 > aabb->x1) || (action->y0 > aabb->y1) || (action->z0 > aabb->z1) || 
        (action->x1 < aabb->x0) || (action->y1 < aabb->y0) || (action->z1 < aabb->z0) ) {
        //printf("No intersection between (%d, %d) (%d, %d) (%d, %d) and (%d, %d) (%d, %d) (%d, %d)\n",
        //       action->x0, action->x1, action->y0, action->y1, action->z0, action->z1,
        //       aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1);
        return false; 
    }
    return true;
}

bool actioncontains(Action *action, AABB *aabb) {
    if( (action->x1 >= aabb->x1) && (action->y1 >= aabb->y1) && (action->z1 >= aabb->z1) && 
        (action->x0 <= aabb->x0) && (action->y0 <= aabb->y0) && (action->z0 <= aabb->z0) ) {
        //printf("Action (%d, %d) (%d, %d) (%d, %d) contains AABB (%d, %d) (%d, %d) (%d, %d)\n",
        //       action->x0, action->x1, action->y0, action->y1, action->z0, action->z1,
        //       aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1);
        return true; 
    }
    return false;
}

// Trims the original AABB as many times as needed and then deletes it.
// For each split, it creates a new AABB from the leftovers with a gap size of
// one between the new and original AABB along the split axis.
//
// Attempts to split along the following planes in order: x0, x1, y0, y1, z0, z1
//
// Returns a pointer to the last AABB to be created.
AABB *splitAABB(Action *action, AABB *aabb) {
    // Split the AABB along the planes of the action surface
    // since we already checked if any of these values are out of bounds (and they aren't)
    // the split only needs to check if the plane falls within the bounds of the AABB,
    // specifically the one constant value in each of the 4 points.
    // For example, there are two XY planes, one at z0 and one at z1. Therefore to check
    // intersection of the z0 XY plane with the AABB, we only need to check if z0 
    // falls between AABBs z0 and z1.
    //
    // This is also going to be a >= or <= (inclusive) check since a plane laying on
    // the surface of the AABB still needs to be split to leave a size-1 gap between all
    // AABBs

    // x0
    if( (action->x0 > aabb->x0) && (action->x0 <= aabb->x1) ) {
        //printf("Splitting (%d, %d) (%d, %d) (%d, %d) along x0 %d\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1, action->x0);
        // creating split AABB
        AABB *split1 = createAABB(aabb->x0, action->x0-1, aabb->y0, aabb->y1, aabb->z0, aabb->z1);
        addAABB(aabb, split1);

        // Adjusting original AABB
        aabb->x0 = action->x0;
        //printf("Adjusted AABB x0 to (%d, %d) (%d, %d) (%d, %d)\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1);
    }

    // x1
    if( (action->x1 >= aabb->x0) && (action->x1 < aabb->x1) ) {
        //printf("Splitting (%d, %d) (%d, %d) (%d, %d) along x1 %d\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1, action->x1);
        // creating split AABB
        AABB *split1 = createAABB(action->x1+1, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1);
        addAABB(aabb, split1);

        // Adjusting original AABB
        aabb->x1 = action->x1;
        //printf("Adjusted AABB x1 to (%d, %d) (%d, %d) (%d, %d)\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1);
    }

    // y0
    if( (action->y0 > aabb->y0) && (action->y0 <= aabb->y1) ) {
        //printf("Splitting (%d, %d) (%d, %d) (%d, %d) along y0 %d\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1, action->y0);
        // creating split AABB
        AABB *split1 = createAABB(aabb->x0, aabb->x1, aabb->y0, action->y0-1, aabb->z0, aabb->z1);
        addAABB(aabb, split1);

        // Adjusting original AABB
        aabb->y0 = action->y0;
        //printf("Adjusted AABB y0 to (%d, %d) (%d, %d) (%d, %d)\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1);
    }

    // y1
    if( (action->y1 >= aabb->y0) && (action->y1 < aabb->y1) ) {
        //printf("Splitting (%d, %d) (%d, %d) (%d, %d) along y1 %d\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1, action->y1);
        // creating split AABB
        AABB *split1 = createAABB(aabb->x0, aabb->x1, action->y1+1, aabb->y1, aabb->z0, aabb->z1);
        addAABB(aabb, split1);

        // Adjusting original AABB
        aabb->y1 = action->y1;
        //printf("Adjusted AABB y1 to (%d, %d) (%d, %d) (%d, %d)\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1);
    }

    // z0
    if( (action->z0 > aabb->z0) && (action->z0 <= aabb->z1) ) {
        //printf("Splitting (%d, %d) (%d, %d) (%d, %d) along z0 %d\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1, action->z0);
        // creating split AABB
        AABB *split1 = createAABB(aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, action->z0-1);
        addAABB(aabb, split1);

        // Adjusting original AABB
        aabb->z0 = action->z0;
        //printf("Adjusted AABB z0 to (%d, %d) (%d, %d) (%d, %d)\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1);
    }

    // z1
    if( (action->z1 >= aabb->z0) && (action->z1 < aabb->z1) ) {
        //printf("Splitting (%d, %d) (%d, %d) (%d, %d) along z1 %d\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1, action->z1);
        // creating split AABB
        AABB *split1 = createAABB(aabb->x0, aabb->x1, aabb->y0, aabb->y1, action->z1+1, aabb->z1);
        addAABB(aabb, split1);

        // Adjusting original AABB
        aabb->z1 = action->z1;
        //printf("Adjusted AABB z1 to (%d, %d) (%d, %d) (%d, %d)\n", aabb->x0, aabb->x1, aabb->y0, aabb->y1, aabb->z0, aabb->z1);
    }

    AABB *next = aabb->prev;

    // delete the original
    removeAABB(aabb);

    return next;
}

int parseinput(FILE *fp, Action actions[500], int limit) {
    int linesize = 100;
    char line[linesize];
    int x0, x1, y0, y1, z0, z1;
    char state[3];
    int numactions = 0;

    while( fgets(line, linesize, fp) != NULL ) {
        sscanf(line, "%3s x=%d..%d,y=%d..%d,z=%d..%d\n", state, &x0, &x1, &y0, &y1, &z0, &z1);
        //printf("%s %d, %d, %d, %d, %d, %d\n", state, x0, x1, y0, y1, z0, z1);

        if( (x0 >= -limit) && (y0 >= -limit) && (z0 >= -limit) && 
            (x1 <= limit) && (y1 <= limit) && (z1 <= limit) ) {
            actions[numactions].state = (strcmp(state, "on") == 0) ? true : false;
            actions[numactions].x0 = x0;
            actions[numactions].x1 = x1;
            actions[numactions].y0 = y0;
            actions[numactions].y1 = y1;
            actions[numactions].z0 = z0;
            actions[numactions].z1 = z1;
            numactions++;
        }
    }
    return numactions;
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
    static Action actions[500];
    int numactions = parseinput(fp, actions, 50);

    // close file
    fclose(fp);

    bool grid[100][100][100];
    int offset = 49; // [49][49][49] represents 0, 0, 0

    int totalon = 0;

    for( int i = 0; i < numactions; i++ ) {
        for( int x = actions[i].x0; x <= actions[i].x1; x++ ) {
            for( int y = actions[i].y0; y <= actions[i].y1; y++ ) {
                for( int z = actions[i].z0; z <= actions[i].z1; z++ ) {
                    if( actions[i].state ) { // on action
                        if( !(grid[x + offset][y + offset][z + offset]) ) { // off grid
                            grid[x + offset][y + offset][z + offset] = true;
                            totalon++;
                        }
                    } else { // off action
                        if( (grid[x + offset][y + offset][z + offset]) ) { // on grid
                            grid[x + offset][y + offset][z + offset] = false;
                            totalon--;
                        }
                    }
                }
            }
        }
    }

    ans = totalon;
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
    static Action actions[500];
    int numactions = parseinput(fp, actions, INT_MAX);

    // close file
    fclose(fp);

    // Each cube is represented by an Axis Aligned Bounding Box (AABB)
    // The first 'On' action creates an AABB with the action bounds
    // Each subsequent action will check for intersections of existing AABBs
    // If an intersection is found, the intersected (existing) AABB will be split
    // into multiple, smaller AABBs based on the 6 planes that make up the faces of the
    // new action. Any AABB that falls within the current action's bounds will be removed.
    //
    // If the new intersecting action is 'Off', it will create the new AABBs via
    // splitting and remove any that fall within its bounds.
    // If the new intersecting action is 'On', it will do the same process, then create
    // an AABB with the action's bounds.
    //
    // AABBs are a doubly linked list. A new action traverses from head to tail. Any
    // deleted AABB is delinked and freed. Any new AABB is added to the tail
    // which will be checked for intersections later, as sometimes AABBs need to be
    // split several times along different planes from the same action.
    

    // first AABB doesnt need intersection checking
    HEAD = createAABBfromaction(&actions[0]);
    HEAD->prev = NULL;
    HEAD->next = NULL;

    AABB *curraabb; 
    AABB *nextaabb;
    Action *curraction;
    for( int i = 1; i < numactions; i++ ) {
        curraction = &actions[i];
        //printf("New action (%d, %d), (%d, %d), (%d, %d)\n", curraction->x0, curraction->x1, curraction->y0, curraction->y1, curraction->z0, curraction->z1);
        // check intersections with existing AABBs
        curraabb = HEAD;
        while( (curraabb != NULL) ) {
            if( actioncontains(curraction, curraabb) ) {
                nextaabb = curraabb->next;
                removeAABB(curraabb);
            } else if( actionintersects(curraction, curraabb) ) {
                nextaabb = splitAABB(curraction, curraabb);
            } else { nextaabb = curraabb->next; }
            curraabb = nextaabb;
        }
        if( curraction->state ) { // if the action is 'On', add it as an AABB.
            nextaabb = createAABBfromaction(&actions[i]);
            addAABB(HEAD, nextaabb);
            HEAD = nextaabb;
        }
    }
    //printf("Actions complete\n");

    // Calculating number of nodes turned on
    curraabb = HEAD;
    uint64_t totalnodes = 0;
    uint64_t currnodes = 0;
    while( (curraabb != NULL) ) {
        currnodes = 1;
        currnodes *= (curraabb->x1 - curraabb->x0 + 1);
        currnodes *= (curraabb->y1 - curraabb->y0 + 1);
        currnodes *= (curraabb->z1 - curraabb->z0 + 1);
        totalnodes += currnodes;
        //printf("AABB (%d, %d) (%d, %d) (%d, %d) added %lu nodes. Total %lu\n", curraabb->x0, curraabb->x1, curraabb->y0, curraabb->y1, curraabb->z0, curraabb->z1, currnodes, totalnodes);
        nextaabb = curraabb->next;
        free(curraabb);
        curraabb = nextaabb;
    }

    ans = totalnodes;
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

    start = clock();
    uint64_t p2 = parttwo(filename);
    clock_t p2time = clock();
    printf("Part 2: (%lf): %lu\n", (double)(p2time - start) / CLOCKS_PER_SEC, p2);
}
