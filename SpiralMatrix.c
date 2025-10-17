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
    
    displayMatrix(size, matrix);
}