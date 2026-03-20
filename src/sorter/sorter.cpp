#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>

enum class Algorithm {
    BUBBLE,
    INSERTION,
    SELECTION,
    MERGE,
    QUICK
};

struct DataVal {
    std::filesystem::path inputPath;
    std::filesystem::path outputPath;
    Algorithm algorithm;
    size_t pageSize;
    size_t pageCount;
};

Algorithm getAlgorithm(std::string value) {

    std::string Valuelower = value;
    std::transform(Valuelower.begin(), Valuelower.end(), Valuelower.begin(), ::tolower);

    if (value == "bubble") return Algorithm::BUBBLE;
    if (value == "insertion") return Algorithm::INSERTION;
    if (value == "selection" ) return Algorithm::SELECTION;
    if (value == "merge") return Algorithm::MERGE;
    if (value == "quick sort") return Algorithm::QUICK;

    throw std::invalid_argument("Algoritmo no valido");

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
            dataVal.inputPath = parameter2;
        }

        else if (parameter1 == "-output") {
            if (!std::filesystem::exists(parameter2)){
                throw std::invalid_argument("Error: la direccion ingresada no existe");
            }

            dataVal.outputPath = parameter2;
        }
        else if (parameter1 == "-alg") {

            dataVal.algorithm = getAlgorithm(parameter2);

        }
        else if (parameter1 == "-pageSize") {
            dataVal.pageSize = std::stoull(parameter2);
        }
        else if (parameter1 == "-pageCount") {
            dataVal.pageCount = std::stoull(parameter2);

        }
    }
    return dataVal;
}

void copyfile( std::filesystem::path inputpath, std::filesystem::path outputpath) {

    std::filesystem::path completePathInput = inputpath / "data.bin";
    std::filesystem::path completePathOutput = inputpath / "dataSorted.bin";

    // abrir archivos
    std::ifstream  src(completePathInput, std::ios::binary);
    std::ofstream  dst(completePathOutput,   std::ios::binary);

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


int main(int argc, char* argv[]) {

    // verificar argumentos
    try {

        // colocar los argumentos en una estructura de variables
        DataVal variables = RecivedArguments(argc, argv);
        std::cout << "Parametros recibidos correctamente correctamente\n";

        // copiar archivo input al output
        copyfile(variables.inputPath, variables.outputPath);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;


}