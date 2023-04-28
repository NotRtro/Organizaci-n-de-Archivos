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
#include <functional>
using namespace std;

const int maxColision = 7;
const int maxFillFactor = 15;

struct RecordHash{
    char cod[7];
    char prenda[15];
    char genero;
    int precio;
    int stock;
    char marca[5];
    RecordHash(){}
    RecordHash(char* _cod, char* _prenda, char _genero, int _precio, int _stock, char* _marca){
        copy_n(_cod, 7, cod);
        copy_n(_prenda, 15, prenda);
        genero = _genero;
        precio  = _precio;
        stock = _stock;
        copy_n(_marca, 5, marca);
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
        RecordHash array[maxColision]{};
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
        ofstream file(filename, ios::binary);
        for (int i = 0; i < maxFillFactor; ++i) {
            file.seekp(i*sizeof(Entry));
            Entry temp;
;           file.write((char*)&temp, sizeof(Entry));
        }
        file.close();
    }

    void set(RecordHash record) {
        fstream file(filename, ios::in | ios::out | ios::binary);
        size_t hashcode = getHashCode(record.cod);
        int index = hashcode % maxFillFactor;
        Entry temp;
        file.seekg(index*sizeof(Entry)+index);
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
            file.write((char*)&temp, sizeof (Entry));
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
        int index = hashcode % maxFillFactor;
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
    void set(RecordHash record, long father){
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
