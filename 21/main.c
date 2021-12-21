#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

void parseinput(FILE *fp, int *p1s, int *p2s) {
    fscanf(fp, "Player 1 starting position: %d\n", p1s);
    fscanf(fp, "Player 2 starting position: %d\n", p2s);
}

typedef struct {
    int space;
    int score;
} Player;

typedef struct {
    uint64_t p1wins;
    uint64_t p2wins;
    bool complete;
} Game;

int rolldie(int die) {
    return( (die % 100) + 1);
}

int movepiece(int startspace, int nummoves) {
    return( ((startspace + nummoves - 1) % 10) + 1 );
}

void playturn(int p1space, int p2space, int p1score, int p2score,
              int turn, Game memo[11][11][21][21][2]) {
    int maxscore = 2;

    Game *currstate = &memo[p1space][p2space][p1score][p2score][turn];

    // 27 permutations of 3 dice rolls
    // if a given move leads to a winning game state, add a win to the player
    // if it doesn't, check for the next gamestate in the memo
    // if it is complete, add the wins from both players to the current state
    // if it isn't complete, dive deeper and repeat the process for the next game state.
    for( int i = 1; i <= 3; i++ ) { 
        for( int j = 1; j <= 3; j++ ) { 
            for( int k = 1; k <= 3; k++ ) { 
                int moves = i + j + k;
                if( turn == 0 ) {
                    int nextspace = movepiece(p1space, moves);
                    int nextscore = p1score + nextspace;
                    if( nextscore >= maxscore ) {
                        currstate->p1wins++;
                        printf("Player 1 win (Spaces: %d, %d, Scores: %d, %d, Turn: %d, Wins: %lu, %lu)\n",
                               nextspace, p2space, nextscore, p2score, turn,
                               currstate->p1wins, currstate->p2wins);
                    } else {
                        int nextturn = 1;
                        Game *nextstate = &memo[nextspace][p2space][nextscore][p2score][nextturn];
                        if( nextstate->complete ) { // check memo for answer
                            currstate->p1wins += nextstate->p1wins;
                            currstate->p2wins += nextstate->p2wins;
                            printf("Memo used (Spaces: %d, %d, Scores: %d, %d, Turn: %d, Wins: %lu, %lu)\n",
                                   nextspace, p2space, nextscore, p2score, nextturn,
                                   currstate->p1wins, currstate->p2wins);
                        } else { // not memoized yet, continue to calculate
                            printf("Diving deeper (Spaces: %d, %d, Scores: %d, %d, Turn: %d, Wins: %lu, %lu)\n",
                                   nextspace, p2space, nextscore, p2score, nextturn,
                                   currstate->p1wins, currstate->p2wins);
                            playturn(nextspace, p2space, nextscore, p2score, nextturn, memo);
                            currstate->p1wins += nextstate->p1wins;
                            currstate->p2wins += nextstate->p2wins;
                            printf("Back from (Spaces: %d, %d, Scores: %d, %d, Turn: %d, Wins: %lu, %lu)\n",
                                   nextspace, p2space, nextscore, p2score, nextturn,
                                   currstate->p1wins, currstate->p2wins);
                        }
                    }
                } else if( turn == 1 ) {
                    int nextspace = movepiece(p2space, moves);
                    int nextscore = p2score + nextspace;
                    if( nextscore >= maxscore ) {
                        currstate->p2wins++;
                        printf("Player 2 win (Spaces: %d, %d, Scores: %d, %d, Turn: %d, Wins: %lu, %lu)\n",
                               p1space, nextspace, p1score, nextscore, turn,
                               currstate->p1wins, currstate->p2wins);
                    } else {
                        int nextturn = 0;
                        Game *nextstate = &memo[p1space][nextspace][p1score][nextscore][nextturn];
                        if( nextstate->complete ) { // check memo for answer
                            currstate->p1wins += nextstate->p1wins;
                            currstate->p2wins += nextstate->p2wins;
                            printf("Memo used (Spaces: %d, %d, Scores: %d, %d, Turn: %d, Wins: %lu, %lu)\n",
                                   p1space, nextspace, p1score, nextscore, nextturn,
                                   currstate->p1wins, currstate->p2wins);
                        } else { // not memoized yet, continue to calculate
                            printf("Diving deeper (Spaces: %d, %d, Scores: %d, %d, Turn: %d, Wins: %lu, %lu)\n",
                                   p1space, nextspace, p1score, nextscore, nextturn,
                                   currstate->p1wins, currstate->p2wins);
                            playturn(p1space, nextspace, p1score, nextscore, nextturn, memo);
                            currstate->p1wins += nextstate->p1wins;
                            currstate->p2wins += nextstate->p2wins;
                            printf("Back from (Spaces: %d, %d, Scores: %d, %d, Turn: %d, Wins: %lu, %lu)\n",
                                   p1space, nextspace, p1score, nextscore, nextturn,
                                   currstate->p1wins, currstate->p2wins);
                        }
                    }
                } else { fprintf(stderr, "Turn %d not recognized\n", turn); }
            }
        }
    }
    currstate->complete = true;
    printf("Game state complete (Spaces: %d, %d, Scores: %d, %d, Turn: %d, Wins: %lu, %lu)\n",
           p1space, p2space, p1score, p2score, turn,
           currstate->p1wins, currstate->p2wins);
}

