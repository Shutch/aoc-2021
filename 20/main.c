#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool i[500][500];
    int x0;
    int y0;
    int nxb; // -x bound
    int pxb; // +x bound
    int nyb; // -y bound
    int pyb; // +y bound
    bool bc; // boundary condition
} Image;

void parseinput(FILE *fp, Image *img, bool *algo) {
    int linesize = 513;
    char line[linesize];
    fgets(line, linesize, fp);
    for( int i = 0; i < linesize; i++ ) {  // parse algo line
        if( line[i] == '.' ) {
            algo[i] = false;
        } else if( line[i] == '#' ) {
            algo[i] = true;
        } //else { fprintf(stderr, "Unrecognized algo symbol %d\n", line[i]); }
    }

    fgets(line, linesize, fp); // end of algo line
    fgets(line, linesize, fp); // blank line

    int x0 = img->x0;
    int y0 = img->y0;
    int x, y = 0;
    while( (fgets(line, linesize, fp) != NULL) ) {
        x = 0;
        while( line[x] != '\n' ) {
            if( line[x] == '.' ) {
                img->i[y+y0][x+x0] = false;
            } else if( line[x] == '#' ) {
                img->i[y+y0][x+x0] = true;
            } else { fprintf(stderr, "Unrecognized image symbol %d\n", line[x]); }
            x++; 
        }
        y++;
    }
    img->nxb = img->x0;
    img->nyb = img->y0;
    img->pxb = img->nxb + x;
    img->pyb = img->nyb + y;
    img->bc = false;
    //printf("Image loaded from (%d, %d) to (%d, %d)\n", img->nxb, img->nyb, img->pxb, img->pyb);
}

void printimage(Image *img) {
    int nxb = img->nxb - 3;
    int nyb = img->nyb - 3;
    int pxb = img->pxb + 3;
    int pyb = img->pyb + 3;
    char c;
    printf("\n");
    for( int y = nyb; y < pyb; y++ ) {
        for( int x = nxb; x < pxb; x++ ) {
            c = (img->i[y][x]) ? '#' : '.';
            printf("%c", c);
        }
        printf("\n");
    }
    printf("\n");
}

int getalgoindex(Image *img, int x, int y) {
    int val = 0, i = 8;
    for( int yi = -1; yi <= 1; yi++ ) {
        for( int xi = -1; xi <= 1; xi++ ) {
            val += (int)img->i[y + yi][x + xi] << i;
            i--;
        }
    }
    return val;
}

int enhanceimage(Image *currimg, Image *nextimg, bool algo[512]) {
    int nxb = currimg->nxb - 1;
    int nyb = currimg->nyb - 1;
    int pxb = currimg->pxb + 1;
    int pyb = currimg->pyb + 1;
    int index, val;
    int ontotal = 0;

    // switching boundary condition if applicable
    if( (algo[0] == true) && (algo[511] == false) ) {
        nextimg->bc = !currimg->bc;
    } else { nextimg->bc = currimg->bc; }

    for( int y = nyb-2; y < pyb+2; y++ ) {
        for( int x = nxb-2; x < pxb+2; x++ ) {
            if( (y <= nyb-1) || (y >= pyb) || (x <= nxb-1) || (x >= pxb) ) {
                nextimg->i[y][x] = nextimg->bc;
            } else {
                index = getalgoindex(currimg, x, y);
                val = algo[index];
                ontotal += val;
                nextimg->i[y][x] = val;
            }
        }
    }
    nextimg->nxb = nxb;
    nextimg->nyb = nyb;
    nextimg->pxb = pxb;
    nextimg->pyb = pyb;

    return ontotal;
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
    static bool algo[512];
    static Image currimg;
    void *ptr1 = &currimg;
    currimg.x0 = 200;
    currimg.y0 = 200;

    parseinput(fp, ptr1, algo);

    // close file
    fclose(fp);


    //printf("Step 0:");
    //printimage(ptr1);

    // running algo
    static Image nextimg;
    void *ptr2 = &nextimg;
    int numsteps = 2;
    int ontotal;
    for( int i = 1; i <= numsteps; i++ ) {
        ontotal = enhanceimage(ptr1, ptr2, algo);

        // swapping pointers
        void *temp = ptr1;
        ptr1 = ptr2;
        ptr2 = temp;
        

        //printf("Step %d:\n", i);
        //printimage(ptr1);
    }

    ans = ontotal;
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
    static bool algo[512];
    static Image currimg;
    void *ptr1 = &currimg;
    currimg.x0 = 200;
    currimg.y0 = 200;

    parseinput(fp, ptr1, algo);

    // close file
    fclose(fp);


    //printf("Step 0:");
    //printimage(ptr1);

    // running algo
    static Image nextimg;
    void *ptr2 = &nextimg;
    int numsteps = 50;
    int ontotal;
    for( int i = 1; i <= numsteps; i++ ) {
        ontotal = enhanceimage(ptr1, ptr2, algo);

        // swapping pointers
        void *temp = ptr1;
        ptr1 = ptr2;
        ptr2 = temp;
        

        //printf("Step %d:\n", i);
        //printimage(ptr1);
    }

    ans = ontotal;
    return ans;

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
