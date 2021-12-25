#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#define TRACE 0
#define DEBUG 0

#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)


#define trace_print(fmt, ...) \
            do { if (TRACE) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

// converts an integer into a base26 string, A-Z
// 0 = A,  25 = Z,   26 = BA,  51 = BZ,  52 = CA,  675 = ZZ,  676 = BAAA
// 0 = A, -25 = -Z, -26 = -BA, 51 = -BZ, 52 = -CA, 675 = -ZZ, 676 = -BAAA
// ASCII chart A = 65, Z = 90
char *inttob26(char *str, int64_t val) {
    int64_t temp;
    strcpy(str, "");
    char str2[10];
    strcpy(str2, "");

    if( val < 0 ) {
        strcat(str, "-");
        val *= -1;
    }

    while( val > 25 ) {
        temp = val % 26;
        strcat(str2, (char[10]) { (char) temp+65, '\0' });
        val = val / 26;
    }
    strcat(str2, (char[10]) { (char) val+65, '\0' });

    // reversing str2 before concating on str
    char *str3;
    str3 = strrev(str2);
    
    strcat(str, str3);

    return str;
}

typedef enum {
    W=0,
    X=1,
    Y=2,
    Z=3,
} ArrInd;

typedef enum {
    INP,
    ADD,
    MUL,
    DIV,
    MOD,
    EQL,
} Op;

typedef enum {
    INT,
    ARRIND,
} Type;

typedef struct {
    Op op;

    ArrInd a;

    union {
        int64_t i;
        ArrInd ai;
    } b;
    Type btype;

} Ins;

// Runs all instructions. Final values for w-z stored in original vals array
//
// inputs are consumed from the inputs array one at a time.
int64_t *runprogram(Ins ins[300], int numins, int64_t vals[4], int inputs[14]) {
    int inputnum = 0;
    int64_t last = 0;

    for( int i = 0; i < numins; i++ ) {
        trace_print("[%d]  %d %d %ld    [%ld, %ld, %ld, %ld]\n",
                    i, ins[i].op, ins[i].a, ins[i].b.i, vals[0], vals[1], vals[2], vals[3]);

        int64_t b = (ins[i].btype == INT) ? ins[i].b.i : vals[ins[i].b.ai];
        switch (ins[i].op) {
            case INP: ;
                char str1[10], str2[10], str3[10], str4[10], strlast[10];
                inttob26(str1, vals[0]);
                inttob26(str2, vals[1]);
                inttob26(str3, vals[2]);
                inttob26(str4, vals[3]);
                inttob26(strlast, last);
                debug_print("INP %2d: %1d [%2ld, %2ld, %2ld, %12ld] (%12ld)    B26 [%s, %s, %s, %7s] (%s)\n",
                            inputnum, inputs[inputnum], vals[0], vals[1], vals[2], vals[3], last,
                            str1, str2, str3, str4, strlast);
                last = vals[3];
                vals[ins[i].a] = inputs[inputnum];
                inputnum++;
                break;
            case ADD: ;
                vals[ins[i].a] = vals[ins[i].a] + b;
                break;
            case MUL: ;
                vals[ins[i].a] = vals[ins[i].a] * b;
                break;
            case DIV: ;
                vals[ins[i].a] = vals[ins[i].a] / b;
                break;
            case MOD: ; 
                vals[ins[i].a] = vals[ins[i].a] % b;
                break;
            case EQL: ; 
                inttob26(str1, vals[0]);
                inttob26(str2, vals[1]);
                inttob26(str3, vals[2]);
                inttob26(str4, vals[3]);
                inttob26(strlast, last);
                debug_print("EQL %2d: %1d [%2ld, %2ld, %2ld, %12ld] (%12ld)    B26 [%s, %s, %s, %7s] (%s)\n",
                            inputnum-1, inputs[inputnum-1], vals[0], vals[1], vals[2], vals[3], last,
                            str1, str2, str3, str4, strlast);
                vals[ins[i].a] = (vals[ins[i].a] == b) ? 1: 0;
                break;
        }
    }
    return vals;
}

// parses input into passes Ins array, returns number of ins
int parseinput( FILE *fp, Ins ins[300]) {
    int linesize = 20;
    char line[linesize];
    char op[4], a, b[10];
    int numins = 0;

    while( fgets(line, linesize, fp) != NULL ) {
        sscanf(line, "%s %c %s\n", op, &a, b);
        // getting opcode enum
        if     ( strcmp(op, "inp") == 0 ) { ins[numins].op = INP; }
        else if( strcmp(op, "add") == 0 ) { ins[numins].op = ADD; }
        else if( strcmp(op, "mul") == 0 ) { ins[numins].op = MUL; }
        else if( strcmp(op, "div") == 0 ) { ins[numins].op = DIV; }
        else if( strcmp(op, "mod") == 0 ) { ins[numins].op = MOD; }
        else if( strcmp(op, "eql") == 0 ) { ins[numins].op = EQL; }
        else { fprintf(stderr, "Opcode %s not recognized\n", op); }

        // converting arg a to W-Z array index
        switch (a) {
            case 'w': 
                ins[numins].a = W;
                break;
            case 'x': 
                ins[numins].a = X;
                break;
            case 'y': 
                ins[numins].a = Y;
                break;
            case 'z': 
                ins[numins].a = Z;
                break;
            default:
                fprintf(stderr, "Arg a %c not recognized\n", a);
                break;
        }

        // converting arg b to int or W-Z array index
        // ASCII w = 119, z = 122
        if( ins[numins].op != INP ) {
            switch (b[0]) {
                case 'w': 
                    ins[numins].b.ai = W;
                    ins[numins].btype = ARRIND;
                    break;
                case 'x': 
                    ins[numins].b.ai = X;
                    ins[numins].btype = ARRIND;
                    break;
                case 'y': 
                    ins[numins].b.ai = Y;
                    ins[numins].btype = ARRIND;
                    break;
                case 'z': 
                    ins[numins].b.ai = Z;
                    ins[numins].btype = ARRIND;
                    break;
                default:
                    ins[numins].b.i = atoi(b);
                    ins[numins].btype = INT;
            }
        } else { ins[numins].b.i = 0; ins[numins].btype = INT; }
        numins++;
    }
    return( numins );
}

// lines can be read in one at a time. Since the w, x, y, z numbers are known at
// runtime, the char values can be checked for w-z, then swapped in with the int
// values before executing the line. Otherwise it's assumed they are ints and can
// be converted
//
// The opcode can be determined beforehand.
uint64_t partone (char *filename) {
    uint64_t ans = -1;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    //read file
    static Ins ins[300];
    int numins = parseinput(fp, ins);

    // close file
    fclose(fp);

    // running program
    int64_t vals[4] = { };
    int inputs[14] = {5, 3, 9, 9, 9, 9, 9, 5, 8, 2,  9,  3,  9,  9};
    //                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13
    
    runprogram(ins, numins, vals, inputs);

    char str1[10], str2[10], str3[10], str4[10];
    inttob26(str1, vals[0]);
    inttob26(str2, vals[1]);
    inttob26(str3, vals[2]);
    inttob26(str4, vals[3]);

    debug_print("Result: %ld, %ld, %ld, %ld    B26: %s, %s, %s, %s\n",
                vals[0], vals[1], vals[2], vals[3],
                str1, str2, str3, str4);

    ans = 0;
    uint64_t exp = 1;
    for( int i = 13; i >= 0; i-- ) {
        ans += inputs[i] * exp;
        exp *= 10;
    }

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

    //read file
    static Ins ins[300];
    int numins = parseinput(fp, ins);

    // close file
    fclose(fp);

    // running program
    int64_t vals[4] = { };
    int inputs[14] = {1, 1, 7, 2, 1, 1, 5, 1, 1, 1,  8,  1,  7,  5};
    //                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13
    
    runprogram(ins, numins, vals, inputs);

    char str1[10], str2[10], str3[10], str4[10];
    inttob26(str1, vals[0]);
    inttob26(str2, vals[1]);
    inttob26(str3, vals[2]);
    inttob26(str4, vals[3]);

    debug_print("Result: %ld, %ld, %ld, %ld    B26: %s, %s, %s, %s\n",
                vals[0], vals[1], vals[2], vals[3],
                str1, str2, str3, str4);

    ans = 0;
    uint64_t exp = 1;
    for( int i = 13; i >= 0; i-- ) {
        ans += inputs[i] * exp;
        exp *= 10;
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
    uint64_t p1 = partone(filename);
    clock_t p1time = clock();
    printf("Part 1 (%lf): %lu\n", (double)(p1time - start) / CLOCKS_PER_SEC, p1);

    start = clock();
    uint64_t p2 = parttwo(filename);
    clock_t p2time = clock();
    printf("Part 2: (%lf): %lu\n", (double)(p2time - start) / CLOCKS_PER_SEC, p2);
}
