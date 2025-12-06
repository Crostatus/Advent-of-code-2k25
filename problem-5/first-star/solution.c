#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LEN 1024

struct Range {
  long long start;
  long long end;
  struct Range *next;
};

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
  long long range_start;
  long long range_end;
  long long id;

  bool scan_range = true;
  bool scan_ids = false;

  int valid_ids = 0;

  struct Range *ranges = NULL;

  while (fgets(buffer, MAX_LINE_LEN, fp) != NULL) {

    if (buffer[0] == '\n') {
      scan_range = false;
      scan_ids = true;
      continue;
    }

    if (scan_ids && (sscanf(buffer, " %lld", &id) != 1)) {
      fprintf(stderr, "Unable to read ID from row: %s", buffer);
      fclose(fp);
      return 0;
    }

    if (scan_range &&
        (sscanf(buffer, " %lld-%lld", &range_start, &range_end) != 2)) {
      fprintf(stderr, "Unable to read range from row: %s", buffer);
      fclose(fp);
      return 0;
    }

    if (scan_range) {
      struct Range *new_range = malloc(sizeof(struct Range));
      new_range->start = range_start;
      new_range->end = range_end;
      new_range->next = ranges;
      ranges = new_range;
    }

    if (scan_ids) {
      struct Range *current = ranges;
      while (current != NULL) {
        if (id >= current->start && id <= current->end) {
          valid_ids++;
          break;
        }
        current = current->next;
      }
    }
  }

  printf("Valid IDs: %d\n", valid_ids);

  fclose(fp);
  return 0;
}
