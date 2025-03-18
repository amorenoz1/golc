#include <stddef.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 50

typedef struct {
   int x;
   int y;
} Directions;

int **allocateGrid() {
   int **grid = (int **)malloc(sizeof(int *) * SIZE);
   if (!grid) {
      perror("Failed to allocate grid");
      exit(EXIT_FAILURE);
   }

   for (size_t i = 0; i < SIZE; i++) {
      grid[i] = (int *)malloc(sizeof(int) * SIZE);
      if (!grid[i]) {
         perror("Failed to allocate grid row");
         exit(EXIT_FAILURE);
      }
   }
   return grid;
}

void freeGrid(int **grid) {
   for (size_t i = 0; i < SIZE; i++) {
      free(grid[i]);
   }
   free(grid);
}

int updateCell(size_t i, size_t j, int **grid) {
   Directions dir[] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0},
                       {1, 0},   {-1, 1}, {0, 1},  {1, 1}};

   unsigned int alive_neighbours = 0;
   int cell = grid[i][j];

   for (size_t k = 0; k < 8; k++) {
      int x = i + dir[k].x;
      int y = j + dir[k].y;

      if (x >= 0 && y >= 0 && x < SIZE && y < SIZE && grid[x][y] == 1) {
         alive_neighbours++;
      }
   }

   if (cell == 1) {
      if (alive_neighbours < 2 || alive_neighbours > 3) {
         return 0;
      }
      return 1;
   } else {
      return alive_neighbours == 3 ? 1 : 0;
   }
}

void prepareGrid(int **grid) {
   for (size_t i = 0; i < SIZE; i++) {
      for (size_t j = 0; j < SIZE; j++) {
         grid[i][j] = 0;
      }
   }
}

int **updateState(int **grid) {
   int **newGrid = allocateGrid();
   for (size_t i = 0; i < SIZE; i++) {
      for (size_t j = 0; j < SIZE; j++) {
         newGrid[i][j] = updateCell(i, j, grid);
      }
   }
   return newGrid;
}

void populateGrid(int **grid, int cellc) {
   for (int i = 0; i < cellc; i++){
      int x = rand() % SIZE;
      int y = rand() % SIZE;
      grid[x][y] = 1;
   }
}

void render(int **grid, const Vector2 *dimension) {
   for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
         Vector2 pos = {i * dimension->x, j * dimension->y};
         if (grid[i][j] == 1) {
            DrawRectangleV(pos, *dimension, WHITE);
         }
      }
   }
}

int main(int argc, char **argv) {
   if (argc != 2) {
      printf("Usage: ./main <seed>\n");
      return 1;
   }

   unsigned int seed = atoi(argv[1]);
   srand(seed);

   int **grid = allocateGrid();
   const Vector2 cellDim = {800.0f / (float) SIZE, 800.0f / (float) SIZE};
   prepareGrid(grid);
   populateGrid(grid, 200);

   InitWindow(800, 800, "GameOfLife");
   SetTargetFPS(1);

   while (!WindowShouldClose()) {

      BeginDrawing();
      {
         render(grid, &cellDim);
      }
      EndDrawing();

      int **newGrid = updateState(grid);
      freeGrid(grid);
      grid = newGrid;
      ClearBackground(BLACK);
   }

   CloseWindow();
   freeGrid(grid);
   return 0;
}
