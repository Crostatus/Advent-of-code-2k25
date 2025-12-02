#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_LINE_LEN 4096

int count_digits(long long n) {
    if (n == 0)
        return 1;

    if (n < 0)
        n = -n;

    return (int)floor(log10((long double)n)) + 1;
}

long long min_number_with_digits(int digits) {
    if (digits <= 0)
        return 0;
    return (long long)pow(10, digits - 1);
}

long long max_number_with_digits(int digits) {
    if (digits <= 0)
        return 0;
    return (long long)pow(10, digits) - 1;
}

bool is_repetition(long long n) {
    char s[32];
    sprintf(s, "%lld", n);

    int L = strlen(s);

    // try all possible base lengths
    for (int base_len = 1; base_len <= L / 2; base_len++) {
        // base length must divide total length
        if (L % base_len != 0)
            continue;

        int repeats = L / base_len;
        if (repeats < 2)
            continue; // just for clarity, though L/base_len>=2 by base_len<=L/2

        bool ok = true;

        // cmp each block with the first block
        for (int r = 1; r < repeats; r++) {
            if (strncmp(s, s + r * base_len, base_len) != 0) {
                ok = false;
                break;
            }
        }

        if (ok)
            return true;
    }

    return false;
}

long long find_and_sum(long long start, long long end) {
    if (start > end)
        return 0;

    long long sum = 0;

    for (long long curr = start; curr <= end; curr++) {
        if (is_repetition(curr)) {
            // uncomment if you want to see each invalid ID:
            //printf("Found invalid ID: %lld\n", curr);
            sum += curr;
        }
    }

    return sum;
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
    long long password = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        // remove final newline
        buffer[strcspn(buffer, "\n")] = '\0';

        char *token = strtok(buffer, ",");
        while (token != NULL) {

            char *dash = strchr(token, '-');
            if (!dash) {
                fprintf(stderr, "Invalid range token: %s\n", token);
                fclose(fp);
                return 1;
            }

            *dash = '\0';
            long long start = atoll(token);
            long long end   = atoll(dash + 1);

            password += find_and_sum(start, end);

            token = strtok(NULL, ",");
        }
    }

    printf("Password: %lld\n", password);

    fclose(fp);
    return 0;
}
