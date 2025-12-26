#ifndef IPC_HELPER_H
#define IPC_HELPER_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100
#define MIN_SIZE 1

void getValidInteger(int* value);
void getValidSize(int* size);
int* readArray(int *size);
void displayArray(const int *array, int size);
void swap(int *value1, int *value2);
int partition(int *array, int low, int high);
void quickSort(int *array, int low, int high);

#endif