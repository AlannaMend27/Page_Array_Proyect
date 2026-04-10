#include "Algorithm5.h"
using namespace std;

//Algoritmo #5 RADIX SORT

int getMax(PagedArray& arr, int n)
{
    // obtener el maximo elemento del arreglo (determinar cantidad digitos)
    int mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}


void countSort(PagedArray& arr, int n, int exp)
{

    // array de salida
    int* output = new int[n];
    int i, count[10] = { 0 };

    // Contabiliza la frecuencia de cada dígito (0-9)
    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    // Calcula las posiciones finales en el arreglo auxiliar
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Ubica cada elemento en su posición correspondiente en el arreglo auxiliar
    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    // Copia los datos ordenados del auxiliar de vuelta a disco
    for (i = 0; i < n; i++)
        arr[i] = output[i];
    delete[] output;
}


void radixSort(PagedArray& arr, int n)
{
    int m = getMax(arr, n);

    // Procesa el arreglo para cada potencia de 10
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}