#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define True 1
#define MIN_MATRIX_SIZE 2
#define MAX_MATRIX_SIZE 10
#define MIN_INTENSITY 0
#define MAX_INTENSITY 255

void getMatrixSize (unsigned int* matrixSize) {
    printf ("Enter Matrix Size (2-10): ");

    while (True)
    {
        if (scanf ("%u", matrixSize) != 1)
        {
            printf ("Invalid integer. Enter Again: ");
            while (getchar() != '\n');
            continue;
        }

        if (*matrixSize < MIN_MATRIX_SIZE || *matrixSize > MAX_MATRIX_SIZE)
        {
            printf ("Invalid choice. Choose size within the range 2-10: ");
            continue;
        }

        break;
    }
}

void generateMatrix(const unsigned int size, int matrix[size][size]){
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            matrix[row][col] = (rand() % (MAX_INTENSITY - MIN_INTENSITY + 1)) + MIN_INTENSITY;
        }
    }
}

void displayMatrix(const unsigned int size, int matrix[size][size]){
    int *firstElementPtr = &matrix[0][0];

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            printf("%d ", *(firstElementPtr + row * size + col));
        }
        printf("\n");
    }
}


int main () {
    unsigned int matrixSize;
    getMatrixSize (&matrixSize);

    int matrix[matrixSize][matrixSize];
    srand(time(NULL));
    generateMatrix(matrixSize, matrix);

    printf("\nOriginal Randomly Generated Matrix: \n");
    displayMatrix(matrixSize, matrix);

}