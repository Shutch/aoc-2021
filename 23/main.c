#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#define DEBUG 0
#define TRACE 0

#if DEBUG > 0
    #define debug_print(...) fprintf(stderr, __VA_ARGS__);
#else
    #define debug_print(...)
#endif

#if TRACE > 0
    #define trace_print(...) fprintf(stderr, __VA_ARGS__);
#else
    #define trace_print(...)
#endif

//    #############
//    #...........#
//    ###A#B#C#D###
//      #A#B#C#D#  
//      #A#B#C#D#  
//      #A#B#C#D#
//      #########
// Top row is index 0-10 (left to right)
// A column is 11-14 (top to bottom)
// B column is 15-18 (top to bottom)
// C column is 19-22 (top to bottom)
// D column is 23-26 (top to bottom)

// Paths
// 0 -> 1
// 1 -> 0, 2
// 2 -> 1, 3, 11
// 3 -> 2, 4
// 4 -> 3, 5, 13
// 5 -> 4, 6
// 6 -> 5, 7, 15
// 7 -> 6, 8
// 8 -> 7, 9, 17
// 9 -> 8, 10
// 10 -> 9
//
// 11 -> 2, 12
// 12 -> 11, 13
// 13 -> 12, 14
// 14 -> 13
//
// 15 -> 4, 16
// 16 -> 15, 17
// 17 -> 16, 18
// 18 -> 17
//
// 19 -> 6, 20
// 20 -> 19, 21
// 21 -> 20, 22
// 22 -> 21
//
// 23 -> 8, 24
// 24 -> 23, 25
// 25 -> 24, 26
// 26 -> 25

int paths[27][4] = {
    {1, -1, -1, -1},     // 0
    {0,  2, -1, -1},     // 1
    {1,  3,  11, -1},    // 2
    {2,  4, -1, -1},     // 3
    {3,  5,  15, -1},    // 4
    {4,  6, -1, -1},     // 5
    {5,  7,  19, -1},    // 6
    {6,  8, -1, -1},     // 7
    {7,  9,  23, -1},    // 8
    {8,  10, -1, -1},    // 9
    {9, -1, -1, -1},     // 10

    {2,  12, -1, -1},    // 11
    {11, 13, -1, -1},    // 12
    {12, 14, -1, -1},    // 13
    {13, -1, -1, -1},    // 14

    {4,  16, -1, -1},    // 15
    {15, 17, -1, -1},    // 16
    {16, 18, -1, -1},    // 17
    {17, -1, -1, -1},    // 18

    {6,  20, -1, -1},    // 19
    {19, 21, -1, -1},    // 20
    {20, 22, -1, -1},    // 21
    {21, -1, -1, -1},    // 22

    {8,  24, -1, -1},    // 23
    {23, 25, -1, -1},    // 24
    {24, 26, -1, -1},    // 25
    {25, -1, -1, -1},    // 26
};


typedef struct {
    char type;
    int movecost;
    int pos;
    bool locked;
} Piece;

typedef struct {
    char spaces[27];
    Piece pieces[16];
    int score;
} Board;

typedef struct {
    uint8_t a1 : 5;
    uint8_t a2 : 5;
    uint8_t a3 : 5;
    uint8_t a4 : 5;
    uint8_t b1 : 5;
    uint8_t b2 : 5;
    uint8_t b3 : 5;
    uint8_t b4 : 5;
    uint8_t c1 : 5;
    uint8_t c2 : 5;
    uint8_t c3 : 5;
    uint8_t c4 : 5;
    uint8_t d1 : 5;
    uint8_t d2 : 5;
    uint8_t d3 : 5;
    uint8_t d4 : 5;
} State;

void printboard(Board *board) {

    char *spaces = board->spaces;
    //printf("#############\n#");
    printf("\n");
    for( int i = 0; i <= 10; i++ ) {
        printf("%c", (spaces[i] == 0) ? '.' : spaces[i]);
    }
    printf("\n ");
    for( int i = 11; i <= 23; i += 4 ) {
        printf(" %c", (spaces[i] == 0) ? '.' : spaces[i]);
    }
    printf("  \n ");
    for( int i = 12; i <= 24; i += 4 ) {
        printf(" %c", (spaces[i] == 0) ? '.' : spaces[i]);
    }
    printf("  \n ");
    for( int i = 13; i <= 25; i += 4 ) {
        printf(" %c", (spaces[i] == 0) ? '.' : spaces[i]);
    }
    printf("  \n ");
    for( int i = 14; i <= 26; i += 4 ) {
        printf(" %c", (spaces[i] == 0) ? '.' : spaces[i]);
    }
    printf("  \n\n");
    //printf("#  \n  #########  \n");
}

