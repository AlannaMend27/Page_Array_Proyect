#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>
#include "PagedArray.h"
#include "../Algorithms/Algorithm1.h"
#include "../Algorithms/Algorithm2.h"
#include "../Algorithms/Algorithm3.h"
#include "../Algorithms/Algorithm4.h"
#include "../Algorithms/Algorithm5.h"

struct DataVal {
    std::filesystem::path inputPathFile;
    std::filesystem::path outputPathFile;
    std::filesystem::path outputPath;
    std::string algorithm;
    int pageSize = 0;
    int pageCount = 0;
};

std::string getAlgorithm(const std::string& value) {

    std::string Valuelower = value;
    std::transform(Valuelower.begin(), Valuelower.end(), Valuelower.begin(), ::tolower);

    if (Valuelower == "quicksort") {
        return "Quick Sort";
    }
    if (Valuelower == "mergesort") {
        return "Merge Sort";
    }
    if (Valuelower == "shellsort" ) {
        return "Shell Sort";
    }
    if (Valuelower == "combsort") {
        return "Comb Sort";
    }
    if (Valuelower == "timsort") {
        return "Tim Sort";
    }

    throw std::invalid_argument("Algoritmo no valido, los algoritmos validos son: Quicksort, Mergesort, Shellsort, Combsort o Timsort");

}

DataVal RecivedArguments(int argc, char* argv[]) {

    // verificar cantidad d parametros
    if (argc != 11) {
        throw std::invalid_argument("cantidad incorrecta de argumentos");
    }
    // inicializar en la funcion la estructura de variables
    DataVal dataVal;

    // verificar cada uno de los parametros
    for (int i= 1; i<argc; i+=2) {
        std::string parameter1 = argv[i];
        std:: string parameter2 = argv[i+1];

        if (parameter1 == "-input") {
            // verificar que la ruta exista
            if (!std::filesystem::exists(parameter2)){
                throw std::invalid_argument("la direccion ingresada no existe");
            }

            // verifiar que la ruta es una carpeta
            if (!std::filesystem::is_directory(parameter2)) {
                throw std::invalid_argument("la direccion ingresada no es una carpeta");
            }

            // verificar que la ruta ingresada contiene un archivo .bin, caso contrario tira error
            bool findBin = false;
            for (const auto& entry : std::filesystem::directory_iterator(parameter2)) {
                if (entry.is_regular_file() && entry.path().extension() == ".bin") {
                    findBin = true;
                    break;
                }
            }

            if (!findBin) {
                throw std::invalid_argument("La direccion ingresada no contiene un archivo .bin");
            }

            std::filesystem::path Input(parameter2);
            dataVal.inputPathFile = Input/"data.bin";
        }
        else if (parameter1 == "-output") {
            if (!std::filesystem::exists(parameter2)){
                throw std::invalid_argument("Error: la direccion ingresada no existe");
            }

            std::filesystem::path Output(parameter2);
            dataVal.outputPath = Output;
            dataVal.outputPathFile = Output / "dataSorted.bin";
        }
        else if (parameter1 == "-alg") {

            dataVal.algorithm = getAlgorithm(parameter2);

        }
        else if (parameter1 == "-pageSize") {
            dataVal.pageSize = std::stoi(parameter2);
            if (dataVal.pageSize <= 0) {
                throw std::invalid_argument("el argumento pageSize debe ser un numero positivo");
            }
        }
        else if (parameter1 == "-pageCount") {
            dataVal.pageCount = std::stoi(parameter2);
            if (dataVal.pageCount <= 0) {
                throw std::invalid_argument("el argumento pageCount debe ser un numero positivo");
            }

        }
    }
    return dataVal;
}

void copyfile(const std::filesystem::path& inputpath, const std::filesystem::path& outputpath) {

    // abrir archivos
    std::ifstream  src(inputpath, std::ios::binary);
    std::ofstream  dst(outputpath,   std::ios::binary);

    // verificar si se abrieron correctamente los archivos
    if (!src.is_open()) {
        throw std::runtime_error(" No se pudo abrir el archivo de origen");
    }
    if (!dst.is_open()) {
        throw std::runtime_error("No se pudo crear el archivo de destino");
    }

    // escribir en el archivo
    dst << src.rdbuf();

    if (dst.fail()) {
        throw std::runtime_error("Error crítico: Falló la escritura en el destino.");
    }

    std::cout << "El archivo se copio correctamente" << std::endl;

}

