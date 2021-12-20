#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int beacons[50][3];
    int numbeacons;
    int distances[50][50];
    int pos[3];
    bool locked;
} Scanner;

typedef struct {
    int coords[3]; // relative to scanner 0
} Beacon;

int parseinput(FILE *fp, Scanner *scanners) {
    int linesize = 100;
    char line[100];
    int x, y, z;
    int numscanners = 0;

    while( fgets(line, linesize, fp) != NULL ) {
        while( fscanf(fp, "%d,%d,%d\n", &x, &y, &z) == 3 ) {
            int beaconnum = scanners[numscanners].numbeacons;
            scanners[numscanners].beacons[beaconnum][0] = x;
            scanners[numscanners].beacons[beaconnum][1] = y;
            scanners[numscanners].beacons[beaconnum][2] = z;
            scanners[numscanners].numbeacons++;
            //printf("Added %d, %d, %d to scanner %d beacon %d\n", x, y, z, numscanners, beaconnum);
        } // next scanner
        numscanners++;
    }
    return( numscanners );
}

bool contains(int val, int list[], int n) {
    for( int i = 0; i < n; i++ ) {
        if( val == list[i] ) { return( true ); }
    }
    return( false );
}

bool contains3(int val[3], int list[200][5], int n) {
    for( int i = 0; i < n; i++ ) {
        if( (val[0] == list[i][2]) && (val[1] == list[i][3]) && (val[2] == list[i][4]) ) {
            return( true );
        }
    }
    return( false );
}

// This is actually distance^2 to keep it ints
int calcdist( int c1[3], int c2[3] ) {
    int dist = 0;
    for( int i = 0; i < 3; i++ ) {
        dist += (c1[i]-c2[i])*(c1[i]-c2[i]);
    }
    return( dist );
}

// transform is the axis swap, [0, 1, 2] is no swap, [0, 2, 1] swaps Y and Z axis
// signs is the sign swap, [1, 1, 1] is no swap, [1, -1, 1] swaps the sign on the Y axis
// transform occurs first, then sign swap
void rotscanner(Scanner *s, int transform[3], int signs[3]) {
    int temp[3];
    for( int i = 0; i < s->numbeacons; i++ ) {
        printf("Transformed from (%d, %d, %d)", s->beacons[i][0], s->beacons[i][1], s->beacons[i][2]);

        // transform (axis swap)
        temp[0] = s->beacons[i][transform[0]];
        temp[1] = s->beacons[i][transform[1]];
        temp[2] = s->beacons[i][transform[2]];
        s->beacons[i][0] = temp[0];
        s->beacons[i][1] = temp[1];
        s->beacons[i][2] = temp[2];

        // sign swap
        s->beacons[i][0] *= signs[0];
        s->beacons[i][1] *= signs[1];
        s->beacons[i][2] *= signs[2];

        printf(" to (%d, %d, %d)\n", s->beacons[i][0], s->beacons[i][1], s->beacons[i][2]);
    }
}

// rotates s2 until all matched distances align with s1. The transforms compare the
// abs values of s1 and s2 matches. Then the s2 coordinate signs are adjusted to match
// the s1 values.
//
// Then all s2 beacon coordinates are adjusted with those parameters
// using that rotation.
void alignscanner(Scanner *s1, Scanner *s2, int matches[66][6], int nummatches) {
    int temp[3];
    // all possible position swaps
    int transforms[] = {3, 4, 5,
                        3, 5, 4,
                        4, 3, 5,
                        4, 5, 3,
                        5, 3, 4,
                        5, 4, 3};
    // finding matching transform
    for( int t = 0; t < 18; t += 3 ) {
        bool tmatch = true;
        for( int i = 0; i < nummatches; i++ ) {
            temp[0] = matches[i][transforms[t]];
            temp[1] = matches[i][transforms[t+1]];
            temp[2] = matches[i][transforms[t+2]];
            if( (abs(matches[i][0]) != abs(temp[0])) ||
                (abs(matches[i][1]) != abs(temp[1])) ||
                (abs(matches[i][2]) != abs(temp[2])) ) {
                tmatch = false;
                break;     
            }
        }
        if( tmatch ) {
            // calculating sign flips
            int signs[3];
            signs[0] = ((matches[11][0] * temp[0]) > 0) ? 1 : -1;
            signs[1] = ((matches[11][1] * temp[1]) > 0) ? 1 : -1;
            signs[2] = ((matches[11][2] * temp[2]) > 0) ? 1 : -1;
            int transform[] = {transforms[t]-3, transforms[t+1]-3, transforms[t+2]-3};
            printf("Match on all %d with t: (%d, %d, %d) and s: (%d, %d, %d)\n",
                   nummatches, transform[0], transform[1], transform[2], signs[0], signs[1], signs[2]);

            // rotating all beacon points
            rotscanner(s2, transform, signs);
            return;
        }
    }

}

