//
// Created by HP on 23/04/2023.
//

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
    //    string datos="../Data_from_Structus/dataSecuencial.dat",aux="../Data_from_Structus/dataSecAux.dat",datosVector="../Data_from_Structus/dataAuxSecuencial.dat";
    string datos="../dataSecuencial.dat",aux="../dataSecAux.dat",datosVector="../dataAuxSecuencial.dat";
    pair<int,char> header;
    long n = 1000;
    long k = log2(n);
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

    bool add(Record key) {
        fstream file;
        if(header.first==-1){
            file.open(datos,ios::out|ios::binary);
            key.puntero=header;
            header.first=0;
            header.second='d';
            file.write((char*)&key, sizeof(Record));
            file.close();
            return true;
        }
        else if(size()<n){
            pair<Record,pair<int,char>>encontrado=search_Record(key);
            if(encontrado.first>key){
                cout<<"llave menor";
                file.open(aux,ios::app|ios::binary);
                pair<int,char>temp=header;
                header.first=(file.tellp()/sizeof(Record));
                header.second='a';
                key.puntero=temp;
                file.write((char*)&key, sizeof(Record));
            }else{
                cout<<"llave mayor";
                file.open(aux,ios::app|ios::binary);
                pair<int,char>temp=encontrado.first.puntero;
                encontrado.first.puntero.first=(file.tellp()/sizeof(Record));
                encontrado.first.puntero.second='a';
                key.puntero=temp;
                file.write((char*)&key,sizeof(Record));

                fstream actRegistro(datos,std::ios::out|std::ios::in|std::ios::ate);
                actRegistro.seekg(encontrado.second.first*sizeof(Record));
                actRegistro.write((char*)&encontrado.first,sizeof(Record));
                actRegistro.close();
            }
            file.close();
            rebuild();
            return true;
        }else{
            file.close();
            return true;
        }
    }
    template<typename T>
    vector<Record> rangeSearch(T begin_key, T end_key){
        vector<Record>retornar;
    }

    //    template<typename T>
    //    bool remove(T key);

    pair<Record,pair<int,char>> search_Record(Record key){
        ifstream file(datos,ios::ate|ios::binary);
        if(!file.is_open())throw "No se pudo abrir el archivo";
        else{
            pair<Record,pair<int,char>>retornar;
            int m,l=0;
            int u=(file.tellg()/sizeof(Record))-1; //total de archivos
            Record alumno;
            while(u>=l){
                m=(u+l)/2;
                file.seekg(m*sizeof(Record));
                file.read((char*)&alumno, sizeof(Record));
                if(key<alumno){
                    u=m-1;
                }else if(key>alumno){
                    l=m+1;
                }else{
                    retornar.first=alumno;
                    retornar.second=alumno.puntero;
                    return retornar;
                }
            }
            return pair<Record,pair<int,char>>(alumno,{u,alumno.puntero.second});
        }
    }
    void rebuild(){
        Record prenda;
        prenda.puntero=header;
        ofstream vecWrite(datosVector, ios::out | ios::binary);
        while(prenda.puntero.first!=-1||prenda.puntero.second!='d'){
            if(prenda.puntero.second=='a'){
                ifstream file(aux,ios::binary);
                file.seekg(prenda.puntero.first*sizeof(Record));
                file.read((char*)&prenda, sizeof(Record));
                vecWrite.write((char*)&prenda,sizeof(Record));
                file.close();
            }else{
                ifstream file(datos,ios::binary);
                file.seekg(prenda.puntero.first*sizeof(Record));
                file.read((char*)&prenda,sizeof(Record));
                vecWrite.write((char*)&prenda,sizeof(Record));
                file.close();
            }
        }
        vecWrite.close();
        ifstream vecLec(datosVector,  ios::binary);
        vecLec.seekg(0,ios::end);
        ofstream file(datos,ios::binary);
        int sz=vecLec.tellg()/sizeof(Record);
        vecLec.seekg(0,ios::beg);
        for(int i=0;i<sz;i++){
            vecLec.seekg(i* sizeof(Record));
            vecLec.read((char*)&prenda,sizeof(Record));
            if(prenda.puntero.first!=-1)
                prenda.puntero.first=i+1;prenda.puntero.second='d';
            file.write((char*)&prenda,sizeof(Record));
        }
        header={0,'d'};//acutalizar header
        vecLec.close();
        file.close();
        //        std::remove("../Data_from_Structus/dataAuxSecuencial.dat");
        //        std::remove("../dataAuxSecuencial.dat");
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

};
#endif //BASEDATOS2_SQUENTIAL_H
