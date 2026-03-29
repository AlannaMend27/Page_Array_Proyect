#ifndef PAGED_ARRAY_H
#define PAGED_ARRAY_H
#include <cstdio>
#include <filesystem>

class PagedArray {
private:
    FILE* file;
    int* totalMemory;      // slots de páginas en RAM
    int*  pageIds;    // qué página del archivo tiene cada slot
    bool* dirty;      // si la página fue modificada y hay que escribirla al disco
    int*  lruOrder;   // para el algoritmo LRU
    int   lruClock;   // contador global de accesos
    size_t   pageSize;
    size_t   pageCount;
    size_t   totalElements;

public:
    int   pageFaults;
    int   pageHits;
    PagedArray(std::filesystem::path filePath, int pageSize, int pageCount, int totalElements);
    ~PagedArray();
    void LoadPage(int freePageIndex, size_t PageToLoad);
    void SavePage(size_t PageIndex);
    int ReplaceLRU();
    int FindSpace(size_t numPage);
    int GetSpace();
    int& DataRef(int SlotPage, size_t PageIndex);
    int& operator[](int index);

    int getPageFaults()    const { return pageFaults; }
    int getPageHits()      const { return pageHits; }
    int getTotalElements() const { return totalElements; }
};

#endif