int amountOfElements(const std::filesystem::path& inputPath) {
    // contar la cantidad de elementos en el archivo
    std::ifstream f(inputPath, std::ios::binary | std::ios::ate);
    if (!f.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo para contar elementos");
    }
    return  static_cast<int>(f.tellg() / sizeof(int));

}

void statistics(PagedArray& arr, const DataVal& variables, std::chrono::duration<double> time) {

    // obtener tiempo

    std::cout << "==== RESUMEN DE EJECUCION ====  " << std::endl;
    std::cout << "-> Algoritmo utilizado:  " << variables.algorithm << std::endl;
    std::cout << "-> Duracion:  " << time.count() << " s" <<std::endl;
    std::cout << "-> Cantidad de page hits:  " << arr.getPageHits() << std::endl;
    std::cout << "-> Cantidad de page Faults:  " << arr.getPageFaults() << std::endl;
    std::cout << "-> Cantidad de paginas:  " << arr.getPageCount() << std::endl;
    std::cout << "-> tamano de paginas:  " << arr.getPageSize() << std::endl;
    std::cout << "-> cantidad de elementos:  " << arr.getTotalElements() << std::endl;
}

void legibleFile(const std::filesystem::path& outputpath, const std::filesystem::path& inputPathFile) {
    // crear el archivo legible
    std::ifstream InputFile(inputPathFile, std::ios::binary);
    std::filesystem::path LegiblePath = outputpath/"dataSorted-LegibleFile.txt";
    std::ofstream LegibleFile(LegiblePath);

    // verificar si se creo correctamente el archivo legible
    if (!LegibleFile) {
        throw std::runtime_error("Error al abrir el archivo.");
    }

    // crear el buffer (De 4096 bytes)
    const size_t bufferSize = 4096;
    char buffer[bufferSize];

    //Escribir cabecera
    LegibleFile << "=== Proyecto PagedArray ===" << std::endl;
    LegibleFile << "Dato Ordenados:" << std::endl;

    //leer el archivo de acuerdo con la cantidad de bytes que caben en el buffer
    while (InputFile.read(buffer, bufferSize) || InputFile.gcount()>0 ) {

        int dataSize = sizeof(int);
        std::streamsize bytesRead = InputFile.gcount();

        // leer byte por byte del los datos ingresados en el buffer
        for (std::streamsize i = 0; i + dataSize <= bytesRead; i += dataSize) {

            // convertir el valor de binario a int
            int valor = *reinterpret_cast<int*>(&buffer[i]);

            LegibleFile << valor;

            LegibleFile << ",";
        }

    }

    InputFile.close();
    LegibleFile.close();

}

void sortFile( const DataVal& variables, int totalElements, PagedArray& arr) {

    if (variables.algorithm == "Quick Sort") {
        quickSort(arr, 0, totalElements-1);
    }
    else if (variables.algorithm == "Merge Sort") {
        mergeSort( arr, 0, totalElements-1);
    }
    else if (variables.algorithm == "Shell Sort") {
        shellSort(arr, totalElements-1);
    }
    else if (variables.algorithm == "Comb Sort") {
        combSort(arr, totalElements);
    }
    else if (variables.algorithm == "Tim Sort") {
        timSort(arr, totalElements);
    } else {
        throw std::runtime_error("Algoritmo no reconocido");
    }
}

int main(int argc, char* argv[]) {

    // verificar argumentos
    try {

        // colocar los argumentos en una estructura de variables
        const DataVal variables = RecivedArguments(argc, argv);
        std::cout << "Parametros recibidos correctamente \n";

        // copiar archivo input al output
        copyfile(variables.inputPathFile, variables.outputPathFile);

        // Instanciar un objeto de la clase PagedArray
        int totalElements = amountOfElements(variables.inputPathFile);
        PagedArray arr(variables.outputPathFile, variables.pageSize, variables.pageCount, totalElements);

        // iniciar reloj (ver cuanto dura ordenando archivo)
        auto start = std::chrono::high_resolution_clock::now();

        // ordenar archivo y asegurar escritura en disco
        sortFile(variables, totalElements, arr);
        arr.flush();

        // parar reloj
        const auto stop = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration<double>(stop - start);

        // Imprimir estadisticas

        statistics(arr,variables, duration);

        //hacer el archivo legible
        legibleFile(variables.outputPath, variables.outputPathFile);


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;


}