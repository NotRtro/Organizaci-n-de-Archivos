# Proyecto 1: Organización de Archivos

![GUI de referencia](/imagenes/capibara.jpg)

Este proyecto consiste en implementar dos técnicas de organización de archivos para datos reales que se encuentran en al menos dos archivos planos. Las técnicas que se implementarán son: Sequential File, AVL File, ISAM-Sparse Index, Extendible Hashing y B+ Tree.


## Objetivos del proyecto:
El objetivo del estudiante es entender y aplicar los algoritmos de almacenamiento de archivos físicos y acceso concurrente para poder implementarlo en la gestión de bases de datos organizadas en archivos de texto o archivos binarios.

## Instrucciones de compilación:
Para poder correr la GUI, necesitamos descargar la carpeta GUI, luego de eso, instalaremos QTCreator.
Una vez instalado el programa nos dirigiremos a "Abrir proyecto" y seleccionaremos la carpeta descargada "GUI".
El resto solo seria ejecutar el código
## Estructura del proyecto:

## Colaboradores:
ANGEL TITO
Rodo Vilcarromero
Ronaldo Flores
Salva
Jeffry 


### 1. Sequential File:
Una técnica de organización de archivos llamada Sequential File (también conocida como "archivo secuencial") es un método simple y común para almacenar y buscar datos en un archivo en memoria secundaria.

En un archivo secuencial, los registros se almacenan en un orden determinado, siguiendo una secuencia específica. Esta secuencia puede ser cualquier ordenamiento que se desee, como orden alfabético, orden cronológico, etc.

Los registros en un archivo secuencial se almacenan uno tras otro, sin ningún espacio o separación entre ellos. Cada registro tiene una longitud fija, lo que significa que cada registro ocupa el mismo número de bytes en el archivo. Para buscar un registro específico, se debe recorrer el archivo secuencial desde el principio hasta el final hasta encontrar el registro deseado.

