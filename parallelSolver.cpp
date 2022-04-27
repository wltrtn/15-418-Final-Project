#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include "CycleTimer.h"

typedef struct matrix {
    short **data;   // filled board values
    short **fixed;  // indicates given values in original input
} MATRIX;

typedef struct listNode {
    MATRIX mat;
    short i, j;
    struct listNode *next;
} item;

int l;  // length of subgrid
int SIZE;   // length of board
MATRIX solution;
item *head; // head of queue
item *tail;

// read input file, return matrix object
MATRIX initInput(char *filename) {

    MATRIX matrix;  
    FILE *inputFile = fopen(filename, "r");

    int subgridLen;
    fscanf(inputFile, "%d", &subgridLen);
    l = subgridLen;
    SIZE = l * l;

    // allocate memory for matrix with SIZE x SIZE dimensions
    matrix.data = (short**) malloc(SIZE * sizeof(short*));  
    for (int i = 0; i < SIZE; i++)
        matrix.data[i] = (short*) malloc(SIZE * sizeof(short));

    matrix.fixed = (short**) malloc(SIZE * sizeof(short*));
    for (int i = 0; i < SIZE; i++)
        matrix.fixed[i] = (short*) malloc(SIZE * sizeof(short));
    
    for (int i = 0; i < SIZE; i++) 
        for (int j = 0; j < SIZE; j++)    
            matrix.fixed[i][j] = 0;
        
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(inputFile, "%d", &subgridLen);
            matrix.data[i][j] = subgridLen;
            if (matrix.data[i][j] != 0)
                matrix.fixed[i][j] = 1;
        }  
    }
    
    fclose(inputFile);
    return matrix;
}

void printMatrix(MATRIX *matrix) {
    printf("\n_________________________\n");
    for (int i = 0; i < SIZE; i++) {
        printf("| ");
        for (int j = 0; j < SIZE; j = j + 3) 
            printf("%1d %1d %1d | ", matrix->data[i][j], matrix->data[i][j + 1], matrix->data[i][j + 2]);
        
        if ((i + 1) % 3 == 0) 
            printf("\n-------------------------\n");
        else 
            printf("\n");
    }
}

void printGrid(MATRIX *matrix) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix->data[i][j]);
        }
        printf("%s", "\n");
    }
}

// checks if the value at (r, c) is a legal move
short permissible(MATRIX matrix, short r, short c) {

    short value = matrix.data[r][c];

    // check same column
    for (short line = 0; line < SIZE; line++) {
        if ((r != line) && (matrix.data[line][c] == value)) 
            return 0;
    }

    // check same line
    for (short column = 0; column < SIZE; column++) {
        if (c != column && matrix.data[r][column] == value)
            return 0;
    }
    
    // check subgrid
    short rGroup = ((r / l) * l);
    short cGroup = ((c / l) * l);
    for (short line = rGroup; line < rGroup + l; line++) {
        for (short column = cGroup; column < cGroup + l; column++) {
            if (r != line && c != column && matrix.data[line][column] == value) 
                    return 0;
            
        }
    }

    return 1;
}

// decreases board position to previous non-fixed value
void decreasePosition(MATRIX *matrix, short *rPointer, short *cPointer) {

    do {
        if (*cPointer == 0 && *rPointer > 0) {
            *cPointer = SIZE - 1;
            (*rPointer)--;
        } else
            (*cPointer)--;

    } while (*cPointer >= 0 && matrix->fixed[*rPointer][*cPointer] == 1);
}

// increases board position to next non-fixed value
void increasePosition(MATRIX *matrix, short *rPointer, short *cPointer) {
  
    do {
        if (*cPointer < SIZE - 1)
            (*cPointer)++;
        else {
            *cPointer = 0;
            (*rPointer)++;
        }

    } while (*rPointer < SIZE && matrix->fixed[*rPointer][*cPointer] == 1);
}

// deallocates given item node
void freeListElement(item *node) {
    for (int i = 0; i < SIZE; i++) {
        free(node->mat.data[i]);
        free(node->mat.fixed[i]);
    }
    free(node->mat.data);
    free(node->mat.fixed);
    free(node);
}

// creates new item node representing current matrix values and position
item* createItem(MATRIX matrix, short i, short j) {

    item* node = (item*)malloc(sizeof(item));
    // allocate memory for new copy
    node->mat.data = (short**)malloc(SIZE * sizeof(short*));
    for (int m = 0; m < SIZE; m++)
        node->mat.data[m] = (short*) malloc(SIZE * sizeof(short));
    
    node->mat.fixed = (short**)malloc(SIZE * sizeof(short*));
    for (int m = 0; m < SIZE; m++)
        node->mat.fixed[m] = (short*)malloc(SIZE * sizeof(short));

    // copy matrix values
    for (short x = 0; x < SIZE; x++) {
        for (short y = 0; y < SIZE; y++) {
            node->mat.data[x][y] = matrix.data[x][y];
            node->mat.fixed[x][y] = matrix.fixed[x][y]; 
        }
    }

    node->i = i;
    node->j = j;
    node->next = NULL;
    
    return node;
}

