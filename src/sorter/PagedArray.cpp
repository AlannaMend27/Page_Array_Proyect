#include "PagedArray.h"
#include <stdexcept>
#include <iostream>

// constructor
PagedArray::PagedArray(const char* filePath, int pageSize, int pageCount, int totalElements) {
    this->pageSize      = pageSize;
    this->pageCount     = pageCount;
    this->totalElements = totalElements;
    this->pageFaults    = 0;
    this->pageHits      = 0;
    this->lruClock      = 0;

    // verificar que el archivo se pueda abrir
    file = fopen(filePath, "r+b");
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo de trabajo.");
    }

    // reservar espacio en la memoria RAM
    pages    = new int*[pageCount];
    pageIds  = new int[pageCount];
    dirty    = new bool[pageCount];
    lruOrder = new int[pageCount];

    // crear matriz para guadar las paginas
    for (int i = 0; i < pageCount; i++) {
        pages[i]    = new int[pageSize];
        pageIds[i]  = -1;    // -1 dice slot vacío
        dirty[i]    = false;
        lruOrder[i] = 0;
    }
    
}

// destructor
PagedArray::~PagedArray() {

    delete[] pages;
    delete[] pageIds;
    delete[] dirty;
    delete[] lruOrder;

    if (file) fclose(file);
}