void parseinput(FILE *fp, Board *board) {
    int linesize = 50;
    char line[linesize];

    fgets(line, linesize, fp); // throwaway
    fgets(line, linesize, fp); // throwaway
    fscanf(fp, "###%c#%c#%c#%c###",
           &board->spaces[11], &board->spaces[15], &board->spaces[19], &board->spaces[23]);
    fscanf(fp, "  #%c#%c#%c#%c#  ",
           &board->spaces[14], &board->spaces[18], &board->spaces[22], &board->spaces[26]);

    // adding pt 2
    board->spaces[12] = 'D';
    board->spaces[13] = 'D';
    board->spaces[16] = 'C';
    board->spaces[17] = 'B';
    board->spaces[20] = 'B';
    board->spaces[21] = 'A';
    board->spaces[24] = 'A';
    board->spaces[25] = 'C';

    // debug
    //board->spaces[12] = 'A';
    //board->spaces[13] = 'A';
    //board->spaces[16] = 'B';
    //board->spaces[17] = 'B';
    //board->spaces[20] = 'C';
    //board->spaces[21] = 'C';
    //board->spaces[24] = 'D';
    //board->spaces[25] = 'D';

    // filling out pieces info
    for( int i = 0; i < 16; i++ ) {
        board->pieces[i].type = board->spaces[i+11];
        board->pieces[i].pos = i+11;
        board->pieces[i].locked = false;
        // debug
        //if( i+11 == 19 || i+11 == 23 ) {
        //    board->pieces[i].locked = false;
        //} else {
        //    board->pieces[i].locked = true;
        //}
        switch( board->pieces[i].type ) {
            case 'A':
                board->pieces[i].movecost = 1;
                break;
            case 'B':
                board->pieces[i].movecost = 10;
                break;
            case 'C':
                board->pieces[i].movecost = 100;
                break;
            case 'D':
                board->pieces[i].movecost = 1000;
                break;
        }
    }
}

bool validnextspace(char board[19], int nextspace) {
    // unoccupied, open space
    if( board[nextspace] != 0 ) {
        return false;
    }
    return true;
}

// validates final space plus all spaces to that space
bool validfinalspace(char piece, char board[19], int startspace, int finalspace) {
    trace_print("Validating %c from %d to %d\n", piece, startspace, finalspace);

    // not the same space
    trace_print("Checking finalspace != startspace\n");
    if( finalspace == startspace ) {
        return false;
    }

    // unoccupied, open space
    trace_print("Checking finalspace is not occupied\n");
    if( board[finalspace] != 0 ) {
        return false;
    }

    // not space outside of a room, indexes 2, 4, 6, 8
    trace_print("Checking finalspace is not outside a room\n");
    if( (finalspace == 2) || (finalspace == 4) || (finalspace == 6) || (finalspace == 8) ) {
        return false;
    }

    // not a room they dont belong in (A: 11-14), (B: 15-18), (C: 19-22), (D: 23-26)
    trace_print("Checking piece belongs in finalspace\n");
    switch (piece) {
        case 'A':
            if( !(finalspace <= 10 || (finalspace >= 11 && finalspace <= 14)) ) { return false; }
            break;
        case 'B':
            if( !(finalspace <= 10 || (finalspace >= 15 && finalspace <= 18)) ) { return false; }
            break;
        case 'C':
            if( !(finalspace <= 10 || (finalspace >= 19 && finalspace <= 22)) ) { return false; }
            break;
        case 'D':
            if( !(finalspace <= 10 || (finalspace >= 23 && finalspace <= 26)) ) { return false; }
            break;
        default:
            fprintf(stderr, "Unexpected character %c (%d)\n", piece, piece);
    }

    // room doesn't already contain an amphipod that doesn't belong
    trace_print("Checking room doesnt already contain amphipod that doesn't belong\n");
    switch (finalspace) {
        case 11: case 12: case 13: case 14:
            for( int i = 11; i <= 14; i++ ) {
                if( !(board[i] == 0 || board[i] == 'A') ) { return false; }
            }
            break;
        case 15: case 16: case 17: case 18:
            for( int i = 15; i <= 18; i++ ) {
                if( !(board[i] == 0 || board[i] == 'B') ) { return false; }
            }
            break;
        case 19: case 20: case 21: case 22:
            for( int i = 19; i <= 22; i++ ) {
                if( !(board[i] == 0 || board[i] == 'C') ) { return false; }
            }
            break;
        case 23: case 24: case 25: case 26:
            for( int i = 23; i <= 26; i++ ) {
                if( !(board[i] == 0 || board[i] == 'D') ) { return false; }
            }
            break;
        default:
            break;
    }

    // if not headed to a room, hasn't stopped moving in the hallway
    // If headed to a room, not deviating from room
    trace_print("Checking startspace and finalspace arent both in the hallway\n");
    if( startspace <= 10 && finalspace <= 10 ) {
        return false;
    }

    // if leaving room, leaves to hallway space, doesn't go elsewhere in the room
    trace_print("Checking startspace and finalspace arent both in a room\n");
    if( startspace >= 11 & finalspace >= 11 ) {
        return false;
    }

    // Isn't headed towards the bottom-most slot in a room it belongs to
    trace_print("Checking all spaces below finalspace in room are occupied\n");
    switch (finalspace) {
        case 11: case 12: case 13:
            for( int i = finalspace+1; i <= 14; i++ ) {
                if( !(board[i] == 'A') ) { return false; }
            }
            break;
        case 15: case 16: case 17:
            for( int i = finalspace+1; i <= 18; i++ ) {
                if( !(board[i] == 'B') ) { return false; }
            }
            break;
        case 19: case 20: case 21:
            for( int i = finalspace+1; i <= 22; i++ ) {
                if( !(board[i] == 'C') ) { return false; }
            }
            break;
        case 23: case 24: case 25:
            for( int i = finalspace+1; i <= 26; i++ ) {
                if( !(board[i] == 'D') ) { return false; }
            }
            break;
        default:
            break;
    }

    return true;
}

