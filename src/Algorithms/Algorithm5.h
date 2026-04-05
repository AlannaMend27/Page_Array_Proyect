#ifndef PAGE_ARRAY_PROYECT_ALGORITHM5_H
#define PAGE_ARRAY_PROYECT_ALGORITHM5_H

// Definicion de la clase PagedArray
#include "../sorter/PagedArray.h"

// metodos del algoritmo timsort
void timSort(PagedArray& arr, int n);
void timMerge(PagedArray& arr, int izquierda, int medio, int derecha);
void timInsertion(PagedArray& arr, int izquierda, int derecha);

#endif //PAGE_ARRAY_PROYECT_ALGORITHM5_H