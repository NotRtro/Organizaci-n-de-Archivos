# Proyecto 1: Organización de Archivos

Este proyecto consiste en implementar dos técnicas de organización de archivos para datos reales que se encuentran en al menos dos archivos planos. Las técnicas que se implementarán son: Sequential File, AVL File, ISAM-Sparse Index, Extendible Hashing y B+ Tree.

## Objetivos del proyecto:
El objetivo del estudiante es entender y aplicar los algoritmos de almacenamiento de archivos físicos y acceso concurrente para poder implementarlo en la gestión de bases de datos organizadas en archivos de texto o archivos binarios.

## Instrucciones de compilación:

## Estructura del proyecto:

## Colaboradores:

## Enunciado
En grupos de hasta cuatro integrantes, se deberá elegir un dominio de datos que conste de al menos dos archivos planos con datos reales que se encuentran en Kaggle. Luego, sobre ellos se implementarán dos técnicas de organización de archivos, junto con sus principales operaciones: inserción, eliminación y búsqueda.

## Requerimientos generales
Se deben implementar dos de las siguientes técnicas de organización de archivos en memoria secundaria:
### 1. Sequential File:
Una técnica de organización de archivos llamada Sequential File (también conocida como "archivo secuencial") es un método simple y común para almacenar y buscar datos en un archivo en memoria secundaria.

En un archivo secuencial, los registros se almacenan en un orden determinado, siguiendo una secuencia específica. Esta secuencia puede ser cualquier ordenamiento que se desee, como orden alfabético, orden cronológico, etc.

Los registros en un archivo secuencial se almacenan uno tras otro, sin ningún espacio o separación entre ellos. Cada registro tiene una longitud fija, lo que significa que cada registro ocupa el mismo número de bytes en el archivo. Para buscar un registro específico, se debe recorrer el archivo secuencial desde el principio hasta el final hasta encontrar el registro deseado.

A continuación, te muestro un ejemplo de cómo crear un archivo secuencial en C++ y cómo realizar una búsqueda en él:

```cpp
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Registro {
    string nombre;
    int edad;
    // ... otros campos
};

void agregarRegistro(ofstream& archivo, const Registro& registro) {
    archivo.write(reinterpret_cast<const char*>(&registro), sizeof(registro));
}

void buscarRegistro(ifstream& archivo, const string& nombreBuscado) {
    Registro registro;
    while (archivo.read(reinterpret_cast<char*>(&registro), sizeof(registro))) {
        if (registro.nombre == nombreBuscado) {
            cout << "Se encontró el registro: " << registro.nombre << " " << registro.edad << endl;
            return;
        }
    }
    cout << "No se encontró ningún registro con el nombre " << nombreBuscado << endl;
}

int main() {
    ofstream archivo("miArchivo.dat", ios::binary);
    Registro registro1 = {"Juan", 25};
    Registro registro2 = {"Pedro", 30};
    agregarRegistro(archivo, registro1);
    agregarRegistro(archivo, registro2);
    archivo.close();

    ifstream archivoLectura("miArchivo.dat", ios::binary);
    buscarRegistro(archivoLectura, "Pedro");
    archivoLectura.close();
    return 0;
}
```
En este ejemplo, creamos un archivo binario llamado "miArchivo.dat" y agregamos dos registros (nombre y edad) al archivo utilizando la función agregarRegistro(). Luego, abrimos el archivo para lectura y realizamos una búsqueda de un registro con un nombre específico utilizando la función buscarRegistro(). La función recorre el archivo secuencial y compara el nombre de cada registro con el nombre buscado hasta encontrar un registro con el nombre deseado. Si no se encuentra ningún registro con el nombre buscado, se imprime un mensaje apropiado.

Es importante tener en cuenta que el archivo secuencial no es muy eficiente para realizar búsquedas en grandes conjuntos de datos, ya que se debe leer cada registro desde el principio del archivo hasta encontrar el registro deseado. Además, la eliminación de registros puede ser complicada, ya que puede dejar "huecos" en el archivo si se eliminan registros intermedios.

### 2. AVL File:
La estrategia de organización de archivos AVLFile en C++ es una técnica que permite almacenar registros en un archivo de forma ordenada y balanceada mediante la utilización de árboles AVL. Esta técnica es útil cuando se necesitan realizar operaciones de búsqueda, inserción y eliminación eficientes en un archivo de datos.

Un archivo AVLFile consta de una serie de nodos, cada uno de los cuales contiene un registro y un apuntador al siguiente nodo en el archivo. Los nodos se organizan en forma de árbol AVL, en el cual cada nodo tiene una altura que representa la longitud del camino más largo desde ese nodo hasta una hoja.

Cuando se realiza una operación de inserción en un archivo AVLFile, se busca la posición correcta en el árbol AVL para el nuevo registro y se inserta en un nuevo nodo. Luego, se realiza una serie de rotaciones en el árbol AVL para mantener el equilibrio y la altura adecuada.

