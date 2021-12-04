#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct square {
    int value;
    int called;
};

struct board {
    struct square squares[5][5];
};

int partone (char *filename) {
    int ans = -1;
    int linesize = 1000;
    int nums[linesize];
    int totalnums = 0;
    char line[linesize];
    char *token;
    struct board boards[1000];
    int boardnum = 0;
    int scoresum;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // Read in first line of bingo numbers and parse
    fgets(line, linesize, fp);
    token = strtok(line, ", ");
    while (token != NULL) {
        nums[totalnums] = atoi(token);
        token = strtok(NULL, ", ");
        totalnums++;
    };

    // read in rest of files (boards)
    while ((fgets(line, linesize, fp)) != NULL) {
        // wait for a line with len 1 (gap between boards
        if (strlen(line) == 1) {
            // parsing a each of the 5 board lines
            for (int y = 0; y < 5; y++) {
                fgets(line, linesize, fp);
                // parsing each line into 5 values
                token = strtok(line, " ");
                for (int x = 0; x < 5; x++) {
                    boards[boardnum].squares[y][x].value = atoi(token);
                    boards[boardnum].squares[y][x].called = 0;
                    token = strtok(NULL, " ");
                }
            }
        boardnum++;
        }
    }

    // close file
    fclose(fp);

    int i, b;
    // run through bingo numbers
    for (i = 0; i < totalnums; i++) {
        // single number on all boards
        for (b = 0; b < boardnum; b++) {
            for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                    if (boards[b].squares[y][x].value == nums[i]) {
                        boards[b].squares[y][x].called = 1;
                        goto NUMBERCALLED;
                    }
                }
            }
            NUMBERCALLED:
            // check board for win condition
            // All one row
            for (int y = 0; y < 5; y++) {
                scoresum = 0;
                for (int x = 0; x < 5; x++) {
                    scoresum += boards[b].squares[y][x].called;
                }
                if (scoresum == 5) { goto WINCONDITION; }
            }

            // All one column
            for (int x = 0; x < 5; x++) {
                scoresum = 0;
                for (int y = 0; y < 5; y++) {
                    scoresum += boards[b].squares[y][x].called;
                }
                if (scoresum == 5) { goto WINCONDITION; }
            }
        }
    }
    
    WINCONDITION:
    // calculating winner score
    scoresum = 0;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            if (boards[b].squares[y][x].called == 0) {
                scoresum += boards[b].squares[y][x].value;
            }
        }
    }
    ans = scoresum * nums[i];
    return ans;
}







struct board2 {
    struct square squares[5][5];
    int won;
};

int parttwo (char *filename) {
    int ans = -1;
    int linesize = 1000;
    int nums[linesize];
    int totalnums = 0;
    char line[linesize];
    char *token;
    struct board2 boards[1000];
    int boardnum = 0;
    int scoresum;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // Read in first line of bingo numbers and parse
    fgets(line, linesize, fp);
    token = strtok(line, ", ");
    while (token != NULL) {
        nums[totalnums] = atoi(token);
        token = strtok(NULL, ", ");
        totalnums++;
    };

    // read in rest of files (boards)
    while ((fgets(line, linesize, fp)) != NULL) {
        // wait for a line with len 1 (gap between boards
        if (strlen(line) == 1) {
            // parsing a each of the 5 board lines
            for (int y = 0; y < 5; y++) {
                fgets(line, linesize, fp);
                // parsing each line into 5 values
                token = strtok(line, " ");
                for (int x = 0; x < 5; x++) {
                    boards[boardnum].squares[y][x].value = atoi(token);
                    boards[boardnum].squares[y][x].called = 0;
                    token = strtok(NULL, " ");
                }
            }
        boards[boardnum].won = 0;
        boardnum++;
        }
    }

    // close file
    fclose(fp);

    int i, b, remainingboards;
    remainingboards = boardnum;
    // run through bingo numbers
    for (i = 0; i < totalnums; i++) {
        // single number on all boards
        for (b = 0; b < boardnum; b++) {
            // only work on boards that havent won yet
            if (boards[b].won == 0) {
                for (int y = 0; y < 5; y++) {
                    for (int x = 0; x < 5; x++) {
                        if (boards[b].squares[y][x].value == nums[i]) {
                            boards[b].squares[y][x].called = 1;
                            goto NUMBERCALLED;
                        }
                    }
                }
                NUMBERCALLED:
                // check board for win condition
                // All one row
                for (int y = 0; y < 5; y++) {
                    scoresum = 0;
                    for (int x = 0; x < 5; x++) {
                        scoresum += boards[b].squares[y][x].called;
                    }
                    if (scoresum == 5) {
                        boards[b].won = 1;
                        remainingboards--;
                        if (remainingboards == 0) {
                            goto WINCONDITION;
                        } else {
                            goto NEXTBOARD;
                        }
                    }
                }

                // All one column
                for (int x = 0; x < 5; x++) {
                    scoresum = 0;
                    for (int y = 0; y < 5; y++) {
                        scoresum += boards[b].squares[y][x].called;
                    }
                    if (scoresum == 5) {
                        boards[b].won = 1;
                        remainingboards--;
                        if (remainingboards == 0) {
                            goto WINCONDITION;
                        } else {
                            goto NEXTBOARD;
                        }
                    }
                }
            }
            NEXTBOARD:;
        }
    }
    
    WINCONDITION:
    printf("WIN: %d, %d", nums[i], b);
    // calculating winner score
    scoresum = 0;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            if (boards[b].squares[y][x].called == 0) {
                scoresum += boards[b].squares[y][x].value;
            }
        }
    }
    ans = scoresum * nums[i];
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
        filename = "input.txt";
    }
    int p1 = partone(filename);
    printf("Part 1: %d\n", p1);
    int p2 = parttwo(filename);
    printf("Part 2: %d\n", p2);
}
