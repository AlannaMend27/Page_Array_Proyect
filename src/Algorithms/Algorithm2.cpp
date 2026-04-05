#include "Algorithm2.h"
using namespace std;

// Algoritmo #2 MERGE SORT

// combina dos subarreglos arr[].
// el primer arreglo es arr[left..mid]
// el segundo arreglo arr[mid+1..right]
void merge(PagedArray& arr, int left, int mid, int right){

    int n1 = mid - left + 1;
    int n2 = right - mid;

    // crear arreglos temporales en el heap
    int* L = new int[n1];
    int* R = new int[n2];

    // copiar los datos a los arreglos temporales L y R
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    // Fusionar los arreglos temporales
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos que quedan en el arregle L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos que quedan en el arregle R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // liberar memoria de arreglos auxiliares
    delete[] L;
    delete[] R;
}

void mergeSort(PagedArray& arr, int left, int right){

    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);

}