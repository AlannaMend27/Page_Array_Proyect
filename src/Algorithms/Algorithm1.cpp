#include "Algorithm1.h"
using namespace std;

// Algoritmo #1 QUICK SORT

int partition(PagedArray& arr, int low, int high) {

    // escoger pivote
    int pivot = arr[high];
    int i = low - 1;

    // separar el arreglo en dos subarreglos
    // uno con los menores al pivote
    // otro con los mayores al pivite

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Implementacion de quick sort (utiliza recursividad)

void quickSort(PagedArray& arr, int low, int high) {
    if (low < high) {

        // pi es el pivote, que lo retorna la funcion partition
        int pi = partition(arr, low, high);

        // llamadas recursivas para cada subarreglo que se va creando
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
