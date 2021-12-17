#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

int mod(int a, int b) {
    int m = a % b;
    if (m < 0) {
        m = (b < 0) ? m - b : m + b;
    }
    return (m);
}

int intpow(int b, int e) {
    int result = 1;
    for( int i = 0; i < e; i++ ) {
        result *= b;
    }
    return result;
}

// gets a certain number of bits from the full buffer array
uint32_t getbits(uint32_t *buffer, int *bitoffset, int numbits) {
    uint32_t val = 0;
    uint32_t b = buffer[*bitoffset/32];
    uint32_t rembits = 32 - (*bitoffset % 32);
    if( rembits > numbits ) { // requested bits fall entirely in current 4 bytes
        uint32_t maskshift = rembits - numbits;
        val = (b & ((intpow(2, numbits)-1) << maskshift)) >> maskshift;
    } else { // bring in next 4 bytes to complete requested bits
        uint32_t maskshift = numbits - rembits;
        val = (b & (intpow(2, rembits)-1)) << maskshift;
        b = buffer[(*bitoffset/32) + 1];
        maskshift = 32 - (numbits - rembits);
        val = val + ((b & ((intpow(2, numbits-rembits)-1) << maskshift)) >> maskshift);
    }
    *bitoffset += numbits;
    return( val );
}

uint32_t literalpacket(uint32_t *buffer, int *bitoffset);
uint32_t operatorpacket(uint32_t *buffer, int *bitoffset);
uint32_t parsepacket(uint32_t *buffer, int *bitoffset);

uint32_t literalpacket(uint32_t *buffer, int *bitoffset) {
    uint32_t cont = 1;
    uint32_t val = 0;
    //printf("Starting Literal @ %d\n", *bitoffset);
    do {
        cont = getbits(buffer, bitoffset, 1);
        val = (val << 4) + getbits(buffer, bitoffset, 4);
    } while (cont == 1);
    //printf("Literal Complete: val %d @ %d\n", val, *bitoffset);
    return(val);
}

uint32_t operatorpacket(uint32_t *buffer, int *bitoffset) {
    uint32_t versum = 0;
    uint32_t lenID = getbits(buffer, bitoffset, 1);
    if( lenID == 0 ) {
        uint32_t subpacklen = getbits(buffer, bitoffset, 15);
        int startoffset = *bitoffset;
        //printf("Starting Operator 0: len %d @ %d\n", subpacklen, *bitoffset);
        while( (*bitoffset - startoffset) < subpacklen ) {
            versum += parsepacket(buffer, bitoffset);
        }
        //printf("Operator Complete 0: len %d @ %d\n", subpacklen, *bitoffset);
    } else {
        uint32_t totalsubpacks = getbits(buffer, bitoffset, 11);
        //printf("Starting Operator 1: num %d @ %d\n", totalsubpacks, *bitoffset);
        for( int i = 0; i < totalsubpacks; i++ ) {
            versum += parsepacket(buffer, bitoffset);
        }
        //printf("Operator Complete 1: num %d @ %d\n", totalsubpacks, *bitoffset);
    }
    return(versum);
}

uint32_t parsepacket(uint32_t *buffer, int *bitoffset) {
    uint32_t ver, typeID, versum = 0;
    ver = getbits(buffer, bitoffset, 3);
    versum += ver;
    typeID = getbits(buffer, bitoffset, 3);
    //printf("Parsing packet ver %d, type %d @ %d\n", ver, typeID, *bitoffset);
    switch (typeID) {
        case 4: ; // literal packet
            literalpacket(buffer, bitoffset);
            break;
        default: ; // operator packet
            versum += operatorpacket(buffer, bitoffset);
            break;
    }
    return(versum);
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
    static uint32_t buffer[1000] = { };
    int bufferlen = 0;
    uint32_t temp = 0;
    int bitcount = 0;

    while( fscanf(fp, "%1x", &temp) > 0 ) {
        buffer[bufferlen] = (buffer[bufferlen] << 4) + temp;
        bitcount += 4;
        if( bitcount % 32 == 0 ) { bufferlen++; }
    }
    // if the input isn't divisible by 32 bits, pad the last buffer value with
    // trailing 0s so the input is continguous in memory
    if( bitcount % 32 != 0 ) {
        int rembits = 32 - (bitcount % 32);
        buffer[bufferlen] = buffer[bufferlen] << rembits;
    }
    bufferlen++;

    // close file
    fclose(fp);

    // uint32_t buffprt = buffer[0];
    int bitoffset = 0;
    int versum = 0;

    versum = parsepacket(buffer, &bitoffset);

    ans = versum;
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
}