A continuación, te muestro un ejemplo de cómo crear un archivo secuencial en C++ y cómo realizar una búsqueda en él:
**CÓDIGO:
```cpp

#ifndef BASEDATOS2_SQUENTIAL_H
#define BASEDATOS2_SQUENTIAL_H
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>
#include <vector>

using namespace std;

class squential {
    string datos="../Data_from_Structurs/dataSecuencial.dat",aux="../Data_from_Structurs/datasecaux.dat",datosVector="../Data_from_Structurs/dataAuxSecuencial.dat";
//    string datos="../dataSecuencial.dat",aux="../dataSecAux.dat",datosVector="../dataAuxSecuencial.dat";
    pair<int,char> header;
    long n = 20000;
    long k = log2(n);
    int eliminados=0;
public:
    squential(){
        header.first=-1;header.second='d';
        ofstream file(datos,ios::binary);
        file.close();
        ofstream file1(aux,ios::binary);
        file1.close();
    }
    struct Record {
        char id[7];
        char prenda[10];
        char genero[1];
        float precio;
        char stock[4];
        char marca[7];
        pair<int, char> puntero;
        Record() {}
        Record(string id_,string prenda_,string genero_,float precio_,string stock_,string marca_){
            for(int i=0;i<7;i++){
                this->id[i]=id_[i];
            }
            for(int i=0;i<10;i++){
                this->prenda[i]=prenda_[i];
            }
            for(int i=0;i<1;i++){
                this->genero[i]=genero_[i];
            }
            this->precio=precio_;
            for(int i=0;i<4;i++){
                this->stock[i]=stock_[i];
            }
            for(int i=0;i<7;i++){
                this->marca[i]=marca_[i];
            }
            puntero={0,'r'};
        }
    };

    friend bool operator<(Record &r1, Record &r2) {
        return r1.id < r2.id;
    }

    friend bool operator>(Record &r1, Record &r2) {
        return r1.id > r2.id;
    }
    friend bool operator==(Record &r1, Record &r2) {
        return r1.id == r2.id;
    }
    friend bool operator!=(Record &r1, Record &r2) {
        return r1.id != r2.id;
    }

    bool add(Record key) {

        if(header.first==-1){
            fstream file;
            file.open(datos,ios::out|ios::binary);
            key.puntero=header;
            header.first=0;
            header.second='d';
            file.write((char*)&key, sizeof(Record));
            file.close();
            return true;
        }
        else if(size()<n){
            fstream file;
            pair<Record,pair<int,char>>encontrado=search_Record(key);
            if(encontrado.second.first==-1){//actualizar el header
                file.open(aux,ios::app|ios::binary);
                pair<int,char>temp=header;
                header.first=(file.tellp()/sizeof(Record));
                header.second='a';
                key.puntero=temp;
                file.write((char*)&key, sizeof(Record));
            }
            else{
                cout<<"llave mayor";
                file.open(aux,ios::app|ios::binary);
                pair<int,char>temp=encontrado.first.puntero;
                encontrado.first.puntero.first=(file.tellp()/sizeof(Record));
                encontrado.first.puntero.second='a';
                key.puntero=temp;
                file.write((char*)&key,sizeof(Record));

                //actualizar el dato
                fstream actRegistro(datos,ios::out|ios::in|ios::ate|ios::binary);
                actRegistro.seekp(encontrado.second.first*sizeof(Record));
                actRegistro.write((char*)&encontrado.first,sizeof(Record));
                actRegistro.close();
            }
            file.close();
            rebuild();
            return true;
        }
        else{
            pair<Record,pair<int,char>>encontrado=search_Record(key);
            if(encontrado.second.first==-1){
                fstream file;
                file.open(aux,ios::app|ios::binary);
                if(header.second=='d'){
                    pair<int,char>temp=header;
                    header.first=(file.tellp()/sizeof(Record));
                    header.second='a';
                    key.puntero=temp;
                    file.write((char*)&key, sizeof(Record));
                }else{
                    ifstream leer(aux,ios::binary);//anis
                    Record prn,tmpRecord;
                    int u=header.first;//1
                    leer.seekg(u*sizeof(Record));//an
                    leer.read((char*)&tmpRecord,sizeof(Record));//an0a
                    prn=tmpRecord;
                    if(tmpRecord<key||tmpRecord.puntero.second!='d'){// an<anis
                        while(tmpRecord<key&&tmpRecord.puntero.second!='d'){// an<anis y 0a!=d, ana < ane y 2a!=d, anuel<anis
                            prn=tmpRecord;
                            leer.seekg(tmpRecord.puntero.first*sizeof(Record));//0->ana2a, anuel
                            leer.read((char*)&tmpRecord,sizeof(Record));//ana2a anuel0d
                            u=tmpRecord.puntero.first;//0a 0d
                        }if(tmpRecord>key&&prn<key){

                            tmpRecord=prn;
                        }
                        leer.close();
                        pair<int,char>temp=tmpRecord.puntero;// 2a
                        tmpRecord.puntero.first=(file.tellp()/sizeof(Record));//3a
                        tmpRecord.puntero.second='a';
                        key.puntero=temp;//ane2a
                        file.write((char*)&key,sizeof(Record));//ane2a
                        file.close();

                        //actualizar
                        file.open(aux,ios::out|ios::in|ios::ate|ios::binary);
                        file.seekp(u*sizeRecord());//0
                        file.write((char*)&tmpRecord,sizeof(Record));//ana2a
                    }else{
                        pair<int,char>temp=header;//0a
                        header.first=(file.tellp()/sizeof(Record));//1
                        header.second='a';
                        key.puntero=temp;//an0a
                        file.write((char*)&key, sizeof(Record));
                    }
                }
                file.close();
            }
            else {
                int u=encontrado.second.first;//5
                fstream actRegistro;
                if(encontrado.second.second=='a'){//5a
                    actRegistro.open(aux,ios::out|ios::in|ios::ate|ios::binary);
                    Record prn,tmp;
                    tmp=encontrado.first;//ronaldo1a
                    actRegistro.seekg(encontrado.first.puntero.first*sizeof(Record));
                    actRegistro.read((char*)&prn,sizeof(Record));//sapo-1d
                    u=encontrado.first.puntero.first;//1
                    if(prn<key||prn.puntero.second!='d'){
                        while(prn<key&&prn.puntero.second!='d'){//sapo<ser y d!=d
                            actRegistro.seekg(prn.puntero.first*sizeof(Record));
                            actRegistro.read((char*)&prn,sizeof(Record));//jarold 4d
                            u=prn.puntero.first;// 0
                        }
                    }else{
                        prn=tmp;
                    }
                    if(encontrado.first==prn){//ronaldo==sapo
                        actRegistro.close();
                        actRegistro.open(datos,ios::out|ios::in|ios::ate|ios::binary);
                        u=encontrado.second.first;//5
                    }else{
                        encontrado.first=prn;//sapo
                    }
                }
                else{
                    actRegistro.open(datos,ios::out|ios::in|ios::ate|ios::binary);
                }
                ofstream auxFile(aux,ios::app|ios::binary);
                pair<int,char>tmp=encontrado.first.puntero;//-1d
                encontrado.first.puntero.first=auxFile.tellp()/sizeof(Record);//sapo2a
                encontrado.first.puntero.second='a';//
                key.puntero=tmp;//ser-1d
                auxFile.write((char*)&key,sizeof(Record));//ser-1d
                auxFile.close();//

                actRegistro.seekp(u*sizeof(Record));//1
                actRegistro.write((char*)&encontrado.first,sizeof(Record));//ron1a
                actRegistro.close();
            }
            if(sizeAux()>k){
                rebuild();
            }

            return true;
        }
    }
    template<typename T>
    vector<Record> rangeSearch(T begin_key, T end_key){
        rebuild();
        vector<Record>retornar;
        Record inicio,final;inicio.id=begin_key;final.puntero=end_key;
        pair<Record,pair<int,char>>encontrado= search_Record(inicio);
        if(encontrado.first!=inicio)
            throw "LLave invalida";
        else{
            while(inicio!=final){
                ifstream file;
                retornar.push_back(inicio);
                if(inicio.puntero.second=='a'){
                    file.open(aux,ios::binary);
                    file.seekg(inicio.puntero.first*sizeof(Record));
                    file.read((char*)&inicio,sizeof(Record));
                }else{
                    file.open(datos,ios::binary);
                    file.seekg(inicio.puntero.first*sizeof(Record));
                    file.read((char*)&inicio,sizeof(Record));
                }
            }
            retornar.push_back(final);
        }
        return retornar;
    }

    template<typename T>

    bool remove(T key){
        Record tmp;
        tmp.id=key;
        pair<Record,pair<int,char>>encontrado=search_Record(tmp);
        fstream file;
        if(tmp==encontrado.first){
            file.open(datos,ios::out|ios::in|ios::ate|ios::binary);
            file.seekg(encontrado.second.first*sizeof(Record));
            file.read((char*)&tmp,sizeof(Record));
            if(tmp.puntero.second=='d'){
                tmp.puntero.second='w';
            }else{
                tmp.puntero.second='x';
            }
            file.seekg(encontrado.second.first*sizeof(Record));
            file.write((char*)&tmp,sizeof(Record));
        }else{
            if(encontrado.first.puntero.second=='d'){
                throw "Llave incorrecta";
            }else{//buscamos en el aux
                file.open(aux,ios::out|ios::in|ios::ate|ios::binary);
                file.seekg(encontrado.first.puntero.first*sizeof(Record));
                file.read((char*)&tmp,sizeof(Record));
                Record prn=tmp;
                Record antes;
                while(prn!=tmp&&tmp.puntero.second!='d'){
                    antes=tmp;
                    file.seekg(tmp.puntero.first*sizeof(Record));
                    file.read((char*)&tmp,sizeof(Record));
                }
                if(prn==tmp){
                    if(tmp.puntero.second=='d'){
                        tmp.puntero.second='z';
                    }else{
                        tmp.puntero.second='y';
                    }
                    file.seekg(antes.puntero.first*sizeof(Record));
                    file.write((char*)&tmp,sizeof(Record));
                }else{
                    throw "Llave incorrecta";
                }
            }
        }
        file.close();
    }

    pair<Record,pair<int,char>> search_Record(Record key){
        ifstream file(datos,ios::ate|ios::binary);
        if(!file.is_open())throw "No se pudo abrir el archivo";
        else{
            pair<Record,pair<int,char>>retornar;
            int m,l=0;
            int u=(file.tellg()/sizeof(Record))-1; //total de archivos
            Record prenda;
            while(u>=l){
                m=(u+l)/2;
                file.seekg(m*sizeof(Record));
                file.read((char*)&prenda, sizeof(Record));//cesar
                if(key < prenda){
                    u=m-1;
                }else if(key > prenda){
                    l=m+1;
                }else{
                    retornar.first=prenda;
                    retornar.second=prenda.puntero;
                    return retornar;
                }
            }
            if(u!=-1){
                file.seekg(u*sizeof(Record));
                file.read((char*)&prenda, sizeof(Record));
                file.close();
            }
            return pair<Record,pair<int,char>>(prenda, {u, prenda.puntero.second});
        }
    }
    void rebuild(){
        Record prenda;
        prenda.puntero=header;
        ofstream vecWrite(datosVector, ios::out | ios::binary);
        //0d
        while(prenda.puntero.first!=-1||prenda.puntero.second!='d'){//2z
            if(prenda.puntero.second=='a'){
                ifstream file(aux,ios::binary);
                file.seekg(prenda.puntero.first*sizeof(Record));
                file.read((char*)&prenda, sizeof(Record));//
                if(prenda.puntero.second=='d'||prenda.puntero.second=='a')
                    vecWrite.write((char*)&prenda,sizeof(Record));
                file.close();
            }else if(prenda.puntero.second=='d'){
                ifstream file(datos,ios::binary);
                file.seekg(prenda.puntero.first*sizeof(Record));
                file.read((char*)&prenda,sizeof(Record));
                if(prenda.puntero.second=='d'||prenda.puntero.second=='a')
                    vecWrite.write((char*)&prenda,sizeof(Record));
                vecWrite.write((char*)&prenda,sizeof(Record));//benson 0a
                file.close();
            }else{
                if(prenda.puntero.second=='w'){
                    ifstream file(datos,ios::binary);
                    file.seekg(prenda.puntero.first*sizeof(Record));
                    file.read((char*)&prenda, sizeof(Record));
                    if(prenda.puntero.second=='d'||prenda.puntero.second=='a')
                        vecWrite.write((char*)&prenda,sizeof(Record));
                    file.close();
                }
                else if(prenda.puntero.second=='x'){
                    ifstream file(aux,ios::binary);
                    file.seekg(prenda.puntero.first*sizeof(Record));
                    file.read((char*)&prenda, sizeof(Record));
                    if(prenda.puntero.second=='d'||prenda.puntero.second=='a')
                        vecWrite.write((char*)&prenda,sizeof(Record));
                    file.close();
                }
                else if(prenda.puntero.second=='y'){
                    ifstream file(aux,ios::binary);
                    file.seekg(prenda.puntero.first*sizeof(Record));
                    file.read((char*)&prenda, sizeof(Record));
                    if(prenda.puntero.second=='d'||prenda.puntero.second=='a')
                        vecWrite.write((char*)&prenda,sizeof(Record));
                    file.close();
                }else{
                    ifstream file(datos,ios::binary);
                    file.seekg(prenda.puntero.first*sizeof(Record));
                    file.read((char*)&prenda, sizeof(Record));
                    if(prenda.puntero.second=='d'||prenda.puntero.second=='a')
                        vecWrite.write((char*)&prenda,sizeof(Record));
                    file.close();
                }

            }
        }
        vecWrite.close();
        ifstream vecLec(datosVector,  ios::binary);
        vecLec.seekg(0,ios::end);
        int sz=vecLec.tellg()/sizeof(Record);//3

        ofstream file(datos,ios::binary);
        vecLec.seekg(0,ios::beg);

        for(int i=0;i<sz;i++){// 0 1 2
            vecLec.seekg(i* sizeof(Record));
            vecLec.read((char*)&prenda,sizeof(Record));
            if(prenda.puntero.first!=-1)
                prenda.puntero.first=i+1;prenda.puntero.second='d';
            file.write((char*)&prenda,sizeof(Record));//cesar1d ron-1d
        }
        header={0,'d'};//acutalizar header
        vecLec.close();
        file.close();
        std::remove("../Data_from_Structurs/dataAuxSecuencial.dat");
        ofstream limpAux(aux);
        limpAux.close();
    }
    int sizeRecord(){
        return 33;
    }
    size_t size(){
        ifstream file(datos,ios::binary);
        file.seekg(0,ios::end);
        size_t t=file.tellg()/sizeof(Record);
        file.close();
        return t;
    }
    size_t sizeAux(){
        ifstream file(aux,ios::binary);
        file.seekg(0,ios::end);
        size_t t=file.tellg()/sizeof(Record);
        file.close();
        return t;
    }

};
#endif //BASEDATOS2_SQUENTIAL_H
```
En este ejemplo, creamos un archivo binario llamado "miArchivo.dat" y agregamos dos registros (nombre y edad) al archivo utilizando la función agregarRegistro(). Luego, abrimos el archivo para lectura y realizamos una búsqueda de un registro con un nombre específico utilizando la función buscarRegistro(). La función recorre el archivo secuencial y compara el nombre de cada registro con el nombre buscado hasta encontrar un registro con el nombre deseado. Si no se encuentra ningún registro con el nombre buscado, se imprime un mensaje apropiado.

