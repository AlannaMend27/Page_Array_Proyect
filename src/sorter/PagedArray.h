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
    int   pageSize;
    int   pageCount;
    int   totalElements;
    long long   pageFaults;
    long long  pageHits;
    int* findSlot;
    int totalPages;
    int* freeSlot;
    int freeTop;

public:
    PagedArray(const std::filesystem::path& filePath, int pageSize, int pageCount, int totalElements);
    ~PagedArray();
    void LoadPage(int freePageIndex, int PageToLoad) ;
    void SavePage(int PageIndex) ;
    int ReplaceClock();
    int FindSpace(int numPage) const;
    int GetSpace();
    int& DataRef(int slotPage, size_t pageIndex) const;
    void flush();
    int& operator[](int index);

    [[nodiscard]] long long getPageFaults()    const { return pageFaults; }
    [[nodiscard]] long long getPageHits()      const { return pageHits; }
    [[nodiscard]] int getTotalElements() const { return totalElements; }
    [[nodiscard]] int getPageSize() const { return pageSize; }
    [[nodiscard]] int getPageCount() const { return pageCount; }
};

#endif