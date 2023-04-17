---
title: "Codigos Estadística"
subtitle: "armani"
output:
  html_document:
    toc: true
    toc_float: true
    toc_depth: 3
    number_sections: TRUE
---

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


### 3. ISAM-Sparse Index:
La organización de archivos ISAM-Sparse Index es una técnica que permite organizar los datos en un archivo mediante la creación de un índice para acelerar las búsquedas. Esta técnica utiliza una tabla de índices esparsa para mejorar la eficiencia de acceso a los registros.

En ISAM-Sparse Index, se divide el archivo de datos en bloques de tamaño fijo, y se crea un índice que apunta al inicio de cada bloque en el archivo. El índice es una tabla de búsqueda que contiene sólo los registros que apuntan al comienzo de cada bloque.

Cuando se busca un registro, se busca primero en la tabla de índice para encontrar el bloque que contiene el registro, y luego se busca en el bloque correspondiente.

A continuación se muestra un ejemplo de cómo implementar ISAM-Sparse Index en C++:

```cpp
struct Registro {
    int llave;
    string nombre;
    // ... otros campos
};

class ISAM {
private:
    fstream archivo;
    vector<int> indice;
    int tamBloque;
    int numRegistros;

    // Función para actualizar la tabla de índice
    void actualizarIndice(int posicion, int offset) {
        int bloque = posicion / tamBloque;
        if (bloque >= indice.size()) {
            indice.resize(bloque + 1);
        }
        if (indice[bloque] == 0 || indice[bloque] > offset) {
            indice[bloque] = offset;
        }
    }

public:
    // Constructor
    ISAM(string nombreArchivo, int tamBloque) {
        this->tamBloque = tamBloque;
        numRegistros = 0;
        archivo.open(nombreArchivo, ios::in | ios::out | ios::binary | ios::trunc);
        if (!archivo) {
            cerr << "Error al abrir el archivo" << endl;
        }
    }

    // Función para agregar un registro
    bool add(Registro registro) {
        int offset = numRegistros * sizeof(Registro);
        archivo.seekp(offset, ios::beg);
        archivo.write(reinterpret_cast<char*>(&registro), sizeof(registro));
        actualizarIndice(offset, numRegistros);
        numRegistros++;
        return true;
    }

    // Función para buscar un registro por llave
    Registro search(int llave) {
        int bloque = llave / tamBloque;
        int inicio = indice[bloque];
        int fin = (bloque == indice.size() - 1) ? archivo.tellg() : indice[bloque+1];
        Registro registro;
        for (int offset = inicio; offset < fin; offset += sizeof(registro)) {
            archivo.seekg(offset, ios::beg);
            archivo.read(reinterpret_cast<char*>(&registro), sizeof(registro));
            if (registro.llave == llave) {
                return registro;
            }
        }
        // No se encontró el registro
        registro.llave = -1;
        return registro;
    }
};

```
En este ejemplo, la clase ISAM representa el archivo y contiene una tabla de índice indice que se actualiza cada vez que se agrega un registro. La función add agrega un registro al archivo y actualiza la tabla de índice. La función search busca un registro por llave y utiliza la tabla de índice para encontrar el bloque que contiene el registro. Luego, busca en el bloque correspondiente hasta que encuentra el registro o llega al final del bloque.

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


### 5. B+ Tree:
Un árbol B+ es una estructura de datos que se utiliza para almacenar y organizar grandes cantidades de datos en un archivo o base de datos. El árbol B+ está diseñado específicamente para el acceso rápido y eficiente a datos en disco y es comúnmente utilizado en sistemas de bases de datos y sistemas de archivos.

El árbol B+ se organiza como un árbol de búsqueda binario equilibrado, donde cada nodo interno tiene una lista de claves que divide a los subárboles. Sin embargo, a diferencia de un árbol de búsqueda binario, el árbol B+ almacena solo claves en los nodos internos y todos los datos se almacenan en las hojas.

Cada hoja en un árbol B+ contiene un rango de claves y un puntero al siguiente nodo de hoja. Los nodos hoja se organizan en una estructura de lista vinculada, lo que facilita el recorrido de todas las claves almacenadas en el árbol.

Un ejemplo de implementación de un árbol B+ en C++ se presenta a continuación:

