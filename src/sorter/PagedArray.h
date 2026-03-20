#ifndef PAGED_ARRAY_H
#define PAGED_ARRAY_H

#include <cstdio>

class PagedArray {
private:
    int** pages;      // slots de páginas en RAM
    int*  pageIds;    // qué página del archivo tiene cada slot
    bool* dirty;      // si la página fue modificada y hay que escribirla al disco
    int*  lruOrder;   // para el algoritmo LRU
    int   lruClock;   // contador global de accesos
    int   pageSize;
    int   pageCount;
    int   totalElements;

    FILE* file;

    int   pageFaults;
    int   pageHits;

public:
    PagedArray(const char* filePath, int pageSize, int pageCount, int totalElements);
    ~PagedArray();

    int& operator[](int index);

    int getPageFaults()    const { return pageFaults; }
    int getPageHits()      const { return pageHits; }
    int getTotalElements() const { return totalElements; }
};

#endif