Es importante tener en cuenta que el archivo secuencial no es muy eficiente para realizar búsquedas en grandes conjuntos de datos, ya que se debe leer cada registro desde el principio del archivo hasta encontrarke el registro deseado. Además, la eliminación de registros puede ser complicada, ya que puede dejar "huecos" en el archivo si se eliminan registros intermedios.
Lógica para las funciones:

#### 1. Search
Usamos la busqueda binaria para la búsqueda con las siguientes condiciones
        - 1.1 Las variables m,l inician en 0, luego el u será el tamaño de nuestra data en "datosSecuencial.dat" - 1.
        - 1.2 Iniciamos un bucle while con la condición (u>=l).
        - 1.3 Mientras se ejecute el while, m se calculará a (u+l)/2.
        - 1.4 Nos moveremos en esa posicion m del registro "datosSecuencial.dat" (leyendo un registro) y compararemos con la key que estamos buscando.
        - 1.5 Si key>Data_en_m, u =m-1.
        - 1.6 Si key<Data_en_m, l=m+1.
        - 1.7 Si son iguales retornar registro en la posicion m.
        - 1.8 Si termina el while y el u es diferente a -1. Leemos por ultimavez en la posicion u, para "datosSecuencial.dat" y retornamos ese registro, si no solo retornamos el último registro leido, junto a un pair<int,char> que tendra como primer dato el u, y como segundo dato el caracter del ultimo registro leido.
    