uint64_t playgame(int p1space, int p2space) {
    int p1score = 0;
    int p2score = 0;
    int turn = 0;

    // memoization of outcomes
    // Args: p1space, p2space, p1score, p2score, turn
    // Returns p1wins, p2wins
    //
    // Memoization will record a specific game state (before victory) and return
    // the number of wins from p1
    //
    // Ranges of inputs:
    // p1space 1-10
    // p2space 1-10
    // p1score 0-20
    // p2score 0-20
    // turn 0-1
    // [11][11][21][21][2]
    //
    // Each location in the array will hold a struct that contains
    // p1wins, p2wins, flag if this case has been run before
    
    static Game memo[11][11][21][21][2] = { };

    playturn(p1space, p2space, p1score, p2score, turn, memo);


    uint64_t p1wins = memo[p1space][p2space][p1score][p2score][turn].p1wins;
    uint64_t p2wins = memo[p1space][p2space][p1score][p2score][turn].p2wins;

    if( p1wins > p2wins ) {
        return( p1wins );
    } else {
        return( p2wins );
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
    int p1s, p2s;
    parseinput(fp, &p1s, &p2s);

    // close file
    fclose(fp);

    Player p1, p2;
    p1.space = p1s;
    p1.score = 0;
    p2.space = p2s;
    p2.score = 0;
    int die = 0; // starts at 1
    int moves;
    int dierolls = 0;

    int maxscore = 1000;
    while( 1 ) {
        // player 1
        moves = 0;
        for( int i = 0; i < 3; i++ ) {
            die = rolldie(die);
            moves += die;
            dierolls++;
        }
        p1.space = movepiece(p1.space, moves);
        p1.score += p1.space;
        //printf("Player 1 moves %d to square %d. Score: %d, Die: %d\n", moves, p1.space, p1.score, die);

        if( p1.score >= 1000 ) {
            return( dierolls * p2.score );
        }

        // player 2
        moves = 0;
        for( int i = 0; i < 3; i++ ) {
            die = rolldie(die);
            moves += die;
            dierolls++;
        }
        p2.space = movepiece(p2.space, moves);
        p2.score += p2.space;
        //printf("Player 2 moves %d to square %d. Score: %d, Die: %d\n", moves, p2.space, p2.score, die);

        if( p2.score >= 1000 ) {
            return( dierolls * p1.score );
        }
    }
    return ans;
}

uint64_t parttwo (char *filename) {
    int ans = -1;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file
    int p1space, p2space;
    parseinput(fp, &p1space, &p2space);

    // close file
    fclose(fp);

    uint64_t winnerscore = playgame(p1space, p2space);

    ans = winnerscore;
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
