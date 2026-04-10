# Proyecto Arreglos Paginados
Este proyecto implementa un sistema de ordenamiento de archivos binarios de gran tamaño basado en el concepto de paginación de memoria. El proyecto tiene dos componentes principales: un generador de archivos binarios con números enteros aleatorios y un ordenador de archivos que utiliza la clase PagedArray para simular el manejo de memoria paginada. La clase PagedArray permite ordenar archivos que superan la capacidad de la memoria RAM, cargando y descargando páginas del disco según sea necesario, mientras que los algoritmos de ordenamiento interactúan con ella de forma transparente como si fuera un arreglo convencional.

## Requisitos
- Compilador C++ con soporte C++20
- CMake 3.20 o superior

## Compilación
Para ejecutar el proyecto "Arreglos Paginados" es necesario hacerlo desde la terminal. 

### CLion (recomendado)
En caso de utilizar el IDE de CLion (donde fue desarrollado el proyecto) se deben seguir los siguientes pasos:
1. Abrir CLion
2. Dar clic en la opción del menú superior "Open" y seleccionar la carpeta donde se encuentra el proyecto.
3. Presionar el botón de build (martillo)

CLion detectará el archivo `CMakeLists.txt` automáticamente y creará una carpeta llamada `cmake-build-debug/` que contendrá los ejecutables del proyecto. Luego de esto, para ejecutar desde la terminal, es necesario dirigirse a la carpeta `cmake-build-debug/` con ayuda del comando `cd cmake-build-debug`. Una vez ahí se puede ejecutar el programa desde la terminal de la manera que se detalla a continuación.

### Windows (terminal)
En caso de ejecutar el proyecto desde la terminal de Windows es necesario seguir los siguientes pasos:
1. Abrir PowerShell o CMD
2. Navegar hasta la carpeta raíz del proyecto
3. Ejecutar los siguientes comandos para generar los archivos de compilación y ejecutar el proyecto:
```bash
cd cmake-build-debug
cmake -G "MinGW Makefiles" ..
mingw32-make
```
Una vez ejecutados los comandos en PowerShell o CMD, se puede ejecutar el programa desde la terminal de la manera que se detalla a continuación.

### Linux (terminal)
En caso de ejecutar el proyecto desde la terminal de Linux es necesario seguir los siguientes pasos:
1. Abrir una terminal
2. Navegar hasta la carpeta raíz del proyecto
3. Ejecutar los siguientes comandos para generar los archivos de compilación y ejecutar el proyecto:
```bash
mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
make
```
Una vez ejecutados los comandos, se puede ejecutar el programa desde la terminal de la manera que se detalla a continuación.

## Generador de archivos

El primer programa del proyecto se encarga de generar archivos binarios de números enteros aleatorios del tamaño solicitado por el usuario. 

### Uso
Para utilizar el programa es necesario hacerlo desde la terminal utilizando el siguiente comando:
```bash
./generator -size <SIZE> -output <OUTPUT PATH>
```

### Parámetros
- `-size`: Este parámetro indica el tamaño del archivo a generar, el cual puede ser:
  - `SMALL`  → 256 MB
  - `MEDIUM` → 512 MB
  - `LARGE`  → 1 GB
- `-output`: Este parámetro indica la ruta de la carpeta donde se guardará el archivo generado.

### Ejemplo
```bash
./generator -size SMALL -output "C:/Documentos"
```
Genera un archivo binario de números enteros de 256 MB en la ruta: `C:/Documentos/data.bin`

---

## Ordenador de archivos
La segunda parte del proyecto se encarga de ordenar los archivos binarios creados por el generador de archivos. El ordenador de archivos implementa la clase PagedArray, que permite manejar la memoria de forma paginada, simulando el comportamiento de un sistema de memoria virtual. Los algoritmos de ordenamiento son los encargados de ordenar estos archivos utilizando la clase PagedArray como si fuera un arreglo común, gracias a la sobrecarga del operador[] (manejada en PagedArray).

### Uso
```bash
./sorter -input <INPUT PATH> -output <OUTPUT PATH> -alg <ALGORITMO> -pageSize <PAGE-SIZE> -pageCount <PAGE-COUNT>
```

### Parámetros
- `-input`: Indica la ruta de la carpeta donde está el archivo binario a ordenar
- `-output`: Indica la ruta de la carpeta donde se guardará el archivo ordenado
- `-alg`: Establece el algoritmo de ordenamiento a utilizar, el cual puede ser:
  - `quicksort`
  - `mergesort`
  - `shellsort`
  - `combsort`
  - `radixsort`
- `-pageSize`: Indica la cantidad máxima de enteros por página
- `-pageCount`: Indica la cantidad de páginas en memoria RAM

### Ejemplo
```bash
./sorter -input "C:/Documentos/input" -output "C:/Documentos/output" -alg "quicksort" -pageSize 1024 -pageCount 64
```

> [!IMPORTANT]
> El parámetro `-alg` acepta los valores establecidos anteriormente. El programa es insensible a mayúsculas y minúsculas, sin embargo el nombre debe escribirse exactamente como se muestra, sin espacios adicionales ni caracteres especiales.

### Archivos generados
El sorter genera dos archivos en la carpeta output:
- `dataSorted.bin` → archivo binario ordenado
- `dataSorted-LegibleFile.txt` → archivo legible con los datos separados por comas

### Resumen de ejecución
Al finalizar el ordenamiento se imprime un resumen con:
- Algoritmo utilizado
- Duración en segundos
- Cantidad de page hits
- Cantidad de page faults
- Cantidad de páginas en RAM
- Tamaño de página
- Cantidad de elementos ordenados

Ejemplo:
```bash
PS C:\Documentos\Algoritmos y estructuras de datos II\Page_Array_Proyect\cmake-build-debug > ./sorter -input "C:/Documentos" -output "C:/Documentos" -alg "quicksort" -pageSize 1024 -pageCount 64                                                                                                                                                                       
Parametros recibidos correctamente
El archivo se copió correctamente
==== RESUMEN DE EJECUCION ====  
-> Algoritmo utilizado:  Quick Sort
-> Duracion:  68.2704 s
-> Cantidad de page hits:  4562974994
-> Cantidad de page Faults:  1369635
-> Cantidad de paginas:  64
-> tamano de paginas:  1024
-> cantidad de elementos:  67108864
```

## Autores
- **Alanna Mendoza Fonseca** - 2025082215

**Curso:** Algoritmos y Estructuras de Datos II (CE 1103)

**Institución:** Instituto Tecnológico de Costa Rica

**Semestre:** I Semestre 2026