Por otro lado, cuando se realiza una operación de búsqueda en un archivo AVLFile, se realiza una búsqueda binaria en el árbol AVL para encontrar el nodo que contiene el registro deseado. Esto permite una búsqueda eficiente en tiempo logarítmico en el número de nodos del árbol.

Finalmente, cuando se realiza una operación de eliminación en un archivo AVLFile, se busca el nodo que contiene el registro a eliminar y se realiza una serie de rotaciones para mantener el equilibrio y la altura adecuada en el árbol AVL. Luego, se marca el nodo como eliminado, pero no se elimina físicamente del archivo hasta que se realice una operación de compactación en el archivo.

En resumen, la estrategia de organización de archivos AVLFile en C++ utiliza árboles AVL para mantener un archivo de datos ordenado y balanceado, lo que permite realizar operaciones de búsqueda, inserción y eliminación eficientes en tiempo logarítmico en el número de nodos del árbol.

Para comenzar, se necesita definir una estructura para los nodos del árbol AVL que contendrá un registro y los punteros a sus hijos izquierdo y derecho, así como una variable de equilibrio para mantener el balance del árbol. Por ejemplo:

```cpp
struct NodoAVL {
    Registro registro;
    NodoAVL *izquierdo;
    NodoAVL *derecho;
    int equilibrio;
};
```
Luego, se necesita definir la estructura para el archivo AVLFile, que contendrá un puntero al nodo raíz del árbol y un identificador para el archivo. Por ejemplo:
```cpp
struct AVLFile {
    NodoAVL *raiz;
    int archivoID;
};
```
Para insertar un registro en un archivo AVLFile, se necesita implementar una función recursiva que recorra el árbol AVL comparando las llaves de los registros para determinar la posición correcta del nuevo registro. Luego, se realiza una serie de rotaciones en el árbol para mantener su equilibrio y altura adecuada. Por ejemplo:
```cpp
void insertarRegistroAVL(NodoAVL *&raiz, Registro registro) {
    if (raiz == NULL) {
        raiz = new NodoAVL;
        raiz->registro = registro;
        raiz->izquierdo = NULL;
        raiz->derecho = NULL;
        raiz->equilibrio = 0;
    } else if (registro.key < raiz->registro.key) {
        insertarRegistroAVL(raiz->izquierdo, registro);
        if (altura(raiz->izquierdo) - altura(raiz->derecho) == 2) {
            if (registro.key < raiz->izquierdo->registro.key) {
                rotacionSimpleIzquierda(raiz);
            } else {
                rotacionDobleIzquierda(raiz);
            }
        }
    } else if (registro.key > raiz->registro.key) {
        insertarRegistroAVL(raiz->derecho, registro);
        if (altura(raiz->derecho) - altura(raiz->izquierdo) == 2) {
            if (registro.key > raiz->derecho->registro.key) {
                rotacionSimpleDerecha(raiz);
            } else {
                rotacionDobleDerecha(raiz);
            }
        }
    }
    raiz->equilibrio = max(altura(raiz->izquierdo), altura(raiz->derecho)) + 1;
}
```
En el código anterior, las funciones **rotacionSimpleIzquierda**, **rotacionDobleIzquierda**, **rotacionSimpleDerecha** y **rotacionDobleDerecha** son funciones que realizan las rotaciones necesarias en el árbol AVL para mantener su equilibrio. La función altura devuelve la altura de un nodo del árbol.

Para realizar una búsqueda en un archivo **AVLFile**, se puede implementar una función recursiva que recorra el árbol AVL comparando las llaves de los registros hasta encontrar el registro deseado. Por ejemplo:
```cpp
Registro buscarRegistroAVL(NodoAVL *raiz, int key) {
    if (raiz == NULL) {
        Registro registroVacio;
        return registroVacio;
    } else if (key == raiz->registro.key) {
        return raiz->registro;
    } else
```




### 4. Extendible Hashing:
La estrategia de organización de archivos Extendible Hashing en C++ es un método de almacenamiento y búsqueda de datos en archivos que se basa en la utilización de una tabla hash y la división de los datos en bloques de tamaño fijo para facilitar la búsqueda y la actualización de los mismos.

En esta estrategia, se utiliza una tabla hash que contiene punteros a bloques de datos. Cada bloque de datos contiene un número fijo de elementos y está identificado por un número de bloque. Cuando se agrega un nuevo elemento a la tabla hash, se utiliza la función de hash para calcular su clave y se determina el número de bloque correspondiente. Si el bloque está lleno, se divide en dos bloques de la misma capacidad y se actualiza la tabla hash para que los nuevos bloques puedan ser encontrados en función de la clave del elemento.

Aquí hay un ejemplo de implementación de la estrategia de organización de archivos Extendible Hashing en C++:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int BLOCK_SIZE = 4;

struct Block {
    vector<int> data;
};