#### 2. Remove
Para el Remove primero ejecutaremos el Rebuild, luego de esto evaluamos:
        - 2.1 Verificamos si la llave existe con la función search(key).
        - 2.2 Si el registro se encuentra en "datosSecuencial.dat" verificamos a quien apunta y lo modificamos con otro caracter ejmp ('w' si apunta a otro dato del mismo archivo y 'x' si tiene un puntero a algún dato en "dataSecAux.dat")
        - 2.3 Si no verificamos si está en alguna parte del "dataSecAux.dat" con un while y moviendonos con los punteros
        - 2.4 Si no está retornamos que es una llave inválida
        - 2.5 Si es que se encuentra realizamos la misma lógica del paso 2.2 ejmp ('y' si apunta a algún dato del mismo archivo y 'z' si apunta a algún dato del archivo "datosSecuencial.dat").
    
#### 3. Rebuild
Para el rebuild primero leemos el header y lo guardamos en un registro temporal, además de que creamos un file temporal que contendrá todos los datos ordenados ("dataVector.dat").
        - 3.1 Iniciamos un while.
        - 3.2 si la condición de que temporal.puntero.first!=-1 y temporal.puntero.second!='d'.
        - 3.3 Verificamos a quien apunta.
        - 3.4 Si tiene un temporal.puntero..second='a' entonces abrimos el "dataSecAux.dat", nos movemos en la posicion del registro con temporal.puntero.first y leemos ese registro, luego aquel registro lo añadimos a "dataVector.dat".
        - 3.5 Si tiene un temporal.puntero..second='d' entonces abrimos el "dataSecuencial.dat", nos movemos en la posicion del registro con temporal.puntero.first y leemos ese registro, luego aquel registro lo añadimos a "dataVector.dat".
        - 3.6 Si no, entonces verificamos si contiene un temporal.puntero.second= conjunto(w,x,y,z), dependiendo de cual tenga abrimos el archivo indicado como en el paso 2.2 y 2.7, luego leemos ese registro gracias al temporal.puntero.first y validamos lo mismo de su puntero.second (para cubrir el caso de si un eliminado está apuntando a un eliminado). Si contiene algún 'a' o 'd' como puntero.second entonces leemos ese regitro y lo escribimos en "dataVector.dat".
        - 3.7 Repetimos el paso 2.2 hasta que se salga y cerrar el archivo "dataVector.dat".
        - 3.8 Luego con los datos ordenados en "dataVector.dat", solo nos queda abrir denuevo el archivo "dataVector.dat" y el archivo "dataSecuencial.dat" e iterar en todos los adtos del "dataVector.dat", si leemos un registro que tiene un puntero.first diferente a -1 entonces reemplazamos ese puntero por i+1 y su puntero.second a 'd'.
        - 3.9 luego escribimos ese registro en "dataSecuencial.dat".
        - 3.10 Limpiamos todos los datos en "DataSecAux.dat" y eliminamos el registro "dataVector.dat".
    
### 2. AVL File:
La estrategia de organización de archivos AVLFile en C++ es una técnica que permite almacenar registros en un archivo de forma ordenada y balanceada mediante la utilización de árboles AVL. Esta técnica es útil cuando se necesitan realizar operaciones de búsqueda, inserción y eliminación eficientes en un archivo de datos.

Un archivo AVLFile consta de una serie de nodos, cada uno de los cuales contiene un registro y un apuntador al siguiente nodo en el archivo. Los nodos se organizan en forma de árbol AVL, en el cual cada nodo tiene una altura que representa la longitud del camino más largo desde ese nodo hasta una hoja.

Cuando se realiza una operación de inserción en un archivo AVLFile, se busca la posición correcta en el árbol AVL para el nuevo registro y se inserta en un nuevo nodo. Luego, se realiza una serie de rotaciones en el árbol AVL para mantener el equilibrio y la altura adecuada.