// returns true or false if the piece should be locked.
// This should only be used after a move
bool lockpiece(char piece, char board[19], int currspace) {
    switch(piece) {
        case 'A':
            if( currspace >= 11 && currspace <= 14 ) { return true; };
            break;
        case 'B':
            if( currspace >= 15 && currspace <= 18 ) { return true; };
            break;
        case 'C':
            if( currspace >= 19 && currspace <= 22 ) { return true; };
            break;
        case 'D':
            if( currspace >= 23 && currspace <= 26 ) { return true; };
            break;
        default:
            fprintf(stderr, "Unexpected character %c (%d)\n", piece, piece);
    }
    return false;
}

bool validfinalboard(char board[19]) {
    if( 'A' == board[11] && 'A' == board[12] &&
        'A' == board[13] && 'A' == board[14] &&
        'B' == board[15] && 'B' == board[16] &&
        'B' == board[17] && 'B' == board[18] &&
        'C' == board[19] && 'C' == board[20] &&
        'C' == board[21] && 'C' == board[22] &&
        'D' == board[23] && 'D' == board[24] &&
        'D' == board[25] && 'D' == board[26] ) {
        return true;
    }
    return false;
}

// returns an int with the cost of moving to the specified square
// If there is no path then returns 0.
int calcmovecost(char piece, char board[19], int startspace, int finalspace, int movecost) {
    debug_print("Calculating move cost for %c from %d to %d\n", piece, startspace, finalspace);
    struct Path {
        int currspace;
        int prevspace;
        int cost;
    };
    
    // BFS for the space, first hit will be the shortest route
    struct Path pathlist[31] = { };
    int pathhead = 0;
    int pathtail = 0;
    
    // populating initial list from startspace
    int i = 0;
    while( paths[startspace][i] != -1 ) {
        if( validnextspace(board, paths[startspace][i]) ) {
            pathlist[pathtail].currspace = paths[startspace][i];
            pathlist[pathtail].prevspace = startspace;
            pathlist[pathtail].cost = movecost;
            trace_print("Initial path %d to %d added\n", pathtail, paths[startspace][i]);
            pathtail++;
            if( pathtail > 30 ) {
                debug_print("Pathtail > 30, no path found.\n");
                return( 0 );
            }
        }
        i++;
    }
    if( pathtail == 0 ) {  // no valid paths from start
        debug_print("No valid path from start found\n");
        return( 0 );
    }

    // bfs of valid paths
    // a new path is added for each move. The array 'head' is moved up by 1 as a
    // path is updated.
    int numvalidpaths;
    struct Path *currpath;
    do {
        numvalidpaths = 0;
        for( int i = pathhead; i < pathtail; i++ ) {
            currpath = &pathlist[i];
            if( currpath->currspace == finalspace ) {
                debug_print("Path found, cost: %d\n", currpath->cost);
                return( currpath->cost );
            }

            int j = 0;
            while( paths[currpath->currspace][j] != -1 ) { // iterate through list of next spaces
                // check if next move is valid
                if( (validnextspace(board, paths[currpath->currspace][j])) && 
                    (paths[currpath->currspace][j] != currpath->prevspace) ) {
                        pathlist[pathtail].currspace = paths[currpath->currspace][j];
                        pathlist[pathtail].prevspace = currpath->currspace;
                        pathlist[pathtail].cost = currpath->cost + movecost;
                        trace_print("Continued path %d. From %d to %d, cost %d\n",
                                    pathtail, currpath->currspace, paths[currpath->currspace][j], pathlist[pathtail].cost);
                        pathtail++;
                        if( pathtail > 30 ) {
                            debug_print("Pathtail > 30, no path found.\n");
                            return( 0 );
                        }
                }
                j++;
            }
        }
    } while( numvalidpaths > 0 );

    debug_print("No valid paths found\n");
    return( 0 );  // no valid paths found
}

