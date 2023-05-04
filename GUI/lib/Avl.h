//
// Created by rvilc on 17/04/2023.
//

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