Por otro lado, cuando se realiza una operación de búsqueda en un archivo AVLFile, se realiza una búsqueda binaria en el árbol AVL para encontrar el nodo que contiene el registro deseado. Esto permite una búsqueda eficiente en tiempo logarítmico en el número de nodos del árbol.

Finalmente, cuando se realiza una operación de eliminación en un archivo AVLFile, se busca el nodo que contiene el registro a eliminar y se realiza una serie de rotaciones para mantener el equilibrio y la altura adecuada en el árbol AVL. Luego, se marca el nodo como eliminado, pero no se elimina físicamente del archivo hasta que se realice una operación de compactación en el archivo.

En resumen, la estrategia de organización de archivos AVLFile en C++ utiliza árboles AVL para mantener un archivo de datos ordenado y balanceado, lo que permite realizar operaciones de búsqueda, inserción y eliminación eficientes en tiempo logarítmico en el número de nodos del árbol.

Para comenzar, se necesita definir una estructura para los nodos del árbol AVL que contendrá un registro y los punteros a sus hijos izquierdo y derecho, así como una variable de equilibrio para mantener el balance del árbol. Por ejemplo:

```cpp

#ifndef AVLARCHIVOS_FINAL_H
#define AVLARCHIVOS_FINAL_H
#include <iostream>
#include <fstream>
#include <functional>
using namespace std;

class AVL
{
public:
    struct RecordAVL{
        char cod[7];
        char prenda[10];
        char genero[1];
        float precio;
        int stock;
        char marca[7];
        RecordAVL(){}
        RecordAVL(string _cod, string _prenda, string _genero, float _precio, int _stock, string _marca){
            for(int i=0;i<7;i++){
                this->cod[i]= _cod[i];
            }
            for(int i=0;i<10;i++){
                this->prenda[i]=_prenda[i];
            }
            for(int i=0;i<1;i++){
                this->genero[i]=_genero[i];
            }
            this->precio=_precio;
            this->stock=_stock;

            for(int i=0;i<7;i++){
                this->marca[i]=_marca[i];
            }
        }
    };
    struct NodeBT {
        RecordAVL data;
        long left;
        long right;
        long height;
        NodeBT() {
            left = right = -1;
        }
        NodeBT(RecordAVL record){   
            left = right = -1;
            this->data = record;
        }
    };
private:
    long root;
    string filename;

    long sizeC(){
        ifstream file(filename, ios::binary);
        file.seekg(0, ios::end);
        long nBytes = file.tellg();
        file.close();
        return nBytes;
    }
    int sizeNode(){
        return 45;
    }

public:
    AVL(string filename){
        this->filename = filename;
        root = -1;
    }
    RecordAVL find(char key[5])
    {
        RecordAVL temp = find(root, key);
        return temp;
    }
    void insert(RecordAVL alumno1) {
        insert(this->root, alumno1,true);
    }

private:
    RecordAVL find(long nodepos, char key[5]){
        ifstream file(filename, ios::binary);
        if (nodepos == -1)
            throw std::out_of_range( "No se encontro" );
        else {
            NodeBT temp;
            file.seekg(nodepos * sizeof(NodeBT));
            file.read((char*)&temp, sizeof(NodeBT));

            if (key < temp.data.cod) {
                file.close();
                return find(temp.left, key);
            }
            else if (key > temp.data.cod) {
                file.close();
                return find(temp.right, key);
            }
            else
                file.close();
                return temp.data;
        }
    }
    void insert(long node, RecordAVL value, bool sit) { // sit == true -> left
        fstream file(filename, std::ios::out|std::ios::in|std::ios::ate);
        if (node == -1) {// sit == false -> rigth
            NodeBT temp(value);
            NodeBT aux = getNode(node).first;
            (sit) ? aux.left = sizeC()/sizeof(NodeBT) : aux.right = sizeC()/sizeof(NodeBT) ;
            file.seekp(0, ios::end);
            file.write((char *) &temp, sizeof(NodeBT));
            file.seekp(node*sizeof(NodeBT));
            file.write((char*)&aux, sizeof(NodeBT));
            file.close();
            return;
        }
        NodeBT temp;
        file.seekg(node * sizeof ( NodeBT));
        file.read((char*)&temp, sizeof(NodeBT));
        if (temp.data.cod > value.cod)
            insert(temp.left, value, true);
        else if (value.cod < temp.data.cod)
            insert(temp.right, value, false);

        updateHeight(temp, node);
        balance(node);
        file.close();
    }

    void updateHeight(NodeBT& node, long pos) {
        int leftHeight = height(node.left);
        int rightHeight = height(node.right);
        node.height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
        ofstream file(filename, std::ios::out|std::ios::in|std::ios::ate);
        file.seekp(pos*sizeof(NodeBT));
        file.write((char*)&node, sizeof(NodeBT));


    }
    int height(long node) {
        return node == -1 ? 0 : getNode(node).first.height;
    }
    void balance(long node) {
        {
            pair<NodeBT, long> temp = getNode(node);
            int balanceFactor = balancingFactor(temp.first);

            if (balanceFactor > 1) {
                pair<NodeBT, long> temp2 = getNode(temp.first.left);
                if (balancingFactor(temp2.first) < 0) {
                    left_rota(temp2.first, temp2.second);
                }
                right_rota(temp.first, temp2.second);
            } else if (balanceFactor < -1) {
                pair<NodeBT, long> temp2 = getNode(temp.first.right);
                if (balancingFactor(temp2.first) > 0) {
                    right_rota(temp2.first, temp2.second);
                }
                left_rota(temp.first, temp2.second);
            }
        }

    }
    pair<NodeBT, long> getNode(long pos){
        NodeBT temp;
        ifstream file(filename, ios::binary);
        file.seekg(pos * sizeof(NodeBT));
        file.read((char*)&temp, sizeof(NodeBT));
        file.close();
        return  pair<NodeBT, long>(temp, pos);

    }
    int balancingFactor(NodeBT node) {
        if (node.left && node.right) {
            return getNode(node.left).first.height - getNode(node.right).first.height;
        } else if (node.left && node.right == -1) {
            return getNode(node.left).first.height;
        } else if (node.left == -1 && node.right) {
            return getNode(-node.right).first.height;
        }
    }
    void left_rota(NodeBT &node, long pos) {
        pair<NodeBT, long> temp = getNode(node.right);
        node.right = temp.first.left;
        temp.first.left = pos;

        pair<NodeBT, long> temp2 = getNode(node.left);

        node = temp.first;
        fstream file(filename,std::ios::out|std::ios::in|std::ios::ate);
        file.seekp(pos * sizeof (NodeBT));
        file.write((char*)&node, sizeof (NodeBT));
        file.seekp( temp2.second * sizeof (NodeBT));
        file.write((char*)&temp2.first, sizeof (NodeBT));
        updateHeight(temp2.first, temp2.second);
        updateHeight(node, pos);
        file.close();

    }

    void right_rota(NodeBT &node, long pos) {
        pair<NodeBT, long> temp = getNode(node.left);
        node.left = temp.first.right;
        temp.first.right = pos;

        pair<NodeBT, long> temp2 = getNode(node.right);
        node = temp.first;

        fstream file(filename,std::ios::out|std::ios::in|std::ios::ate);
        file.seekp(pos * sizeof (NodeBT));
        file.write((char*)&node, sizeof (NodeBT));
        file.seekp( temp2.second * sizeof (NodeBT));
        file.write((char*)&temp2.first, sizeof (NodeBT));

        updateHeight(temp2.first, temp2.second);
        updateHeight(node, pos);
        file.close();
    }
    RecordAVL minValue(long node) {
        ifstream file(filename, ios::binary);
        NodeBT temp = getNode(node).first;
        if (temp.left == -1) {
            file.close();
            return temp.data;
        }
        return minValue(temp.left);
    }

    RecordAVL maxValue(long node) {
        ifstream file(filename, ios::binary);
        NodeBT temp = getNode(node).first;
        if (temp.right == -1) {
            file.close();
            return temp.data;
        }
        file.close();
        return maxValue(temp.right);
    }

    int size(long node) {
        return node == -1 ? 0 : size(getNode(node).first.left) + 1 + size(getNode(node).first.right);
    }
    bool isBalanced(long node) {
        if (node == -1) {
            return true;
        }
        int bf = balancingFactor(getNode(node).first);
        if (bf > 1 || bf < -1) {
            return false;
        }
        return isBalanced(getNode(node).first.left) && isBalanced(getNode(node).first.right);
    }
    void remove(long node, char value[5], long father = 0, bool sit = true) {
        if (node == -1) return;
        if (value < getNode(node).first.data.cod) {
            remove(getNode(node).first.left, value, node);
            balance(node);
        } else if (value > getNode(node).first.data.cod) {
            remove(getNode(node).first.right, value, node);
            balance(node);
        } else if (getNode(node).first.left >-1 && getNode(node).first.right > -1) {
            fstream file(filename, std::ios::out|std::ios::in|std::ios::ate);
            NodeBT temp = getNode(node).first;
            RecordAVL aux = minValue(temp.right);
            temp.data = aux;
            file.seekp(node*sizeof (NodeBT));
            file.write((char *)&temp, sizeof(NodeBT));
            balance(node);
            remove(temp.right, temp.data.cod, node);
        } else {
            if(getNode(node).first.right > -1) {
                fstream file(filename, std::ios::out | std::ios::in | std::ios::ate);
                NodeBT temp = getNode(father).first;
                temp.right = getNode(node).first.right;
                file.seekp(father*sizeof (NodeBT));
                file.write((char *)&temp, sizeof(NodeBT));
            }else if (getNode(node).first.left > -1){
                fstream file(filename, std::ios::out | std::ios::in | std::ios::ate);
                NodeBT temp = getNode(father).first;
                temp.left = getNode(node).first.left;
                file.seekp(father*sizeof (NodeBT));
                file.write((char *)&temp, sizeof(NodeBT));
            }else{
                fstream file(filename, std::ios::out | std::ios::in | std::ios::ate);
                NodeBT temp = getNode(father).first;
                if (node == temp.right){
                    temp.right = -1;
                    file.seekp(father*sizeof (NodeBT));
                    file.write((char *)&temp, sizeof(NodeBT));
                }else if(node == temp.left){
                    temp.left = -1;
                    file.seekp(father*sizeof (NodeBT));
                    file.write((char *)&temp, sizeof(NodeBT));
                }else{
                    temp.right = temp.left = -1;
                    file.seekp(father*sizeof (NodeBT));
                    file.write((char *)&temp, sizeof(NodeBT));
                }
            }

        }

    }
};

#endif //AVLARCHIVOS_FINAL_H

```
#### 1.- la primera estrategia usada en el AVL es que nuestro root siempre sea el 0, esto debido a que jugamos con los punteros y con las posiciones dentro del archivo para lograr rotaciones correctas y un balanceo preciso y con la misma estrategia la altura.
#### 2.- segunda estrategia que hemos usado es un booleano denominado "sit" el cual nos da a conocer en que parte sera insertado el siguiente nodo, ya sea true que significa que va a la izquierda o false que es agregarlo a la derecha y asi saber siempre en que posicion debe ir para que vaya con la secuencia de un avl.
#### 3.- la tercera estrategia usada es la de una funcion que retorna un pair el cual contiene un NodeBt el cual lo leemeos del archivo y su posicion dentro de este para luego de las modificaciones o las actualizaciones puede ser escrito de manera correcta.
#### 4.- La ultima optimizacion o agregado que le dimos fue que escribimos un nodo con atributos record, left, rigth y tamaño, y asi de esta forma poder tener un ingreso de datos más limpio dentro de nuestro AVL.