class HashTable {
private:
    int globalDepth;
    vector<int> directory;
    vector<Block> blocks;
    int hash(int key) {
        return key % (1 << globalDepth);
    }
    void split(int bucketIndex) {
        int localDepth = getLocalDepth(bucketIndex);
        int newBucketIndex = bucketIndex + (1 << (localDepth - 1));
        directory[newBucketIndex] = directory[bucketIndex];
        directory[bucketIndex] = newBucketIndex;
        blocks.push_back(blocks[directory[newBucketIndex]]);
        for (int i = BLOCK_SIZE - 1; i >= 0; --i) {
            if (hash(blocks[newBucketIndex].data[i]) != newBucketIndex) {
                blocks[directory[bucketIndex]].data.erase(remove(blocks[directory[bucketIndex]].data.begin(), blocks[directory[bucketIndex]].data.end(), blocks[newBucketIndex].data[i]), blocks[directory[bucketIndex]].data.end());
                blocks[directory[newBucketIndex]].data.push_back(blocks[newBucketIndex].data[i]);
                blocks[newBucketIndex].data.pop_back();
            }
        }
    }
public:
    HashTable() {
        globalDepth = 1;
        directory.resize(1 << globalDepth, 0);
        blocks.resize(1);
    }
    void insert(int key) {
        int bucketIndex = hash(key);
        if (blocks[bucketIndex].data.size() < BLOCK_SIZE) {
            blocks[bucketIndex].data.push_back(key);
        } else {
            int localDepth = getLocalDepth(bucketIndex);
            if (localDepth == globalDepth) {
                globalDepth++;
                directory.resize(1 << globalDepth, 0);
            }
            split(bucketIndex);
            insert(key);
        }
    }
    bool find(int key) {
        int bucketIndex = hash(key);
        return find(blocks[bucketIndex].data.begin(), blocks[bucketIndex].data.end(), key) != blocks[bucketIndex].data.end();
    }
    void print() {
        for (int i = 0; i < blocks.size(); ++i) {
            cout << "Bucket " << i << ": ";
            for (int j = 0; j < blocks[i].data.size(); ++j) {
                cout << blocks[i].data[j] << " ";
            }
            cout << endl;
        }
    }
    int getLocalDepth(int bucketIndex) {
        int mask = (1 << globalDepth) -    1;
    int bucketMask = bucketIndex & ((1 << globalDepth) - 1);
    int dirMask = directory[bucketMask] & ((1 << globalDepth) - 1);
    while (bucketMask != dirMask) {
        mask++;
        bucketMask &= ~(1 << (mask - 1));
        dirMask &= ~(1 << (mask - 1));
    }
    return mask;
}};

int main() {
HashTable ht;
ht.insert(1);
ht.insert(2);
ht.insert(3);
ht.insert(4);
ht.insert(5);
ht.insert(6);
ht.insert(7);
ht.insert(8);
ht.insert(9);
ht.print();
cout << ht.find(1) << endl;
cout << ht.find(10) << endl;
return 0;
}

```

En este ejemplo, la estructura `Block` representa un bloque de datos que contiene un vector de enteros. La clase `HashTable` representa la tabla hash y contiene un vector de bloques y un vector de enteros que representa el directorio. El tamaño de bloque se define como una constante `BLOCK_SIZE`. La función `hash` calcula el índice de la tabla hash en función de la clave del elemento.

La función `split` se utiliza para dividir un bloque cuando se agrega un nuevo elemento y el bloque está lleno. El directorio y la tabla de bloques se actualizan para reflejar la nueva estructura de bloques.

La función `insert` se utiliza para agregar un nuevo elemento a la tabla hash. Si el bloque correspondiente está lleno, se utiliza la función `split` para dividir el bloque y agregar el nuevo elemento en el bloque correspondiente.

La función `find` se utiliza para buscar un elemento en la tabla hash. Si el elemento se encuentra en el bloque correspondiente, la función devuelve `true`. En caso contrario, devuelve `false`.

La función `print` se utiliza para imprimir el contenido de la tabla hash en la consola.

En el `main`, se crea una nueva instancia de la tabla hash, se agregan algunos elementos y se imprime el contenido de la tabla hash. Luego se realizan algunas búsquedas de elementos en la tabla hash.


## Consideraciones:


Se debe escoger una de las siguientes combinaciones de técnicas de organización de archivos:
- Sequential File o AVL File o ISAM-Sparse Index
- Extendible Hash o B+Tree

Además, se deberán implementar las siguientes operaciones:
1. Búsqueda específica que puede retornar más de un elemento que coincide con la clave: `vector<Registro> search(T key)`
2. Búsqueda por rango que retorna todos los registros que calzan entre dos claves de búsqueda: `vector<Registro> rangeSearch(T begin_key, T end_key)`
3. Agregar un registro al archivo respetando la técnica de organización: `bool add(Registro registro)`
4. Proponer un algoritmo de eliminación para cada técnica: `bool remove(T key)`

c. Se debe implementar un parser de código SQL a sentencias ejecutables.

d. Se debe mostrar los resultados de forma amigable al usuario, utilizando la siguiente GUI de referencia.

## GUI de referencia
La GUI debe mostrar los resultados de las operaciones realizadas de forma amigable al usuario. Se sugiere la siguiente GUI de referencia:

![GUI de referencia](/imagenes/gui.jpg)
