#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <random>

size_t getCapacity(const std::string& size) {
    if (size == "SMALL") return 32;
    if (size == "MEDIUM") return 64;
    if (size == "LARGE") return 128;
    throw std::invalid_argument("Ingrese una capacidad valida: SMALL - MEDIUM - LARGE");
}

int generate(const std::string& size, const std::filesystem::path& path1) {

    // Semilla para numeros aleatorios
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);

    // Calcular tamano del archivo
    const size_t capacity = getCapacity(size);

    // Crear archivo
    std::filesystem::path filePath = path1 / "data.bin";
    std::ofstream outFile(filePath, std::ios::binary);

    // Verificar que el archivo se abrio correctamente
    if (!outFile.is_open()) {
        std::cerr << "Error: no se pudo crear el archivo en: " << filePath << std::endl;
        return 1;
    }

    //Guardar numeros binarios en el archivo abierto
    const size_t n = (capacity * 1024 * 1024) / sizeof(int);
    for (size_t i = 0; i < n; i++) {
        int numero = dist(rng);
        outFile.write(reinterpret_cast<char*>(&numero), sizeof(int));

        // Verificar que la escritura no fallo
        if (outFile.fail()) {
            std::cerr << "Error: fallo la escritura en el archivo" << std::endl;
            outFile.close();
            return 1;
        }
    }
    outFile.close();

    if (outFile.fail()) {
        std::cerr << "Error: fallo al cerrar el archivo" << std::endl;
        return 1;
    }

    std::cout << "Generator: Archivo generado correctamente" << std::endl;
    return 0;

}

int main(int argc, char* argv[]) {
    // VERIFICAR ARGUMENTOS RECIBIDOS
    if (argc != 5) {
        std::cerr << "Error, Uso correcto: generator -size <SIZE> -output <PATH>";
        return 1;
    }

    if (std::string(argv[1]) != "-size") {
        std::cerr << "Error: se esperaba el argumento -size";
        return 1;
    }

    if (std::string(argv[3]) != "-output") {
        std::cerr << "Error: se esperaba el argumento -output";
        return 1;
    }

    // Inicializar variables
    std::string size = argv[2];
    std::filesystem::path path1 = argv[4] ;

    // VERIFICAR VARIABLES RECIBIDAS

    //Validacion de size

    if ( size != "SMALL" && size != "MEDIUM" && size != "LARGE") {
        std::cerr << "Error: el tamano del archivo debe ser: SMALL, MEDIUM O LARGE";
        return 1;
    }

    // Validaciones de path
    if (!std::filesystem::exists(path1)) {
        std::cerr << "Error: La ruta ingresada no existe";
        return 1;
    }

    if (!std::filesystem::is_directory(path1)) {
        std::cerr << "Error: La ruta ingresada no es un directorio";
        return 1;
    }


    //GENERADOR DE ARCHIVOS
    generate(size,path1);

    return 0;
}

