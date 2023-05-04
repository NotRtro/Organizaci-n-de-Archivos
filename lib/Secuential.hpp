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
