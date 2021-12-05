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
    int linesize = 30;
    char line[linesize];
    int startx, starty, endx, endy;
    int grid[1000][1000];
    int nums = 0;

    while ((fgets(line, linesize, fp)) != NULL) {
        // x,y -> x,y
        startx = atoi(strtok(line, ",-> "));
        starty = atoi(strtok(NULL, ",-> "));
        endx = atoi(strtok(NULL, ",-> "));
        endy = atoi(strtok(NULL, ",-> "));
        //printf("%d, %d, %d, %d\n", startx, starty, endx, endy);

        // only continue if straight line
        if ((startx == endx) || (starty == endy)) {
            printf("%d, %d, %d, %d\n", startx, starty, endx, endy);
            // ugly if else to handle start > end, end > start, end = start
            //printf("[%d][%d] = %d", x, y, grid[x][y]);
            if (startx > endx) {
                for (int x = startx; x != endx-1; x--) {
                    if (grid[x][starty] == 1) { nums++; }
                    grid[x][starty]++;  // straight horizontal line, right to left
                    printf("[%d][%d] = %d\n", x, starty, grid[x][starty]);
                }
            } else if (startx < endx) {
                for (int x = startx; x != endx+1; x++) {
                    if (grid[x][starty] == 1) { nums++; }
                    grid[x][starty]++;  // straight horizontal line, left to right
                    printf("[%d][%d] = %d\n", x, starty, grid[x][starty]);
                }
            } else { // startx == endx, vertical line
                if (starty > endy) {
                    for (int y = starty; y != endy-1; y--) {
                        if (grid[startx][y] == 1) { nums++; }
                        grid[startx][y]++;  // straight vertical line, top to bottom
                        printf("[%d][%d] = %d\n", startx, y, grid[startx][y]);
                    }
                } else if (starty < endy) {
                    for (int y = starty; y != endy+1; y++) {
                        if (grid[startx][y] == 1) { nums++; }
                        grid[startx][y]++;  // straight vertical line, top to bottom
                        printf("[%d][%d] = %d\n", startx, y, grid[startx][y]);
                    }
                } else {
                    fprintf(stderr, "Line with no length: %d,%d -> %d,%d\n", startx, starty, endx, endy);
                }

            }

        }
    }

    // close file
    fclose(fp);

    ans = nums;
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
    int linesize = 30;
    char line[linesize];
    int startx, starty, endx, endy;
    int grid[1000][1000];
    int nums = 0;
    int x, y, incx, incy;

    while ((fgets(line, linesize, fp)) != NULL) {
        // x,y -> x,y
        startx = atoi(strtok(line, ",-> "));
        starty = atoi(strtok(NULL, ",-> "));
        endx = atoi(strtok(NULL, ",-> "));
        endy = atoi(strtok(NULL, ",-> "));
        //printf("%d, %d, %d, %d\n", startx, starty, endx, endy);

        printf("%d, %d, %d, %d\n", startx, starty, endx, endy);
        // ugly if else to handle start > end, end > start, end = start
        //printf("[%d][%d] = %d", x, y, grid[x][y]);
        if( startx > endx) { incx = -1; }
        else if( startx < endx) { incx = 1; }
        else { incx = 0; }

        if( starty > endy) { incy = -1; }
        else if( starty < endy) { incy = 1; }
        else { incy = 0; }

        // drawing lines
        if( incx == 0 ) { // vertical line
            for (int y = starty; y != endy + incy; y += incy) {
                if (grid[startx][y] == 1) { nums++; }
                grid[startx][y]++;
                printf("[%d][%d] = %d\n", startx, y, grid[startx][y]);
            }  
        } else if( incy == 0 ) { // horizontal line
            for (int x = startx; x != endx + incx; x += incx) {
                if (grid[x][starty] == 1) { nums++; }
                grid[x][starty]++;
                printf("[%d][%d] = %d\n", x, starty, grid[x][starty]);
            }  
        } else { // diagonal line, dx = dy
            x = startx;
            y = starty;
            for (int i = 0; i != ((endx - startx) * incx) + 1; i++) {
                if (grid[x][y] == 1) { nums++; }
                grid[x][y]++;
                printf("[%d][%d] = %d\n", x, y, grid[x][y]);
                x += incx;
                y += incy;
            } 
        }
    }

    // close file
    fclose(fp);

    ans = nums;
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
    //int p1 = partone(filename);
    //printf("Part 1: %d\n", p1);
    int p2 = parttwo(filename);
    printf("Part 2: %d\n", p2);
}
