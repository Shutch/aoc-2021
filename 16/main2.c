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

uint64_t literalpacket(uint32_t *buffer, int *bitoffset);
int arraypacket(uint32_t *buffer, int *bitoffset, uint64_t *vals);
uint64_t parsepacket(uint32_t *buffer, int *bitoffset);

uint64_t literalpacket(uint32_t *buffer, int *bitoffset) {
    uint32_t cont = 1;
    uint64_t val = 0;
    //printf("Starting Literal @ %d\n", *bitoffset);
    do {
        cont = getbits(buffer, bitoffset, 1);
        val = (val << 4) + getbits(buffer, bitoffset, 4);
    } while (cont == 1);
    //printf("Literal Complete: val %ld @ %d\n", val, *bitoffset);
    return(val);
}

int arraypacket(uint32_t *buffer, int *bitoffset, uint64_t *vals) {
    int numvals = 0;
    uint32_t lenID = getbits(buffer, bitoffset, 1);
    if( lenID == 0 ) {
        uint32_t subpacklen = getbits(buffer, bitoffset, 15);
        //printf("Starting Array 0: len %d @ %d\n", subpacklen, *bitoffset);
        int startoffset = *bitoffset;
        while( (*bitoffset - startoffset) < subpacklen ) {
            vals[numvals] = parsepacket(buffer, bitoffset);
            numvals++;
        }
        //printf("Array Complete 0: len %d @ %d\n", subpacklen, *bitoffset);
    } else {
        uint32_t totalsubpacks = getbits(buffer, bitoffset, 11);
        //printf("Starting Array 1: num %d @ %d\n", totalsubpacks, *bitoffset);
        for( int i = 0; i < totalsubpacks; i++ ) {
            vals[numvals] = parsepacket(buffer, bitoffset);
            numvals++;
        }
        //printf("Array Complete 1: num %d @ %d\n", totalsubpacks, *bitoffset);
    }
    return(numvals);
}

uint64_t parsepacket(uint32_t *buffer, int *bitoffset) {
    uint32_t ver, typeID, lenID;
    uint64_t val = 0, vals[1000];
    int numvals;
    ver = getbits(buffer, bitoffset, 3);
    typeID = getbits(buffer, bitoffset, 3);
    //printf("Parsing packet ver %d, type %d @ %d\n", ver, typeID, *bitoffset);
    switch (typeID) {
        case 0: ; // sum packet
            //printf("Starting Sum: @ %d\n", *bitoffset);
            numvals = arraypacket(buffer, bitoffset, vals);
            val = 0;
            for( int i = 0; i < numvals; i++ ) {
                val += vals[i];
            }
            //printf("Sum Complete: val %ld @ %d\n", val, *bitoffset);
            break;
        case 1: ; // product packet
            //printf("Starting Product: @ %d\n", *bitoffset);
            numvals = arraypacket(buffer, bitoffset, vals);
            val = 1;
            for( int i = 0; i < numvals; i++ ) {
                val *= vals[i];
            }
            //printf("Product Complete: val %ld @ %d\n", val, *bitoffset);
            break;
        case 2: ; // minimum packet
            //printf("Starting Min: @ %d\n", *bitoffset);
            numvals = arraypacket(buffer, bitoffset, vals);
            val = UINT32_MAX;
            for( int i = 0; i < numvals; i++ ) {
                if( vals[i] < val ) { val = vals[i]; }
            }
            //printf("Min Complete: val %ld @ %d\n", val, *bitoffset);
            break;
        case 3: ; // maximum packet
            //printf("Starting Max: @ %d\n", *bitoffset);
            numvals = arraypacket(buffer, bitoffset, vals);
            val = 0;
            for( int i = 0; i < numvals; i++ ) {
                if( vals[i] > val ) { val = vals[i]; }
            }
            //printf("Max Complete: val %ld @ %d\n", val, *bitoffset);
            break;
        case 4: ; // literal packet
            val = literalpacket(buffer, bitoffset);
            break;
        case 5: ; // greater than packet
            lenID = getbits(buffer, bitoffset, 1);
            *bitoffset += (lenID == 0) ? 15 : 11;
            // exactly 2 packets
            //printf("Starting GT: @ %d\n", *bitoffset);
            val = (parsepacket(buffer, bitoffset) > parsepacket(buffer, bitoffset));
            //printf("GT Complete: val %ld @ %d\n", val, *bitoffset);
            break;
        case 6: ; // less than packet
            lenID = getbits(buffer, bitoffset, 1);
            *bitoffset += (lenID == 0) ? 15 : 11;
            // exactly 2 packets
            //printf("Starting LT: @ %d\n", *bitoffset);
            val = (parsepacket(buffer, bitoffset) < parsepacket(buffer, bitoffset));
            //printf("LT Complete: val %ld @ %d\n", val, *bitoffset);
            break;
        case 7: ; // equal to packet
            lenID = getbits(buffer, bitoffset, 1);
            *bitoffset += (lenID == 0) ? 15 : 11;
            // exactly 2 packets
            //printf("Starting EQ: @ %d\n", *bitoffset);
            val = (parsepacket(buffer, bitoffset) == parsepacket(buffer, bitoffset));
            //printf("EQ Complete: val %ld @ %d\n", val, *bitoffset);
            break;
    }
    return(val);
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
    uint64_t val = 0;

    val = parsepacket(buffer, &bitoffset);

    ans = val;
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

    uint64_t p2 = parttwo(filename);
    clock_t p2time = clock();
    printf("Part 2: (%lf): %ld\n", (double)(p2time - start) / CLOCKS_PER_SEC, p2);
}
