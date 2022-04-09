#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "CycleTimer.h"

//int **grid;
int boxLen; //length of inner grid
int LEN;    //length of whole grid

int** initInput(char *fname) {
    FILE* input = fopen(fname, "r");
    
    fscanf(input, "%d\n", &boxLen);
    LEN = boxLen * boxLen;

    int **grid = (int **)calloc(LEN, sizeof(int *));
    for (int i = 0; i < LEN; i++)
        grid[i] = (int *)calloc(LEN, sizeof(int));

    int entry;
    for (int i = 0; i < LEN; i++) {
        for (int j = 0; j < LEN; j++) {
            fscanf(input, "%d", &entry);
            grid[i][j] = entry;
        }
    }

    fclose(input);
    return grid;
}

void printGrid(int **grid) {
    for (int i = 0; i < LEN; i++) {
        for (int j = 0; j < LEN; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("%s", "\n");
    }
}

// check if placing val in grid position is a legal move
bool isLegal(int **g, int r, int c, int val) {
    // check row 
    for (int j = 0; j < LEN; j++)
        if (g[r][j] == val)
            return false;
    // check col
    for (int i = 0; i < LEN; i++)
        if (g[i][c] == val)
            return false;
    // check box
    int startR = (r / boxLen) * boxLen;
    int startC = (c / boxLen) * boxLen;
    for (int i = startR; i < startR + boxLen; i++) {
        for (int j = startC; j < startC + boxLen; j++)
            if (g[i][j] == val)
                return false;
    }

    return true;
}

bool solve(int **g, int r, int c) {
    
    if (r == LEN - 1 && c == LEN)
        return true;

    if (c == LEN) {
        r++;
        c = 0;
    }

    if (g[r][c] > 0)
        return solve(g, r, c + 1);

    for (int n = 1; n <= LEN; n++) {
        if (isLegal(g, r, c, n)) {
            g[r][c] = n;
            if (solve(g, r, c + 1))
                return true;
        }

        g[r][c] = 0;
    }

    return false;
}

int main(int argc, char *argv[]) {


    if(argv[1] == NULL) {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }

    double startTime;
    double endTime;

    int **grid = initInput(argv[1]);
    
    printGrid(grid);
    printf("\n");

    startTime = CycleTimer::currentSeconds();
    bool hasSolution = solve(grid, 0, 0);
    endTime = CycleTimer::currentSeconds();

    if (hasSolution)
        printGrid(grid);
    else
        printf("%s\n", "no solution");
    
    printf("Computation Time: %.6f ms\n", endTime - startTime);
}