// insert item to back of the queue
void insertItem(item *newItem) {

    if (head == NULL) {
        head = newItem;
        tail = newItem;
    } else {
        tail->next = newItem;
        tail = newItem;
    }
}

// remove item from head of the queue
item* removeItem() {
    item* res = NULL;
    if (head != NULL) {
        res = head;
        head = res->next;
        if(head == NULL) 
            tail = NULL;
        
    }
    return res;
}

// initialize job pool with SIZE jobs, taken from input board
void initPool(MATRIX *matrix){

    short i = 0;
    short j = 0;

    if (matrix->fixed[i][j] == 1)
        increasePosition(matrix, &i, &j);

    short num = 0;
    item* current = NULL;

    while(1) {
        
        matrix->data[i][j]++;

        // add jobs to initial pool, only if placed values are legal
        if (matrix->data[i][j] <= SIZE && permissible(*matrix, i, j) == 1) {
            item* newPath = createItem(*matrix, i, j);
            insertItem(newPath);
            //printf("matrix %d added to pool i=%d, j=%d\n",num, i, j);
            //printMatrix(matrix);
            //printf("\n");
            num++;

            // if out of values for current position, modify a previously added job
        } else if(matrix->data[i][j] > SIZE) {
            if (current != NULL)
                freeListElement(current);
        
            if (num >= SIZE)
                break;
            
            item* current = removeItem();

            if (current == NULL)
                break;
            
            matrix = &(current->mat);
            i = current->i;
            j = current->j;
            
            // found a solution
            if (i == SIZE - 1 && j == SIZE - 1) {
                insertItem(current);
                break;
            }

            num--;

            increasePosition(matrix, &i, &j);
        }
    }

    if (current != NULL) 
        freeListElement(current);
    
    //printf("Created %d initial boards.\n", num);

}

short processPool(MATRIX matrix) {

    head = NULL;
    tail = NULL;

    // use bfs search and insert jobs into queue 
    initPool(&matrix);

    // begin parallel region
    // ensure only one thread accesses queue at a time

    short found = 0;
    short i, j;
    int level;
    item* current;

    #pragma omp parallel shared(found) private(i,j, current, level)
    {  
        #pragma omp critical (pool)
        current = removeItem();
        
        // while there are still jobs in the pool && solution not found
        while (current != NULL && found == 0) {

            MATRIX currMat = current->mat;
            //printMatrix(&currMat);
            i = current->i;
            j = current->j;
            //printf("%d, %d\n", i, j);

            increasePosition(&currMat, &i, &j);

            level = 1;

            // dfs search for solution
            while (level > 0 && i < SIZE && found == 0) {
                if (currMat.data[i][j] < SIZE) {    
                    // increment position value, and goto next pos if value is legal
                    currMat.data[i][j]++;
                    if (permissible(currMat, i, j) == 1) {
                        increasePosition(&currMat, &i, &j);
                        level++;
                    }
                    
                } else {
                    // all values for current position tried and failed,
                    // backtrack to last non-fixed position
                    currMat.data[i][j] = 0;
                    decreasePosition(&currMat, &i, &j);
                    level--;
                } 

            } 

            // position is past end of the board, with all values legal, so solution found
            if (i == SIZE) {
                found = 1;
                solution = currMat;
                continue;
            }

            free(current);

            #pragma omp critical (pool)
            current = removeItem();
        }

    }   // end parallel region
    return found;
}

int main(int argc, char *argv[]) {

    double startTime;
    double endTime;

    if (argv[1] == NULL) {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    } 

    MATRIX mat = initInput(argv[1]);

    printf("Input board:");
    printMatrix(&mat);

    //printf("%d\n", omp_get_max_threads());
    int threadCount;
    if (argv[2] == NULL)
        threadCount = omp_get_max_threads();
    else
        threadCount = atoi(argv[2]);

    printf("Running with %d threads\n\n", threadCount);
    omp_set_num_threads(threadCount);

    startTime = CycleTimer::currentSeconds();

    // generate job pool for input board, and process them to find a solution
    short hasSolution = processPool(mat);

    endTime = CycleTimer::currentSeconds();

    if (hasSolution == 0) {
        printf("No solution\n");
        return 1;
    }

    printf("Solution (%d threads):\n", threadCount);
    //printMatrix(&solution);
    printGrid(&solution);

    printf("Computation Time: %.6f ms\n", endTime - startTime);
    
    // deallocate queue
    item* node = head;
    while (node != NULL) {
        item* next = node->next;
        freeListElement(node);
        node = next;
    }
    
    return 0;
}