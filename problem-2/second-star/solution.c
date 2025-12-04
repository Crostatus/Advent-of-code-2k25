#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 4096
#define TARGET_DIGITS 12

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
    if (len < TARGET_DIGITS) {
      // Just paranoia check
      continue;
    }

    char result_str[TARGET_DIGITS + 1];
    int current_pos = -1;

    for (int k = 0; k < TARGET_DIGITS; k++) {
      int digits_needed_after = (TARGET_DIGITS - 1) - k;
      int search_limit = len - digits_needed_after;

      int max_digit = -1;
      int max_pos = -1;

      // Search for the largest digit in the valid range
      for (int i = current_pos + 1; i < search_limit; i++) {
        if (!isdigit(buffer[i]))
          continue;

        int digit = buffer[i] - '0';
        if (digit > max_digit) {
          max_digit = digit;
          max_pos = i;
        }
        // Optimization: finding a 9 = bingo
        if (digit == 9) {
          break;
        }
      }

      if (max_pos != -1) {
        result_str[k] = buffer[max_pos];
        current_pos = max_pos;
      } else {
        // Should not happen if input is valid and long enough
        fprintf(stderr, "Error: could not find enough digits in line: %s\n",
                buffer);
        result_str[k] = '0';
      }
    }
    result_str[TARGET_DIGITS] = '\0';

    long long line_val = atoll(result_str);
    total += line_val;
  }

  printf("\nTotal joltage: %lld\n", total);

  fclose(fp);
  return 0;
}