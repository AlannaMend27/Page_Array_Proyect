#include "Algorithm4.h"
using namespace std;

// Algoritmo #4 COMBSORT

// encontrar el gap de los elementos para cada iteracion
int getNextGap(int gap)
{
    // Shrink gap by Shrink factor
    gap = (gap*10)/13;

    if (gap < 1)
        return 1;
    return gap;
}

void combSort(PagedArray& arr, int n)
{
    // Inicializar gap
    int gap = n;

    // inicializar bandera de swap en true para que funcione el ciclo
    bool swapped = true;

    // iterar siempre que gap sea diferente de uno
    while (gap != 1 || swapped == true)
    {
        // encontrar gap siguiente
        gap = getNextGap(gap);

        // Establecer swap como falso al inicio
        swapped = false;

        // comparar todos los elementos con el gap actual
        for (int i=0; i<n-gap; i++)
        {
            if (arr[i] > arr[i+gap])
            {
                // swap
                int temp = arr[i];
                arr[i] = arr[i+gap];
                arr[i+gap] = temp;
                // registrar swap
                swapped = true;
            }
        }
    }
}