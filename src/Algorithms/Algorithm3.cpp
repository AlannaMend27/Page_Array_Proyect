#include "Algorithm3.h"

// Algoritmo #3 SHELL SORT

void shellSort(PagedArray& arr, int n) {

    // iniciar con un gap grande, se reduce con cada interacion
    for (int gap = n / 2; gap > 0; gap /= 2) {

        // realizar insertion sort con el gap establecido
        for (int i = gap; i < n; i++) {

            // establecer el elemento que se debe colocar correctamente
            int temp = arr[i];
            int j = i;

            // intercambiar los elementos que son mas grandes que el temp, para hacer espacio
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }

            //colocarlo en su posicion correcta
            arr[j] = temp;
        }
    }
}