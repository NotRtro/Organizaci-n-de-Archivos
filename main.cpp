#include <iostream>
#include "lib/Avl.hpp"
#include "lib/Hash.hpp"
#include "lib/Secuential.hpp"

using namespace std;

int main(){
    Hash temp("Data_from_Structurs/dataHash.dat");
    AVL aux("Data_from_Structurs/dataAvl.dat");

    // casos de prueba
    Record temp2("aea", "polovoersize", 'f', 100, 6,"nike");
    temp.set(temp2);
}