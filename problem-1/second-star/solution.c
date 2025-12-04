#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LEN 1024

int spin_dial(int* position, char direction, int spin, int dial_length) {
    int pos = *position;
    int crosses;

    if (direction == 'R') {
        // Forward movement
        crosses = (pos + spin) / dial_length;

        pos = (pos + spin) % dial_length;
    } 
    else if (direction == 'L') {        
        // Convert backward movement to forward in "reflected space"
        int pos_f = (dial_length - pos) % dial_length;

        crosses = (pos_f + spin) / dial_length;
        
        // Update the real position going backward
        pos = (pos - spin) % dial_length;
        if (pos < 0) {
            pos += dial_length;
        }
    } 


    *position = pos;
    return crosses;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Use: %s <file_name>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    char buffer[MAX_LINE_LEN];
    char direction;
    int spin;

    int position = 50;
    int dial_length = 100;

    int times_hit_zero = 0;
    while (fgets(buffer, MAX_LINE_LEN, fp) != NULL) {
        if (sscanf(buffer, " %c%d", &direction, &spin) != 2) {
            fprintf(stderr, "Invalid row: %s", buffer);
            fclose(fp);
            return 0;
        }        
        
        times_hit_zero+= spin_dial(&position, direction, spin, dial_length);
    }
    printf("Password: %d\n", times_hit_zero);

    fclose(fp);
    return 0;
}