### 4. Extendible Hashing:
La estrategia de organización de archivos Extendible Hashing en C++ es un método de almacenamiento y búsqueda de datos en archivos que se basa en la utilización de una tabla hash y la división de los datos en bloques de tamaño fijo para facilitar la búsqueda y la actualización de los mismos.

En esta estrategia, se utiliza una tabla hash que contiene punteros a bloques de datos. Cada bloque de datos contiene un número fijo de elementos y está identificado por un número de bloque. Cuando se agrega un nuevo elemento a la tabla hash, se utiliza la función de hash para calcular su clave y se determina el número de bloque correspondiente. Si el bloque está lleno, se divide en dos bloques de la misma capacidad y se actualiza la tabla hash para que los nuevos bloques puedan ser encontrados en función de la clave del elemento.

Aquí hay un ejemplo de implementación de la estrategia de organización de archivos Extendible Hashing en C++:

```cpp

#ifndef AVLARCHIVOS_HASH_H
#define AVLARCHIVOS_HASH_H

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <forward_list>
#include <functional>
using namespace std;

const int maxColision = 7;
const int maxFillFactor = 15;

struct RecordHash{
    char cod[7];
    char prenda[10];
    char genero[1];
    float precio;
    int stock;
    char marca[7];
    RecordHash(){}
    RecordHash(string _cod, string _prenda, string _genero, float _precio, int _stock, string _marca){
        for(int i=0;i<7;i++){
            this->cod[i]= _cod[i];
        }
        for(int i=0;i<10;i++){
            this->prenda[i]=_prenda[i];
        }
        for(int i=0;i<1;i++){
            this->genero[i]=_genero[i];
        }
        this->precio=_precio;
        this->stock=_stock;

        for(int i=0;i<7;i++){
            this->marca[i]=_marca[i];
        }
    }
    void display(){
        cout<<cod<<endl;
        cout<<prenda<<endl;
        cout<<genero<<endl;
        cout<<precio<<endl;
        cout<<stock<<endl;
        cout<<marca<<endl;
    }
    };

class Hash{
    struct Entry{
        RecordHash array[maxColision];
        size_t sig = -1;
        size_t size = 0;
        Entry(){
        }
        Entry(RecordHash _record){
            array[size] = _record;
            size++;
        }
    };

    string filename;
public:
    Hash(string _filename):filename(_filename){
        ofstream file(filename, ios::app | ios::binary);
        for (int i = 0; i < maxFillFactor; ++i) {
            file.seekp(i*sizeof(Entry));
            Entry temp;
;           file.write((char*)&temp, sizeof(Entry));
        }
        file.close();
    }

    void set(RecordHash record) {
        fstream file(filename, std::ios::out|std::ios::in|std::ios::ate);
        size_t hashcode = getHashCode(record.cod);
        int index = hashcode % maxFillFactor;
        Entry temp;
        file.seekg(index*sizeof(Entry));
        file.read((char*)&temp, sizeof (Entry));
        if (search(record.cod)) throw std::out_of_range("El dato ya existe no puede sobreescribir");
        if (temp.size == maxColision){
            set(record, index);
            file.close();
        }

        else{
            temp.array[temp.size] = record;
            temp.size++;
            file.seekp(index*sizeof (Entry));
            file.write((char*)&temp, sizeof(Entry));
            file.close();
        }
        //Completar para el caso cuando el set tiene que actualiza

    }

    vector<RecordHash> get(long key) {
        vector<RecordHash> result;
        ifstream file(filename, ios::binary);
        Entry temp;
        file.seekg(key*sizeof(Entry));
        file.read((char*)&temp, sizeof (Entry));
        if (temp.size == 0) throw std::out_of_range ("Dont elements for key");
        for (int i = 0; i < temp.size; ++i) {
            result.emplace_back(temp.array[i]);
        }
        if (temp.sig > -1) get(temp.sig, result);
        return result;
    }
    void get(long pos, vector<RecordHash> &final){
        ifstream file(filename, ios::binary);
        Entry temp;
        file.seekg(pos*sizeof(Entry));
        file.read((char*)&temp, sizeof (Entry));
        for (int i = 0; i < temp.size; ++i) {
            final.emplace_back(temp.array[i]);
        }
        if (temp.sig > -1) get(temp.sig, final);
    }
    void remove(string key) {
        if (!search(key)) throw std::out_of_range ("Key not found");
        size_t hashcode = getHashCode(key);
        int index = hashcode % maxFillFactor;
        fstream file(filename, std::ios::out|std::ios::in|std::ios::ate);
        Entry temp;
        file.seekg(index * sizeof(Entry));
        file.read((char *) &temp, sizeof(Entry));
        bool sit = false;
        for (int i = 0; i < temp.size; ++i) {
            if (temp.array[i].cod == key){
                sit = true;
                if (temp.size == 1){
                    if (temp.sig == -1)temp.size--;
                    else{
                        Entry aux;
                        file.seekg(temp.sig*sizeof (Entry));
                        file.read((char*)&aux, sizeof(Entry));
                        file.seekp(index*sizeof(Entry));
                        file.write((char*)&aux,sizeof (Entry));
                        file.close();
                    }
                }
                for (int j = i; j < temp.size-1; --j) {
                    temp.array[i] = temp.array[i+1];
                }
                temp.size--;
                file.close();
            }
        }
        if (!sit)remove(temp.sig, key);
    }
    bool search(string key) {
        size_t hashcode = getHashCode(key);
        int index = hashcode % maxFillFactor;
        ifstream file(filename,ios::app | ios::binary);
        Entry temp;
        file.seekg(index*sizeof(Entry));
        file.read((char*)&temp, sizeof (Entry));
        for (auto &it : temp.array) {
            if (it.cod == key) {
                file.close();
                return  true;
            }
        }
        return search(temp.sig,  key);
    }
    bool search(long pos, string key){
        if (pos == -1) return false;
        ifstream file(filename, ios::binary);
        Entry temp;
        file.seekg(pos*sizeof(Entry));
        file.read((char*)&temp, sizeof (Entry));
        for (auto &it : temp.array) {
            if (it.cod == key) {
                file.close();
                return true;
            }
        }
        return search(temp.sig,  key);
    }
    size_t getHashCode(string cod) {
        hash<string> ptr_hash;
        return ptr_hash(cod);
    }
private:
    void remove(long pos, string key){
        if (pos == -1) throw std::out_of_range ("Key dont Fund");
        fstream file(filename, std::ios::out|std::ios::in|std::ios::ate);
        Entry temp;
        file.seekg(pos*sizeof(Entry));
        file.read((char*)&temp,sizeof(Entry));
        bool sit = false;
        for (int i = 0; i < temp.size; ++i) {
            if (temp.array[i].cod == key){
                sit = true;
                if (temp.size == 1){
                    if (temp.sig == -1)temp.size--;
                    else{
                        Entry aux;
                        file.seekg(temp.sig*sizeof (Entry));
                        file.read((char*)&aux, sizeof(Entry));
                        file.seekp(pos*sizeof(Entry));
                        file.write((char*)&aux,sizeof (Entry));
                        file.close();
                    }
                }
                for (int j = i; j < temp.size-1; --j) {
                    temp.array[i] = temp.array[i+1];
                }
                temp.size--;
                file.close();
            }
        }
        if (!sit)remove(temp.sig, key);
    }
    long sizeC(){
        ifstream file(filename, ios::binary);
        file.seekg(0, ios::end);
        long nBytes = file.tellg();
        file.close();
        return nBytes;
    }
    void set(RecordHash record, long father){
        fstream file(filename, std::ios::out|std::ios::in|std::ios::ate);
        Entry temp;
        file.seekg(father*sizeof (Entry));
        file.read((char*)&temp,sizeof(Entry));
        long last = sizeC() / sizeof(Entry);
        Entry aux(record);
        aux.sig = last;
        file.seekp(father*sizeof(Entry));
        file.write((char*)&aux,sizeof (Entry));
        file.seekp(0,ios::end);
        file.write((char*)&temp, sizeof(Entry));
    }


};

#endif //AVLARCHIVOS_HASH_H


```

