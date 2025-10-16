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

void rotateMatrix90Clockwise(unsigned int size, int (*matrix)[size]){
    for (unsigned int layer = 0; layer < size / 2; layer++) 
    {
        unsigned int first = layer;
        unsigned int last = size - 1 - layer;

        for (unsigned int index = first; index < last; index++)
        {
            unsigned int offset = index - first;

            //Getting current group element addresses via Pointer Aritmetic
            int *top = (*(matrix + first)) + index;
            int *left   = (*(matrix + (last - offset))) + first;
            int *bottom = (*(matrix + last)) + (last - offset);
            int *right  = (*(matrix + index)) + last;

            //Rotating four pointers' clockwise
            int temp = *top;
            *top = *left;
            *left = *bottom;
            *bottom = *right;
            *right = temp;
        }
    }
}

void applySmoothingFilter(unsigned int size, int matrix[size][size]){
    for (unsigned int row = 0; row < size; row++)
    {
        for (unsigned int col = 0; col < size; col++)
        {
            int sum = 0, count = 0;

            for (int deltaRow = -1; deltaRow <= 1; deltaRow++)
            {
                for (int deltaCol = -1; deltaCol <= 1; deltaCol++)
                {
                    int neighborRow = (int)row + deltaRow;
                    int neighborCol = (int)col + deltaCol;

                    if (neighborRow >= 0 && neighborRow < (int)size && neighborCol >= 0 && neighborCol < (int)size)
                    {
                        int *neighborPtr = (*(matrix + neighborRow)) + neighborCol;
                        int originalVal = (*neighborPtr) & 0xFF;
                        sum += originalVal;
                        count++;
                    }
                }
            }

            int smoothenedVal = sum / count;
            int *cellPtr = (*(matrix + row)) + col;

            //Storing Smoothened Value in uppper 8 bits and keeping old value intact in lower 8 bits
            *cellPtr = ((*cellPtr) & 0xFF) | ((smoothenedVal & 0xFF) << 8);
        }
    }

    //Extracting the new values
    for (unsigned int row = 0; row < size; row++)
    {
        for (unsigned int col = 0; col < size; col++)
        {
            int *cellPtr = (*(matrix + row)) + col;
            int packedVal = *cellPtr;
            int decodedVal = (packedVal >> 8) & 0xFF;
            *cellPtr = decodedVal;
        }
    }
}


void displayMatrix(const unsigned int size, int matrix[size][size]){
    int *firstElementPtr = &matrix[0][0];

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            printf("%3d ", *(firstElementPtr + row * size + col));
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

    printf("\nMatrix after 90 Degree Clockwise Rotation: \n");
    rotateMatrix90Clockwise(matrixSize, matrix);
    displayMatrix(matrixSize, matrix);

    printf("\nMatrix after Applying 3*3 Smoothing Filter: \n");
    applySmoothingFilter(matrixSize, matrix);
    displayMatrix(matrixSize, matrix);
}