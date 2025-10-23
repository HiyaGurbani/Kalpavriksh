#include <stdio.h>
#include <stdbool.h>

#define MIN_MATRIX_SIZE 1
#define MAX_MATRIX_SIZE 20

void getMatrixSize(unsigned int* size){

    printf("Enter the size of matrix: ");
    
    while (true)
    {
        if (scanf("%u", size) != 1)
        {
            printf ("Error: Invalid Integer. Enter Again: ");
            while (getchar() != '\n');
            continue;
        }

        if (*size < MIN_MATRIX_SIZE || *size > MAX_MATRIX_SIZE)
        {
            printf ("Invalid choice. Choose size within the range 1-20: ");
            continue;
        }

        break;
    }
}

void spiralMatrix(const unsigned int size, int matrix[size][size]){
    int top = 0, bottom = size - 1;
    int left = 0, right = size - 1;
    unsigned int val = 1;

    while (top <= bottom && left <= right )
    {
        for (int index = left; index <= right; index++)
        {
            *(*(matrix + top) + index) = val++;
        } 
        top++;

        for (int index = top; index <= bottom; index++)
        {
            *(*(matrix + index) + right) = val++;
        } 
        right--;

        if (top <= bottom)
        {
            for (int index = right; index >= left; index--)
            {
                *(*(matrix + bottom) + index) = val++;
            } 
            bottom--;
        }

        if (left <= right)
        {
            for (int index = bottom; index >= top; index--)
            {
                *(*(matrix + index) + left) = val++;
            } 
            left++;
        }
    }
}

void displayMatrix(const unsigned int size, int matrix[size][size]){
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            printf("%3d ", (*(*(matrix + row) + col)));
        }
        printf("\n");
    }
}

int main(){
    unsigned int size;
    getMatrixSize(&size);

    int matrix[size][size];
    spiralMatrix(size, matrix);
    displayMatrix(size, matrix);
}