En este ejemplo, la estructura `Block` representa un bloque de datos que contiene un vector de enteros. La clase `HashTable` representa la tabla hash y contiene un vector de bloques y un vector de enteros que representa el directorio. El tamaño de bloque se define como una constante `BLOCK_SIZE`. La función `hash` calcula el índice de la tabla hash en función de la clave del elemento.

La función `insert` se utiliza para agregar un nuevo elemento a la tabla hash. Si el bloque correspondiente está lleno, se utiliza la función `split` para dividir el bloque y agregar el nuevo elemento en el bloque correspondiente.

La función `find` se utiliza para buscar un elemento en la tabla hash. Si el elemento se encuentra en el bloque correspondiente, la función devuelve `true`. En caso contrario, devuelve `false`.

La función `print` se utiliza para imprimir el contenido de la tabla hash en la consola.

En el `main`, se crea una nueva instancia de la tabla hash, se agregan algunos elementos y se imprime el contenido de la tabla hash. Luego se realizan algunas búsquedas de elementos en la tabla hash.

#### 1.- Una optimizacion que le dimos a nuestro hash es sobre los set, que no aceptan datos repetidos, y que nuestro fillfactor puede ser variable segun como lo consideremos mejor nosotros, de esta manera logramos un hash variable.
#### 2.- En nuestro caso usamos arrays para almacenar la data de los records dentro de la misma key. Por esta razon nuestro record va en una clase Entry la cual es la que va escrita dentro del archivo, de esta fomra logramos una escritura simple y mas organizada, de la misma forma vemos que podemos optimizar usando templates o modificando el codigo para permitir indexaciones diferentes a la que ya tenemos.