// matches distance^2 between beacons, then aligning s2 beacon coordinates with s1
// axes
//
// Once s2 is aligned, finds the position of s2 relative to the origin
bool matchdist(Scanner *s1, Scanner *s2) {
    int numbeaconmatch = 0;
    // 12 matches, 3 axis distances from s1 and s2
    // {s1x1-s1x2, s1y1-s1y2, s1z1-s1z2, s2x1-s2x2, s2y1-s2y2, s2z1-s2z2}
    int beaconmatches[66][6] = { };
    int beaconids[24] = { };
    int numbeaconids = 0;
    for( int i1 = 0; i1 < s1->numbeacons-1; i1++ ) {
        for( int j1 = i1 + 1; j1 < s1->numbeacons; j1++ ) {
            for( int i2 = 0; i2 < s2->numbeacons-1; i2++ ) {
                for( int j2 = i2 + 1; j2 < s2->numbeacons; j2++ ) {
                    int dist1 = s1->distances[i1][j1];
                    int dist2 = s2->distances[i2][j2];
                    if( dist1 == dist2 ) {
                        // adding beacon ids
                        if( !(contains(i1, beaconids, numbeaconids)) ) {
                            beaconids[numbeaconids] = i1;
                            numbeaconids++;
                            printf("Unique beacon %d added\n", i1);
                        }
                        if( !(contains(j1, beaconids, numbeaconids)) ) {
                            beaconids[numbeaconids] = j1;
                            numbeaconids++;
                            printf("Unique beacon %d added\n", j1);
                        }
                        // adding beacon coordinates
                        for( int a = 0; a < 3; a++ ) {
                            beaconmatches[numbeaconmatch][a] = s1->beacons[i1][a] - s1->beacons[j1][a];
                            beaconmatches[numbeaconmatch][a+3] = s2->beacons[i2][a] - s2->beacons[j2][a];
                        }
                        numbeaconmatch++;
                        printf("Dist %d match between (%d, %d) and (%d, %d), Total Matches: %d\n",
                               dist1, i1, j1, i2, j2, numbeaconmatch);
                        if( numbeaconids >= 12 ) {
                            printf("%d beacons, %d matches found, starting beacon rotations\n", numbeaconids, numbeaconmatch);
                            alignscanner(s1, s2, beaconmatches, numbeaconmatch);

                            // finding s2 position
                            s2->pos[0] = (s1->beacons[i1][0] - s2->beacons[i2][0]) + s1->pos[0];
                            s2->pos[1] = (s1->beacons[i1][1] - s2->beacons[i2][1]) + s1->pos[1];
                            s2->pos[2] = (s1->beacons[i1][2] - s2->beacons[i2][2]) + s1->pos[2];

                            printf("s2 pos set to %d, %d, %d\n", s2->pos[0], s2->pos[1], s2->pos[2]);
                            printf("Locking s2\n");
                            s2->locked = true;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

int sortbeacons( const void *p1, const void *p2 )  {
    int* arr1 = (int*)p1;
    int* arr2 = (int*)p2;
    return( arr1[2] - arr2[2] );
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
    static Scanner scanners[50] = { };
    int numscanners = parseinput(fp, scanners);

    // close file
    fclose(fp);

    // for each beacon in a scanner, calculate the distance^2 to each other beacon
    // distance^2 is (x1-x2)^2 + (y1-y2)^2 + (z1-z2)^2
    // This will form a lower diagonal matrix in the 2D 'distances' array
    for( int s = 0; s < numscanners; s++ ) {
        for( int i = 0; i < scanners[s].numbeacons-1; i++ ) {
            for( int j = i+1; j < scanners[s].numbeacons; j++ ) {
                int dist = calcdist(scanners[s].beacons[i], scanners[s].beacons[j]);
                scanners[s].distances[i][j] = dist;
                //printf("%d to %d: %d\n", i, j, dist);
            }
        }
    }


    // setting scanner 0 as the origin and locking it
    scanners[0].pos[0] = 0; scanners[0].pos[1] = 0; scanners[0].pos[2] = 0;
    scanners[0].locked = true;
    int numlockedscanners = 1;

    // start matching beacon distances with other scanners. 
    //
    // When 12 distance matches are found with another beacon, start finding rotations
    // so that the (6 to 24) distance matched beacons align.
    // They will align when the distances between each axis (x, y, z) are
    // the same for each set of points.
    // For example:
    // scanner 0: (5, -10, 15), (-10, 5, -15), distance^2 = 1350, distances = (-15, 15, -30)
    // scanner 1: (-45, 15, 0), (-15, 0, 15), distance^2 = 1350, distances = (30, -15, 15)
    // By rotating scanner 1 through 2 rotations, it can be aligned with scanner 0
    // Rotation 1: Y axis +90 deg -> (0, 15, 45), (15, 0, 15), distances = (15, -15, -30)
    // Rotation 2: Z axis +180 deg -> (0, -15, 45), (-15, 0, 15), distances = (-15, 15, -30)
    //
    // Once the rotation is determined, the translation can be calculated from a single
    // matched point. For example:
    //      s0 point: (5, -10, 15), s1 point: (0, -15, 45), difference: (-5, -5, 30)
    //
    // Because s0 is pinned to (0, 0, 0), the location of s1 is (-5, -5, 30). If s0
    // was located at some other coordinates, the translation from origin could be
    // found by adding the known station's coordinates to the newly found station's
    // offset coordinates
    //
    // Once a station's beacon coordinates are rotated and its coordinates 
    // (relative to the origin) are known, it is considered 'locked'.
    while( numlockedscanners < numscanners ) {
        for( int s1 = 0; s1 < numscanners; s1++ ) {
            for( int s2 = 0; s2 < numscanners; s2++ ) {
                if( (s1 != s2) && (scanners[s1].locked) && !(scanners[s2].locked) ) {
                    printf("Starting distance matching between s%d and s%d\n", s1, s2);
                    if( matchdist(&scanners[s1], &scanners[s2]) ) {
                        numlockedscanners++;
                    }
                }
            }
        }
    }
    printf("All scanners locked\n");

    // Once all of the stations are locked then a comparison between the beacon
    // coordinates (adjusted for scanner location) can be made. The number of unique
    // coordinates among all scanners is total number of beacons
    // The distance matching, rotating, and translating should only occur between a
    // locked station and an unlocked station
    int allbeacons[200][5]; // s#, b#, x, y, z
    int uniquebeacons[200][5]; // s#, b#, x, y, z
    int numallbeacons = 0;
    int numuniquebeacons = 0;
    for( int s = 0; s < numscanners; s++ ) {
        for( int b = 0; b < scanners[s].numbeacons; b++ ) {
            int temp[3];
            temp[0] = scanners[s].pos[0] + scanners[s].beacons[b][0];
            temp[1] = scanners[s].pos[1] + scanners[s].beacons[b][1];
            temp[2] = scanners[s].pos[2] + scanners[s].beacons[b][2];
            allbeacons[numallbeacons][0] = s;
            allbeacons[numallbeacons][1] = b;
            allbeacons[numallbeacons][2] = temp[0];
            allbeacons[numallbeacons][3] = temp[1];
            allbeacons[numallbeacons][4] = temp[2];
            numallbeacons++;
            if( !(contains3(temp, uniquebeacons, numuniquebeacons)) ) {
                uniquebeacons[numuniquebeacons][0] = temp[0];
                uniquebeacons[numuniquebeacons][1] = temp[1];
                uniquebeacons[numuniquebeacons][2] = temp[2];
                numuniquebeacons++;
            }
        }
    }

    qsort(allbeacons, numallbeacons, sizeof(allbeacons[0]), sortbeacons);
    qsort(uniquebeacons, numuniquebeacons, sizeof(uniquebeacons[0]), sortbeacons);

    for( int b = 0; b < numuniquebeacons; b++ ) {
        printf("s%d  b%d  (%d, %d, %d)\n",
               uniquebeacons[b][0],
               uniquebeacons[b][1],
               uniquebeacons[b][2],
               uniquebeacons[b][3],
               uniquebeacons[b][4]);
    }
    printf("All beacons: %d, Unique beacons: %d\n", numallbeacons, numuniquebeacons);

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

    start = clock();
    int p2 = parttwo(filename);
    clock_t p2time = clock();
    printf("Part 2: (%lf): %d\n", (double)(p2time - start) / CLOCKS_PER_SEC, p2);
}
