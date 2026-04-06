#include "PagedArray.h"
#include <stdexcept>
#include <iostream>
#include<filesystem>

// constructor
PagedArray::PagedArray(const std::filesystem::path& filePath, int pageSize, int pageCount, int totalElements) {
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
    this->findSlot = new int [totalPages];

    for (int i = 0; i < totalPages; i++) {
        findSlot[i] = -1;
    }

    // GET SPACE: variables que guardan la cantidad de espacio libre
    this -> freeSlot = new int[pageCount];
    this -> freeTop = pageCount-1;
    for (int i = 0; i < pageCount; i++) {
        freeSlot[i] = i;
    }

    // inicializar variables importantes de la clase
    for (int i = 0; i < pageCount; i++) {
        pageIds[i] = -1;
        dirty[i] = false;
        clockBit[i] = false;
    }
}

int& PagedArray::operator[](const int index){
    // verificar la cantidad de elementos
    if (index<0 || index >= totalElements) {
        throw std::invalid_argument("Indice Invalido");
    }
    // obtener la pagina donde se encuentra el index
    const int numPage = index / pageSize;

    // obtener el indice de pagina donde se encuentra el index
    const int pageIndex = index % pageSize;

    // verificar si la pagina esta cargada
    int slotPage = FindSpace(numPage);
    if (slotPage != -1) {
        // actualizar variale y retornar el valor
        pageHits += 1;
    }
    else {
        pageFaults += 1;
        // verificar que haya una pagina vacía
        slotPage = GetSpace();
        if (slotPage != -1) {
            LoadPage(slotPage, numPage);
        }
        else {
            slotPage = ReplaceClock();
            LoadPage(slotPage, numPage);
        }

    }
    clockBit[slotPage] = true;
    dirty[slotPage] = true;
    return DataRef(slotPage, pageIndex);
}

int& PagedArray::DataRef(const int slotPage, size_t pageIndex) const {
    return totalMemory[(pageSize * slotPage) + pageIndex];
}

void PagedArray::LoadPage(const int freePageIndex, const int pageToLoad) {

    // calcular donde se cuencuentra la pagina
    const int pageSearch = pageToLoad * pageSize;
    const int bytesForPage = pageSearch * static_cast<int>(sizeof(int));

    // movernos de 4 en 4 bytes hasta la pagina que queremos almacenar
    std::fseek(file, bytesForPage, SEEK_SET);

    size_t intsToRead;

    // calcular la cantidad de elementos a leer
    if (pageSearch+pageSize > totalElements) {
        intsToRead = totalElements - pageSearch;
    }
    else {
        intsToRead = pageSize;
    }

    //Leer enteros y colocarlos en archivo
    int* aux = &this->totalMemory[freePageIndex * pageSize];
    std::fread(aux,sizeof(int),intsToRead,file);

    // actualizar el id de las paginas cargadas y si la pagina fue usada
    pageIds[freePageIndex] = pageToLoad;
    dirty[freePageIndex] = false;
    findSlot[pageToLoad] = freePageIndex;

}

void PagedArray::SavePage(const int PageIndex) {

    // calcular donde se cuencuentra la pagina
    const int pageSearch = pageIds[PageIndex] * pageSize;
    const int bytesForPage = pageSearch * static_cast<int>(sizeof(int));


    // movernos de 4 en 4 bytes hasta la pagina que queremos almacenar
    std::fseek(file, bytesForPage, SEEK_SET);

    size_t intsToWrite;

    // calcular la cantidad de elementos a escribir
    if (pageSearch+pageSize > totalElements) {
        intsToWrite = totalElements - pageSearch;
    }
    else {
        intsToWrite = pageSize;
    }

    //Leer enteros y colocarlos en archivo
    int* aux = &this->totalMemory[PageIndex * pageSize];
    std::fwrite(aux,sizeof(int),intsToWrite,file);

    // actualizar el id de las paginas cargadas y si la pagina fue usada marcar como libre
    findSlot[pageIds[PageIndex]] = -1;
    pageIds[PageIndex] = -1;
    dirty[PageIndex] = false;
    clockBit[PageIndex] = false;

}

int PagedArray::ReplaceClock() {
    while (true) {
        if (!clockBit[clockIndex]) {  // si es false
            // se ha encontrado uno que se puede reemplazar
            if (dirty[clockIndex]) { // si es true
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



int PagedArray::FindSpace(const int numPage) const {
    // encontrar el espacio donde esta guardada una pagina en memoria
    return findSlot[numPage];
}

int PagedArray::GetSpace() {
    // obtener un lugar libre para cargar una pagina
    if (freeTop < 0) {
        return -1;
    }
    // actualizar stack
    const int top = freeTop;
    freeTop -= 1;
    return freeSlot[top];
}

void PagedArray::flush() {
    for (int i = 0; i < pageCount; i++) {
        if (dirty[i]) SavePage(i);
    }
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

