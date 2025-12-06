#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LEN 1024
#define INITIAL_CAPACITY 100

typedef struct {
  long long start;
  long long end;
} Range;

int compare_ranges(const void *a, const void *b) {
  Range *r1 = (Range *)a;
  Range *r2 = (Range *)b;
  if (r1->start < r2->start) {
    return -1;
  }
  if (r1->start > r2->start) {
    return 1;
  }
  return 0;
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
  Range *ranges = malloc(INITIAL_CAPACITY * sizeof(Range));
  int capacity = INITIAL_CAPACITY;
  int count = 0;

  long long start, end;

  // Read ranges
  while (fgets(buffer, MAX_LINE_LEN, fp) != NULL) {
    // Stop if we hit an empty line or a line that doesn't look like a range
    if (buffer[0] == '\n' || buffer[0] == '\r') {
      break;
    }

    if (sscanf(buffer, "%lld-%lld", &start, &end) != 2) {
      break;
    }

    if (count >= capacity) {
      capacity *= 2;
      ranges = realloc(ranges, capacity * sizeof(Range));
      if (!ranges) {
        perror("Memory allocation failed");
        fclose(fp);
        return 1;
      }
    }
    ranges[count].start = start;
    ranges[count].end = end;
    count++;
  }

  fclose(fp);

  if (count == 0) {
    printf("No ranges found.\n");
    free(ranges);
    return 0;
  }

  // Sort ranges
  qsort(ranges, count, sizeof(Range), compare_ranges);

  // Merge ranges and calculate total length
  long long total_fresh = 0;
  long long current_start = ranges[0].start;
  long long current_end = ranges[0].end;

  for (int i = 1; i < count; i++) {
    // Check for overlap or contiguous ranges
    // Overlap: ranges[i].start <= current_end
    // Contiguous: ranges[i].start == current_end + 1
    if (ranges[i].start <= current_end + 1) {
      if (ranges[i].end > current_end) {
        current_end = ranges[i].end;
      }
    } else {
      // No overlap, add the current range length
      total_fresh += (current_end - current_start + 1);
      current_start = ranges[i].start;
      current_end = ranges[i].end;
    }
  }

  // Add the last range
  total_fresh += (current_end - current_start + 1);

  printf("Total fresh ingredient IDs: %lld\n", total_fresh);

  free(ranges);
  return 0;
}
