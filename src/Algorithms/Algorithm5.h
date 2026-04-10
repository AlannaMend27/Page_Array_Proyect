#ifndef PAGE_ARRAY_PROYECT_ALGORITHM5_H
#define PAGE_ARRAY_PROYECT_ALGORITHM5_H

// Definicion de la clase PagedArray
#include "../sorter/PagedArray.h"

// metodos del algoritmo radix sort
int getMax(PagedArray& arr, int n);
void countSort(PagedArray& arr, int n, int exp);
void radixSort(PagedArray& arr, int n);

#endif //PAGE_ARRAY_PROYECT_ALGORITHM5_H