## Analisis de inserccion de datos
![GUI de referencia](/imagenes/analisis.jpg)
## Consideraciones:

Mostramos el buqued 0 con 100000 de datos en el sistema
![GUI de referencia](/imagenes/prueba1.jpg)

Mostramos el buqued 1 con 100000 de datos en el sistema
![GUI de referencia](/imagenes/prueba2.jpg)

Se debe escoger una de las siguientes combinaciones de técnicas de organización de archivos:
- Sequential File o AVL File o ISAM-Sparse Index
- Extendible Hash o B+Tree

Además, se deberán implementar las siguientes operaciones:
1. Búsqueda específica que puede retornar más de un elemento que coincide con la clave: `vector<Registro> search(T key)`
2. Búsqueda por rango que retorna todos los registros que calzan entre dos claves de búsqueda: `vector<Registro> rangeSearch(T begin_key, T end_key)`
3. Agregar un registro al archivo respetando la técnica de organización: `bool add(Registro registro)`
4. Proponer un algoritmo de eliminación para cada técnica: `bool remove(T key)`

## Diseño de GUI
La GUI fue diseñada en Qt Creator, este es un entorno de desarrollo integrado multiplataforma que puede usar para modificar aplicaciones existentes o para crear otras nuevas.

![GUI de referencia](/imagenes/gui.jpg)
