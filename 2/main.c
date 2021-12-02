#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int partone () {
    int ans = -1;
    char *filename;
    filename = "input.txt";

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return (-1);
    }

    // read file
    int BUFFER_SIZE = 64;
    char buffer[BUFFER_SIZE];
    char *direction;
    int quantity;
    int pos = 0;
    int depth = 0;

    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        //printf("Buffer: %s", buffer);
        direction = strtok(buffer, " "); 
        quantity = atoi(strtok(NULL, "\n"));
        
        //printf("Token: %s\n", token);
        if (strcmp(direction, "forward") == 0) {
            pos = pos + quantity;
        } else if (strcmp(direction, "up") == 0) {
            depth = depth - quantity;
        } else if (strcmp(direction, "down") == 0) {
            depth = depth + quantity;
        } else {
            fprintf(stderr, "Unrecognized direction token: %s", direction);
            return (-1);
        }

    }

    // close file
    fclose(fp);

    ans = depth * pos;
    return ans;
}

int parttwo () {
    int ans = -1;
    char *filename;
    filename = "test.txt";

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file
    int BUFFER_SIZE = 64;
    char buffer[BUFFER_SIZE];
    char *direction;
    int quantity;
    int pos = 0;
    int depth = 0;
    int aim = 0;

    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        //printf("Buffer: %s", buffer);
        direction = strtok(buffer, " "); 
        quantity = atoi(strtok(NULL, "\n"));
        
        //printf("Token: %s\n", token);
        if (strcmp(direction, "forward") == 0) {
            pos += quantity;
            depth += aim * quantity;
        } else if (strcmp(direction, "up") == 0) {
            aim -= quantity;
        } else if (strcmp(direction, "down") == 0) {
            aim += quantity;
        } else {
            fprintf(stderr, "Unrecognized direction token: %s", direction);
            return (-1);
        }

    }

    // close file
    fclose(fp);

    ans = depth * pos;
    return ans;
}

int main () {
    int p1 = partone();
    printf("Part 1: %d\n", p1);
    int p2 = parttwo();
    printf("Part 2: %d\n", p2);
}
