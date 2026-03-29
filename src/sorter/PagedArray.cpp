#include "PagedArray.h"
#include <stdexcept>
#include <iostream>
#include<filesystem>

// constructor
PagedArray::PagedArray(std::filesystem::path filePath, int pageSize, int pageCount, int totalElements) {
    this->pageSize      = pageSize;
    this->pageCount     = pageCount;
    this->totalElements = totalElements;
    this->pageFaults    = 0;
    this->pageHits      = 0;
    this->lruClock      = 0;


    // verificar que el archivo se pueda abrir
    this->file = std::fopen(filePath.string().c_str(), "r+b");
    if (!this->file) {
        throw std::runtime_error("No se pudo abrir el archivo de trabajo.");
    }

    // reservar espacio en la memoria RAM
    totalMemory    = new int[pageSize * pageCount];
    pageIds  = new int[pageCount];
    dirty    = new bool[pageCount];
    lruOrder = new int[pageCount];

    std:: cout<<"Hello desde paged Array" << std::endl;
    
}

int& PagedArray::operator[](int index){
    int SlotPage;

    // verificar la cantidad de elementos
    if (index<0 || index > totalElements) {
        throw std::invalid_argument("Indice Invalido");;
    }
    // obtener la pagina donde se encuentra el index
    size_t numPage = index / pageSize;

    // obtener el indice de pagina donde se encuentra el index
    size_t pageIndex = index % pageSize;

    // verificar si la pagina esta cargada
    SlotPage = FindSpace(numPage);
    if (SlotPage != -1) {
        // actualizar variale y retornar el valor
        pageHits += 1;
        lruOrder[SlotPage] = ++lruClock;
    }
    else {
        pageFaults += 1;
        // verificar que haya una pagina vacía
        SlotPage = GetSpace();
        if (SlotPage != -1) {
            LoadPage(SlotPage, numPage);
            lruOrder[SlotPage] = ++lruClock;
        }
        else {
            SlotPage = ReplaceLRU();
            LoadPage(SlotPage, numPage);
            lruOrder[SlotPage] = ++lruClock;
        }

    }
    return DataRef(SlotPage, pageIndex);
}

int& PagedArray::DataRef(int SlotPage, size_t PageIndex) {
    return totalMemory[(pageSize * SlotPage) + PageIndex];
}

void PagedArray::LoadPage(int freePageIndex, size_t PageToLoad) {

    // calcular donde se cuencuentra la pagina
    size_t PageSearch = PageToLoad * pageSize;
    size_t BytesForPage = PageSearch * sizeof(int);

    // movernos de 4 en 4 bytes hasta la pagina que queremos almacenar
    std::fseek(file, BytesForPage, SEEK_SET);

    size_t intsToRead;

    // calcular la cantidad de elementos a leer
    if (PageSearch+pageSize > totalElements) {
        intsToRead = totalElements - PageSearch;
    }
    else {
        intsToRead = pageSize;
    }

    //Leer enteros y colocarlos en archivo
    int* aux = &this->totalMemory[freePageIndex * pageSize];
    std::fread(aux,sizeof(int),intsToRead,file);

    // actualizar el id de las paginas cargadas y si la pagina fue usada
    pageIds[freePageIndex] = PageToLoad;
    dirty[freePageIndex] = false;

}

void PagedArray::SavePage(size_t PageIndex) {

    // calcular donde se cuencuentra la pagina
    size_t PageSearch = pageIds[PageIndex] * pageSize;
    size_t BytesForPage = PageSearch * sizeof(int);

    // movernos de 4 en 4 bytes hasta la pagina que queremos almacenar
    std::fseek(file, BytesForPage, SEEK_SET);

    size_t intsToWrite;

    // calcular la cantidad de elementos a escribir
    if (PageSearch+pageSize > totalElements) {
        intsToWrite = totalElements - PageSearch;
    }
    else {
        intsToWrite = pageSize;
    }

    //Leer enteros y colocarlos en archivo
    int* aux = &this->totalMemory[PageIndex * pageSize];
    std::fwrite(aux,sizeof(int),intsToWrite,file);

    // actualizar el id de las paginas cargadas y si la pagina fue usada
    pageIds[PageIndex] = -1;
    dirty[PageIndex] = false;

}

int PagedArray::ReplaceLRU() {
    size_t menor = lruOrder[0];
    int index = 0;

    for (size_t i = 0; i< pageCount;i++) {
        if (lruOrder[i]< menor) {
            menor = lruOrder[i];
            index = i;
        }
    }
    if (dirty[index] == true) {
        SavePage(index);
    }
    pageIds[index] = -1;
    dirty[index] = false;
    return index;
}



int PagedArray::FindSpace(size_t numPage) {
    // encontrar el espacio donde esta guardada una pagina
    for (int i = 0 ; i < pageCount; i++){
        if (numPage == pageIds[i]) {
            return i;
        }
    }
    return -1;
}

int PagedArray::GetSpace() {
    // obtener un lugar libre para cargar una pagina
    for (int i = 0 ; i < pageCount; i++) {
        if (-1 == pageIds[i]) {
            return i;
        }
    }
    return -1;
}

// destructor
PagedArray::~PagedArray() {

    delete[] totalMemory;
    delete[] pageIds;
    delete[] dirty;
    delete[] lruOrder;

    if (file) fclose(file);
}

//
