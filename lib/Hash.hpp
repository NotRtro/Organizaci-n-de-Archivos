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


class Hash{
    struct Entry{
        struct Record{
            char cod[7];
            char prendra[15];
            char genero[1];
            int precio;
            int stock;
            char marca[5];
        };
        Record array[maxColision];
        size_t sig = -1;
        size_t size = 0;
        Entry(){}
        Entry(Record _record){
            array[size] = _record;
            size++;
        }
    };

    string filename;
public:
    Hash(string _filename):filename(_filename){}

    void set(Entry::Record record) {
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

    Entry::Record &get(string key) {
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
    Entry::Record &get(long pos, string key){
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
    void set(Entry::Record record, long father){
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