```cpp
class BPlusTreeNode {
public:
    int *keys;
    int maxKeys;
    BPlusTreeNode **children;
    int keyCount;
    bool isLeaf;

    BPlusTreeNode(int maxKeys, bool isLeaf) {
        this->maxKeys = maxKeys;
        this->isLeaf = isLeaf;
        keys = new int[maxKeys];
        children = new BPlusTreeNode *[maxKeys + 1];
        keyCount = 0;
    }

    ~BPlusTreeNode() {
        delete[] keys;
        delete[] children;
    }
};

class BPlusTree {
public:
    BPlusTreeNode *root;
    int maxKeys;

    BPlusTree(int maxKeys) {
        this->maxKeys = maxKeys;
        root = new BPlusTreeNode(maxKeys, true);
    }

    ~BPlusTree() {
        destroyTree(root);
    }

    void insert(int key) {
        BPlusTreeNode *newNode = insert(root, key);
        if (newNode != nullptr) {
            BPlusTreeNode *oldRoot = root;
            root = new BPlusTreeNode(maxKeys, false);
            root->keys[0] = newNode->keys[0];
            root->children[0] = oldRoot;
            root->children[1] = newNode;
            root->keyCount = 1;
        }
    }

    void remove(int key) {
        remove(root, key);
    }

    void print() {
        print(root);
    }

private:
    BPlusTreeNode *insert(BPlusTreeNode *node, int key) {
        int i;
        if (node->isLeaf) {
            for (i = 0; i < node->keyCount; i++) {
                if (node->keys[i] == key) {
                    return nullptr;
                }
                if (node->keys[i] > key) {
                    break;
                }
            }
            for (int j = node->keyCount; j > i; j--) {
                node->keys[j] = node->keys[j - 1];
            }
            node->keys[i] = key;
            node->keyCount++;
            if (node->keyCount > maxKeys) {
                return split(node);
            }
            return nullptr;
        } else {
            for (i = 0; i < node->keyCount; i++) {
                if (node->keys[i] == key) {
                    return nullptr;
                }
                            if (node->keys[i] > key) {
                break;
            }
        }
        BPlusTreeNode *newNode = insert(node->children[i], key);
        if (newNode == nullptr) {
            return nullptr;
        }
        for (int j = node->keyCount; j > i; j--) {
            node->keys[j] = node->keys[j - 1];
            node->children[j + 1] = node->children[j];
        }
        node->keys[i] = newNode->keys[0];
        node->children[i + 1] = newNode;
        node->keyCount++;
        if (node->keyCount > maxKeys) {
            return split(node);
        }
        return nullptr;
    }
}

void remove(BPlusTreeNode *node, int key) {
    if (node->isLeaf) {
        int i = 0;
        while (i < node->keyCount && node->keys[i] < key) {
            i++;
        }
        if (i < node->keyCount && node->keys[i] == key) {
            for (int j = i; j < node->keyCount - 1; j++) {
                node->keys[j] = node->keys[j + 1];
            }
            node->keyCount--;
        }
    } else {
        int i = 0;
        while (i < node->keyCount && node->keys[i] <= key) {
            i++;
        }
        remove(node->children[i], key);
        if (node->children[i]->keyCount < (maxKeys + 1) / 2) {
            coalesce(node, i);
        }
    }
}

BPlusTreeNode *split(BPlusTreeNode *node) {
    BPlusTreeNode *newNode = new BPlusTreeNode(maxKeys, node->isLeaf);
    int mid = node->keyCount / 2;
    int i, j;
    for (i = mid + 1, j = 0; i < node->keyCount; i++, j++) {
        newNode->keys[j] = node->keys[i];
        node->children[i]->isLeaf = node->isLeaf;
        newNode->children[j] = node->children[i];
        node->children[i] = nullptr;
        newNode->keyCount++;
        node->keyCount--;
    }
    newNode->children[j] = node->children[i];
    node->children[i] = nullptr;
    node->keyCount--;
    return newNode;
}

void coalesce(BPlusTreeNode *node, int index) {
    BPlusTreeNode *left = node->children[index - 1];
    BPlusTreeNode *right = node->children[index];
    int i, j;
    if (!left->isLeaf) {
        left->keys[left->keyCount] = node->keys[index - 1];
        left->keyCount++;
        for (i = 0, j = left->keyCount; i < right->keyCount; i++, j++) {
            left->keys[j] = right->keys[i];
            left->children[j] = right->children[i];
            left->keyCount++;
        }
        left->children[j] = right->children[i];
    } else {
        for (i = 0, j = left->keyCount; i < right->keyCount; i++, j++) {
            left->keys[j] = right->keys[i];
            left->keyCount++;
        }
        left->children[maxKeys] = right->children[maxKeys];
    }
    for (i = index - 1; i < node->keyCount            ; i++) {
            node->keys[i] = node->keys[i + 1];
            node->children[i + 1] = node->children[i + 2];
        }
        node->keyCount--;
        delete right;
    }
}

void print(BPlusTreeNode *node, int level) {
    if (node->isLeaf) {
        for (int i = 0; i < level; i++) {
            cout << " ";
        }
        for (int i = 0; i < node->keyCount; i++) {
            cout << node->keys[i] << " ";
        }
        cout << endl;
    } else {
        print(node->children[0], level + 1);
        for (int i = 0; i < level; i++) {
            cout << " ";
        }
        for (int i = 0; i < node->keyCount; i++) {
            cout << node->keys[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < node->keyCount + 1; i++) {
            print(node->children[i + 1], level + 1);
        }
    }
}
public:
BPlusTree(int maxKeys) {
root = nullptr;
this->maxKeys = maxKeys;
}
void insert(int key) {
    if (root == nullptr) {
        root = new BPlusTreeNode(maxKeys, true);
        root->keys[0] = key;
        root->keyCount = 1;
    } else {
        BPlusTreeNode *newNode = insert(root, key);
        if (newNode != nullptr) {
            BPlusTreeNode *newRoot = new BPlusTreeNode(maxKeys, false);
            newRoot->keys[0] = newNode->keys[0];
            newRoot->children[0] = root;
            newRoot->children[1] = newNode;
            newRoot->keyCount = 1;
            root = newRoot;
        }
    }
}

void remove(int key) {
    if (root == nullptr) {
        return;
    }
    remove(root, key);
    if (root->keyCount == 0) {
        BPlusTreeNode *oldRoot = root;
        if (root->isLeaf) {
            root = nullptr;
        } else {
            root = root->children[0];
        }
        delete oldRoot;
    }
}

void print() {
    if (root != nullptr) {
        print(root, 0);
    }
}
};

int main() {
BPlusTree tree(3);
tree.insert(3);
tree.insert(7);
tree.insert(10);
tree.insert(12);
tree.insert(14);
tree.insert(15);
tree.insert(16);
tree.insert(19);
tree.insert(20);
tree.insert(22);
tree.insert(23);
tree.insert(24);
tree.insert(26);
tree.insert(27);
tree.insert(30);
tree.insert(33);
tree.insert(34);
tree.insert(38);
tree.insert(39);
tree.insert(41);
tree.insert(47);
tree.insert(50);
tree.insert(59);
tree.print();
tree.remove(38);
tree.print();
tree.remove(39);
tree.print();
return 0;
}

```
En este ejemplo, la clase BPlusTreeNode representa los nodos del árbol B+. Contiene un arreglo de llaves, un arreglo de punteros a los hijos, un flag isLeaf que indica si el nodo es una hoja o un nodo interno y un entero keyCount que indica el número de llaves en el nodo.

La clase BPlusTree contiene un puntero al nodo raíz, el número máximo de llaves permitido por nodo y las funciones insert, remove y print.

La función insert es recursiva y comienza buscando la hoja donde se debe insertar la llave. Si la hoja tiene espacio, simplemente se inserta la llave en la hoja. De lo contrario, se divide la hoja y se inserta la llave en la hoja adecuada.

Si la división de una hoja produce una nueva raíz, se crea un nuevo nodo interno y se establece como la nueva raíz del árbol.

La función remove también es recursiva y comienza buscando la hoja donde se encuentra la llave. Si la llave no está presente en la hoja, se busca en el nodo interno correspondiente y se borra recursivamente.

Si la eliminación de una llave produce una hoja con menos llaves de las permitidas, se realizan operaciones de redistribución o fusión para restaurar la estructura del árbol.

La función print recorre el árbol en orden de profundidad y muestra las llaves en cada nivel.

El programa de ejemplo crea un árbol B+ con un número máximo de llaves por nodo de 3 y luego inserta 24 llaves. Finalmente, se eliminan dos llaves y se muestra el árbol resultante después de cada eliminación.


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

![GUI de referencia](https://i.imgur.com/rOuO7yE.png)
