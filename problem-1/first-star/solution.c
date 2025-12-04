#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LEN 1024

int spin_dial(int* position, char direction, int spin, int dial_length) {

    int factor = (direction == 'R') ? 1 : -1;
    *position = (*position + (spin * factor)) % dial_length;

    return *position == 0 ? 1 : 0;
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


