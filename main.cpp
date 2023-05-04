#include <iostream>
#include "lib/Avl.hpp"
#include "lib/Hash.hpp"
#include "lib/Secuential.hpp"

using namespace std;

int main(){
    Hash temp("Data_from_Structurs/dataHash.dat");
    AVL aux("Data_from_Structurs/dataAvl.dat");

    // casos de prueb
    RecordHash temp2("aaaaaaa", "bbbbbbbbbb", "f", 100.1, 6, "nikebaa");
    RecordHash temp3("bbbbbbb", "bbbbbbbbbb", "f", 100.1, 6, "nikebaa");
    RecordHash temp4("ccccccc", "bbbbbbbbbb", "f", 100.1, 6, "nikebaa");
    RecordHash temp5("ddddddd", "bbbbbbbbbb", "f", 100.1, 6, "nikebaa");
    //temp.set(temp2);
//    vector<RecordHash> aux1, aux2, aux3, aux4, aux5, aux6, aux7, aux8, aux9, aux10,aux11, aux12, aux13, aux14, aux15;
//    vector<vector<RecordHash>> temp4{aux1,aux2,aux3,aux4,aux5,aux6,aux7,aux8,aux9,aux10,aux11,aux11,aux12,aux13,aux14,aux15};
//    for (int i = 0; i < temp4.size() ; ++i) {
//        temp4[i] = temp.get(i);
//    }
//    for (const auto & i : temp4) {
//        for (int j = 0; j < i.size(); ++j) {
//            cout << i[j].marca <<endl;
//        }

//    }
    AVL::RecordAVL prueba53("aaaaaaa", "bbbbbbbbbb", "f", 100, 6, "nikeb");
    aux.insert(prueba53);
    AVL::RecordAVL prueba54("ccccccc", "xxxxxxxxxx", "f", 100, 6, "adida");
    aux.insert(prueba54);
    AVL::RecordAVL prueba55("ddddddd", "zzzzzzzzzz", "m", 1020, 21, "pumas");
    aux.insert(prueba55);
    aux.insert(prueba55);
    aux.insert(prueba55);
    aux.insert(prueba55);
    aux.insert(prueba55);




    vector<AVL::NodeBT> pruebaaaa = aux.rangeSearch("aaaaaaa","zzzzzzz");
    cout<<pruebaaaa.size()<<endl;
    for (auto temp: pruebaaaa) {
        cout << temp.data.cod <<endl;
        cout<< temp.right<<endl;
        cout<<temp.left<<endl;
    }
    //aux.pretyprint();
//    vector<RecordHash> prueba1 = temp.get(0);
//    for (auto & i : prueba1) {
//        i.display();
//        cout<<endl;
//    }
    //temp.set(temp2);
    //temp.set(temp3);
    //temp.set(temp4);
    //temp.set(temp5);
}