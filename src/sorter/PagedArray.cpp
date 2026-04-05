#include "PagedArray.h"
#include <stdexcept>
#include <iostream>
#include<filesystem>

// constructor
PagedArray::PagedArray(std::filesystem::path filePath, int pageSize, int pageCount, int totalElements) {
    this->pageSize = pageSize;
    this->pageCount = pageCount;
    this->totalElements = totalElements;
    this->pageFaults = 0;
    this->pageHits = 0;
    this->clockIndex = 0;


    // verificar que el archivo se pueda abrir
    this->file = std::fopen(filePath.string().c_str(), "r+b");
    if (!this->file) {
        throw std::runtime_error("No se pudo abrir el archivo de trabajo.");
    }

    // reservar espacio en la memoria RAM
    totalMemory    = new int[pageSize * pageCount];
    pageIds  = new int[pageCount];
    dirty    = new bool[pageCount];
    clockBit = new bool[pageCount];

    // FIND SPACE: variables que guardan cual es el slot de memoria de cada pagina del archivo
    totalPages = (totalElements + pageSize-1) / pageSize;
    this->FindSlot = new int [totalPages];

    for (int i = 0; i < totalPages; i++) {
        FindSlot[i] = -1;
    }

    // GET SPACE: variables que guardan la cantidad de espacio libre
    this -> getSpace = new int[pageCount];
    this -> freeTop = pageCount-1;
    for (int i = 0; i < pageCount; i++) {
        getSpace[i] = i;
    }

    // inicializar variables importantes de la clase
    for (int i = 0; i < pageCount; i++) {
        pageIds[i] = -1;
        dirty[i] = false;
        clockBit[i] = false;
    }

    std:: cout<<"Hello desde paged Array" << std::endl;
    
}

int& PagedArray::operator[](int index){
    int SlotPage;

    // verificar la cantidad de elementos
    if (index<0 || index >= totalElements) {
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
        clockBit[SlotPage] = true;
    }
    else {
        pageFaults += 1;
        // verificar que haya una pagina vacía
        SlotPage = GetSpace();
        if (SlotPage != -1) {
            LoadPage(SlotPage, numPage);
            clockBit[SlotPage] = true;
        }
        else {
            SlotPage = ReplaceClock();
            LoadPage(SlotPage, numPage);
            clockBit[SlotPage] = true;
        }

    }
    dirty[SlotPage] = true;
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
    FindSlot[PageToLoad] = freePageIndex;

}

void PagedArray::SavePage(size_t PageIndex, bool PlaceFree) {

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

    // actualizar el id de las paginas cargadas y si la pagina fue usada marcar como libre
    FindSlot[pageIds[PageIndex]] = -1;
    pageIds[PageIndex] = -1;
    dirty[PageIndex] = false;
    clockBit[PageIndex] = false;

}

int PagedArray::ReplaceClock() {
    while (true) {
        if (clockBit[clockIndex] == false) {
            // se ha encontrado uno que se puede reemplazar
            if (dirty[clockIndex] == true) {
                SavePage(clockIndex);
            }
            int selected = clockIndex;
            clockIndex = (clockIndex + 1) % pageCount;
            return selected;
        }
        // damos una segunda oportunidad al bit
        clockBit[clockIndex] = false;
        clockIndex = (clockIndex + 1) % pageCount;

    }
}



int PagedArray::FindSpace(size_t numPage) {
    // encontrar el espacio donde esta guardada una pagina en memoria
    return FindSlot[numPage];
}

int PagedArray::GetSpace() {
    // obtener un lugar libre para cargar una pagina
    if (freeTop < 0) {
        return -1;
    }
    int top = freeTop;
    freeTop -= 1;
    return getSpace[top];
}

// destructor
PagedArray::~PagedArray() {

    // guardar paginas restantes (En caso de que alguna no se guarde antes de terminar)
    for (int i = 0; i < pageCount; i++) {
        if (dirty[i]) SavePage(i);
    }

    delete[] totalMemory;
    delete[] pageIds;
    delete[] dirty;
    delete[] clockBit;

    if (file) fclose(file);

}