// runs the game to completion, recursively
void rungame(Board *board, int *lowscore, State *memo) {
    int numpieces = 16;
    int numspaces = 27;
    int numvalidmoves = 0;
    Board *nextboard = (Board *) malloc(sizeof(Board));;

    //printf("Score: %d\n", board->score);
    //printboard(board);

    for( int i = 0; i < numpieces; i++ ) {
        trace_print("Looking for moves for %c at %d\n", board->pieces[i].type, board->pieces[i].pos);
        if( !(board->pieces[i].locked) ) {
            for( int j = 0; j < numspaces; j++ ) {
                bool valid = validfinalspace(board->pieces[i].type,
                                             board->spaces,
                                             board->pieces[i].pos,
                                             j);
                if( valid ) {
                    int movecost = calcmovecost(board->pieces[i].type,
                                                board->spaces,
                                                board->pieces[i].pos,
                                                j,
                                                board->pieces[i].movecost);
                    if( movecost > 0 ) {
                        if( (board->score + movecost) < *lowscore ) {
                            debug_print("Next move: %c from %d to %d\n",
                                        board->pieces[i].type, board->pieces[i].pos, j);
                            numvalidmoves++;

                            // create next board with moves and cost
                            memcpy(nextboard, board, sizeof(Board));
                            nextboard->spaces[nextboard->pieces[i].pos] = 0;
                            nextboard->spaces[j] = nextboard->pieces[i].type;
                            nextboard->pieces[i].pos = j;
                            nextboard->score += movecost;

                            // check if board is a winner
                            if( validfinalboard(nextboard->spaces) ) {
                                int score = nextboard->score;
                                printf("***Game complete. Score: %d\n", score);
                                //printboard(nextboard);
                                if( score < *lowscore ) {
                                    *lowscore = score;
                                }
                                free(nextboard);
                                return;
                            }

                            // else dive deeper into next move with updated copy board
                            bool lock = lockpiece(nextboard->pieces[i].type,
                                                  nextboard->spaces,
                                                  nextboard->pieces[i].pos);
                            if( lock ) {
                                debug_print("Locking %c at %d\n", nextboard->pieces[i].type, nextboard->pieces[i].pos);
                                nextboard->pieces[i].locked = true;
                            }
                            rungame(nextboard, lowscore, memo);
                        }
                    }
                }
            }
        }
    }
    // if no valid moves left, this game cant be won so the score is set high
    if( numvalidmoves == 0 ) {
        //printf("***No more valid moves on this branch\n");
        free(nextboard);
        return;
    }
    // if the game has multiple winning branches, return the lowest score
    free(nextboard);
    return;
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

    // close file
    fclose(fp);

    // To capture a game state only the position of each piece and the current score
    // needs to be recorded. For faster checks of where a piece can move it may be
    // better to instead record the piece in each of the 19 game spaces.
    //
    // In order to optimize strategy the move history will need to be recorded, at
    // the very least the previous move(s) and the total number of moves.
    //
    // This begins to take the shape of a tree with multiple child nodes under each
    // parent node. The maximum number of child nodes is the maximum number of allowable
    // moves each turn. With 8 pieces and a potential maximum of 3 moves per piece (even
    // though this would never actually occur), each node should have an array of length
    // 24 that can contain pointers to child nodes. A start and end index can be used
    // to keep track of which child nodes have been visited
    //
    // The game could also be played backwards, with the final position known, and
    // the number of steps required to get to the starting position calculated.
    //
    // There are two levels to making a move, final destination and steps to reach it.
    // 
    // Each individual move needs to be acceptable, for example moving from one room
    // to another, there can't be any other pieces between the two rooms, the destination
    // room must match the current piece type.
    //
    // Also the final destination needs to be acceptable. So the final destination
    // can't be the space directly outside of the room despite that path being clear
    // Final moves should be tested, first that they are allowed, then that every
    // move along the way is allowed. If either of these are false, the move series is
    // illegal.
    //
    // The naive approach would be to build a list of possible moves and work through them,
    // starting with the lowest value.

    // did it by hand lol
    ans = 10607;
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

    Board board;
    board.score = 0;

    // read file
    parseinput(fp, &board);

    // close file
    fclose(fp);

    uint64_t memosize = 15460335891000;
    State *memo = (State*) malloc(memosize * sizeof(State));

    //run games recursively
    int lowscore = INT_MAX;
    rungame(&board, &lowscore, memo);

    free(memo);

    ans = lowscore;
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
