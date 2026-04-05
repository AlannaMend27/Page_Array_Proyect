#include "Algorithm5.h"
using namespace std;

// ALGORITMO #5 TIMSORT

//funcion que retorna el minimo
int minimun(int n1, int n2) {
    if (n1>n2) {
        return n2;
    }
    return n1;
}

// Insertion Sort
void timInsertion(PagedArray& arr, int izquierda, int derecha) {
    for (int i = izquierda + 1; i <= derecha; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= izquierda && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

// Merge sort
void timMerge(PagedArray& arr, int izquierda, int medio, int derecha) {
    int tamIzq = medio - izquierda + 1;
    int tamDer = derecha - medio;

    int* left = new int[tamIzq];
    int* right = new int[tamDer];

    // copiar datos
    for (int i = 0; i < tamIzq; i++)
        left[i] = arr[izquierda + i];

    for (int i = 0; i < tamDer; i++)
        right[i] = arr[medio + 1 + i];

    int i = 0, j = 0, k = izquierda;

    while (i < tamIzq && j < tamDer) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < tamIzq) {
        arr[k++] = left[i++];
    }

    while (j < tamDer) {
        arr[k++] = right[j++];
    }

    delete[] left;
    delete[] right;
}

// Timsort
void timSort(PagedArray& arr, int n) {

    int subSize;

    // tamaño de cada bloque que se ordenará
    if (n > 2048)
        subSize = n / 1024;
    else
        subSize = 100;

    // ordenar bloques pequeños del archivo con insertion sort
    for (int i = 0; i < n; i += subSize) {
        int right = minimun(i + subSize - 1, n - 1);
        timInsertion(arr, i, right);
    }

    // merge progresivo
    for (int size = subSize; size < n; size = 2 * size) {

        for (int left = 0; left < n; left += 2 * size) {

            int mid = left + size - 1;
            int right = minimun(left + 2 * size - 1, n - 1);

            if (mid < right) {
                timMerge(arr, left, mid, right);
            }
        }
    }
}
