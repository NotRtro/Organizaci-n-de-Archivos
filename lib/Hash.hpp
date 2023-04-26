//
// Created by rvilc on 21/04/2023.
//

#ifndef AVLARCHIVOS_HASH_H
#define AVLARCHIVOS_HASH_H

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <forward_list>
using namespace std;

const int maxColision = 7;
const int maxFillFactor = 15;

struct Record{
    char cod[7];
    char prendra[15];
    char genero;
    int precio;
    int stock;
    char marca[5];
    Record(){}
    Record(char* _cod, char* _prenda, char _genero, int _precio, int _stock, char* _marca){
        cod[7] = _cod[7];
        prendra[15] = _prenda[15];
        genero = _genero;
        precio = _precio;
        stock = _stock;
        marca[5] = _marca[5];
    }
};

class Hash{
    struct Entry{
        Record array[maxColision]{};
        size_t sig = -1;
        size_t size = 0;
        Entry(){
        }
        Entry(Record _record){
            array[size] = _record;
            size++;
        }
    };

    string filename;
public:
    Hash(string _filename):filename(_filename){}

    void set(Record record) {
        fstream file(filename, ios::in | ios::out | ios::binary);
        size_t hashcode = getHashCode(record.cod);
        int index = hashcode % maxColision;
        Entry temp;
        file.seekg(index*sizeof(Entry));
        file.read((char*)&temp, sizeof (Entry));
        if (get(record.cod).cod == record.cod) throw std::out_of_range("El dato ya existe no puede sobreescribir");
        if (temp.size == maxColision){
            set(record, index);
            file.close();
        }

        else{
            temp.array[temp.size] = record;
            temp.size++;
            file.seekp(index*sizeof (Entry));
            file.write((char*)&temp, sizeof (Entry));
            file.close();
        }
        //Completar para el caso cuando el set tiene que actualiza

    }

    Record &get(string key) {
        size_t hashcode = getHashCode(key);
        int index = hashcode % maxColision;
        ifstream file(filename, ios::binary);
        Entry temp;
        file.seekg(index*sizeof(Entry));
        file.read((char*)&temp, sizeof (Entry));
        for (auto &it : temp.array) {
            if (it.cod == key) {
                file.close();
                return  it;
            }
        }
        return get(temp.sig,  key);
    }
    Record &get(long pos, string key){
        if (pos == -1)throw std::out_of_range("Key not found");
        ifstream file(filename, ios::binary);
        Entry temp;
        file.seekg(pos*sizeof(Entry));
        file.read((char*)&temp, sizeof (Entry));
        for (auto &it : temp.array) {
            if (it.cod == key) {
                file.close();
                return  it;
            }
        }
        return get(temp.sig,  key);
    }
    void remove(string key) {
        if (!search(key)) throw std::out_of_range ("Key not found");
        size_t hashcode = getHashCode(key);
        int index = hashcode % maxColision;
        fstream file(filename, ios::in | ios::out | ios::binary);
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
        int index = hashcode % maxColision;
        ifstream file(filename, ios::binary);
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
private:
    void remove(long pos, string key){
        if (pos == -1) throw std::out_of_range ("Key dont Fund");
        fstream file(filename, ios::in | ios::out | ios::binary);
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
    void set(Record record, long father){
        fstream file(filename, ios::in | ios::out | ios::binary);
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

    size_t getHashCode(string cod) {
        hash<string> ptr_hash;
        return ptr_hash(cod);
    }
};

#endif //AVLARCHIVOS_HASH_H
