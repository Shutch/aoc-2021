#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

int partone () {
    int ans = -1;
    FILE *fp;
    char *filename = "input.txt";
    char l[64];
    char *ptr;
    int i = 0, j = 0, sum = 0;
    int nums[100000], power, filelength, max;
    int gamma = 0, epsilon = 0;

    // open input file
    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file
    while ((fgets(l, 64, fp)) != NULL) {
        nums[i] = strtol(l, &ptr, 2);
        i++;
    }

    power = strlen(l) - 1;  // ignoring newline
    max = pow(2, power) - 1;
    filelength = i;
    
    for (i = 0; i < power; i++) {
        sum = 0;
        for (j = 0; j < filelength; j++) {
            if (nums[j] & (1 << i)) {
                sum++;
            }
        }
        if (sum >= filelength / 2) {
            gamma = gamma ^ (1 << i);
        }
    }
    
    

    // close file
    fclose(fp);

    // get number of binary digits
    epsilon = max - gamma;
    ans = epsilon * gamma;

    return ans;
}

struct node {
   int value;
   bool o2;
   bool co2;
   struct node *next;
};

int parttwo () {
    int ans = -1;
    char *filename;
    filename = "input.txt";
    char l[64];
    char *ptr;
    struct node *head = NULL;
    struct node *current = NULL;
    int numlength, sum, listlength, o2value, co2value, rem;

    // open input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening %s", filename);
        return -1;
    }

    // read file, build linked list
    while ((fgets(l, 64, fp)) != NULL) {
        struct node *link = (struct node*) malloc(sizeof(struct node));
        link->value = strtol(l, &ptr, 2);
        link->next = head;
        link->o2 = true;
        link->co2 = true;
        head = link;
    }
    numlength = strlen(l)-1;

    // starting with most significant bit for o2
    for (int i = numlength-1; i > -1; i--) {
        // finding most common bit (1 or 0)
        sum = 0;
        listlength = 0;
        for (current = head; current != NULL; current = current->next) {
            if (current->o2) {
                if (current->value & (1 << i)) {
                    sum++;
                }
                listlength++;
            }
        }

        // setting flags in linked list for o2
        rem = listlength - sum;
        for (current = head; current != NULL; current = current->next) {
            if (sum >= rem) {
                if (((current->value & (1 << i)) == 0) && current->o2) {
                    current->o2 = false;     
                    listlength--;
                }
            } else {
                if (((current->value & (1 << i))) && current->o2) {
                    current->o2 = false;     
                    listlength--;
                }
                
            }
        }
        // checking for end condition (only 1 o2 flag set)
        // find the node that has the last true value
        if (listlength == 1) {
            for (current = head; current != NULL; current = current->next) {
                if (current->o2) {
                    o2value = current->value;
                }
            }
        }
    }

    // starting with most significant bit for co2
    for (int i = numlength-1; i > -1; i--) {
        // finding most common bit (1 or 0)
        sum = 0;
        listlength = 0;
        for (current = head; current != NULL; current = current->next) {
            if (current->co2) {
                if (current->value & (1 << i)) {
                    sum++;
                }
                listlength++;
            }
        }

        // setting flags in linked list for co2
        rem = listlength - sum;
        for (current = head; current != NULL; current = current->next) {
            if (sum >= rem) {
                if (((current->value & (1 << i))) && current->co2) {
                    current->co2 = false;     
                    listlength--;
                }
            } else {
                if (((current->value & (1 << i)) == 0) && current->co2) {
                    current->co2 = false;     
                    listlength--;
                }
                
            }
        }
        // checking for end condition (only 1 co2 flag set)
        // find the node that has the last true value
        if (listlength == 1) {
            for (current = head; current != NULL; current = current->next) {
                if (current->co2) {
                    co2value = current->value;
                }
            }
        }
    }

    // close file
    fclose(fp);

    // Free list
    ans = o2value * co2value;
    return ans;
}

int main () {
    int p1 = partone();
    printf("Part 1: %d\n", p1);
    int p2 = parttwo();
    printf("Part 2: %d\n", p2);
}
