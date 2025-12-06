#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Coordinates {
  int x;
  int y;
};

#define NORTH (struct Coordinates){.x = -1, .y = 0}
#define SOUTH (struct Coordinates){.x = 1, .y = 0}
#define EAST (struct Coordinates){.x = 0, .y = 1}
#define WEST (struct Coordinates){.x = 0, .y = -1}

#define NORTH_WEST (struct Coordinates){.x = -1, .y = -1}
#define NORTH_EAST (struct Coordinates){.x = -1, .y = 1}
#define SOUTH_WEST (struct Coordinates){.x = 1, .y = -1}
#define SOUTH_EAST (struct Coordinates){.x = 1, .y = 1}

#define MAX_NEIGHBORS 3

struct Coordinates add_coordinates(struct Coordinates a, struct Coordinates b) {
  return (struct Coordinates){.x = a.x + b.x, .y = a.y + b.y};
}

bool belongs_to_matrix(struct Coordinates coord, int rows, int cols) {
  return coord.x >= 0 && coord.x < rows && coord.y >= 0 && coord.y < cols;
}

void update_neighbors(int **matrix, struct Coordinates coord, int rows,
                      int cols) {
  struct Coordinates neighbors[] = {
      NORTH, SOUTH, EAST, WEST, NORTH_WEST, NORTH_EAST, SOUTH_WEST, SOUTH_EAST};
  for (int i = 0; i < 8; i++) {
    struct Coordinates neighbor = add_coordinates(coord, neighbors[i]);
    if (belongs_to_matrix(neighbor, rows, cols)) {
      matrix[neighbor.x][neighbor.y]++;
    }
  }
}

int **build_occurrencies_matrix(int rows, int cols) {
  int **matrix = (int **)malloc(rows * sizeof(int *));
  for (int i = 0; i < rows; i++) {
    matrix[i] = (int *)malloc(cols * sizeof(int));
    for (int j = 0; j < cols; j++) {
      matrix[i][j] = 0;
    }
  }
  return matrix;
}

int main() {
  FILE *fp = fopen("input.txt", "r");
  if (!fp) {
    perror("Error opening input.txt");
    return 1;
  }

  int rows = 0;
  int cols = 0;
  int ch;
  int current_line_len = 0;

  while ((ch = fgetc(fp)) != EOF) {
    if (ch == '\n') {
      rows++;
      if (cols == 0) {
        cols = current_line_len;
      } else if (current_line_len != cols && current_line_len > 0) {
        // Handle potential inconsistency or just ignore empty lines at end
        // For strict rectangular, we might want to error, but let's just warn
        // or assume consistent
      }
      current_line_len = 0;
    } else {
      current_line_len++;
    }
  }
  // If file doesn't end with newline, count the last line
  if (current_line_len > 0) {
    rows++;
    if (cols == 0) {
      cols = current_line_len;
    }
  }

  printf("Detected dimensions: %d rows, %d cols\n", rows, cols);

  // Allocate memory
  char **matrix = (char **)malloc(rows * sizeof(char *));

  for (int i = 0; i < rows; i++) {
    matrix[i] =
        (char *)malloc((cols + 1) * sizeof(char)); // +1 for null terminator
  }

  int **occurrencies_matrix = build_occurrencies_matrix(rows, cols);

  rewind(fp);
  int r = 0;
  char *buffer = (char *)malloc((cols + 2) * sizeof(char));

  while (fgets(buffer, cols + 2, fp) && r < rows) {
    // Remove newline
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(matrix[r], buffer);
    r++;
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (matrix[i][j] == '@') {
        update_neighbors(occurrencies_matrix,
                         (struct Coordinates){.x = i, .y = j}, rows, cols);
      }
    }
  }

  int amout_with_fewer_than_max_neighbors = 0;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (matrix[i][j] == '@' && occurrencies_matrix[i][j] <= MAX_NEIGHBORS) {
        amout_with_fewer_than_max_neighbors++;
      }
    }
  }

  free(buffer);
  fclose(fp);

  printf("Amount of octopuses with fewer than %d neighbors: %d\n",
         MAX_NEIGHBORS + 1, amout_with_fewer_than_max_neighbors);

  // Cleanup
  for (int i = 0; i < rows; i++) {
    free(matrix[i]);
  }
  free(matrix);

  return 0;
}