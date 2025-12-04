#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 4096

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
  long long total = 0;

  while (fgets(buffer, sizeof(buffer), fp)) {
    // Remove final newline
    buffer[strcspn(buffer, "\n")] = '\0';

    int len = strlen(buffer);
    if (len == 0) {
      continue;
    }

    // Find the two digits that form the largest two-digit number
    // We need to check all pairs (i, j) where i < j
    int max_joltage = -1;
    int best_i = -1, best_j = -1;

    for (int i = 0; i < len; i++) {
      if (!isdigit(buffer[i]))
        continue;

      int digit_i = buffer[i] - '0';

      for (int j = i + 1; j < len; j++) {
        if (!isdigit(buffer[j]))
          continue;

        int digit_j = buffer[j] - '0';

        // Form the two-digit number
        int joltage = digit_i * 10 + digit_j;

        if (joltage > max_joltage) {
          max_joltage = joltage;
          best_i = i;
          best_j = j;
        }
      }
    }

    if (max_joltage == -1) {
      printf("No valid pair found for line: %s\n", buffer);
      continue;
    }

    printf("Line: %s -> Batteries at positions %d and %d: %d\n", buffer, best_i,
           best_j, max_joltage);

    total += max_joltage;
  }

  printf("\nTotal joltage: %lld\n", total);

  fclose(fp);
  return 0;
}