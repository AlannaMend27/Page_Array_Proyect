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
    bool*  clockBit;
    int   clockIndex;
    size_t   pageSize;
    int   pageCount;
    size_t   totalElements;
    long long   pageFaults;
    long long  pageHits;
    int* FindSlot;
    int totalPages;
    int* getSpace;
    int freeTop;

public:
    PagedArray(std::filesystem::path filePath, int pageSize, int pageCount, int totalElements);
    ~PagedArray();
    void LoadPage(int freePageIndex, size_t PageToLoad);
    void SavePage(size_t PageIndex, bool PlaceFree = true);
    int ReplaceClock();
    int FindSpace(size_t numPage);
    int GetSpace();
    int& DataRef(int SlotPage, size_t PageIndex);
    void flush();
    int& operator[](int index);

    int getPageFaults()    const { return pageFaults; }
    long long getPageHits()      const { return pageHits; }
    long long getTotalElements() const { return totalElements; }
    size_t getPageSize() const { return pageSize; }
    size_t getPageCount() const { return pageCount; }
};

#endif