#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "../lib/Hash.hpp"

using namespace std;

// Definimos una estructura para representar un token, que puede ser una palabra clave, un identificador, un número o un operador.
struct Token {
    enum Type { Keyword, Identifier, Number, Operator, Value } type;
    std::string value;

    Token(Type type, std::string value) : type(type), value(value) {}
};

// Función para separar una cadena en tokens.
std::vector<Token> tokenize(std::string query) {
    std::vector<Token> tokens;
    std::string current_token;

    // Iteramos sobre cada caracter de la cadena.
    for (size_t i = 0; i < query.size(); i++) {
        char c = query[i];

        // Ignoramos los espacios en blanco.
        if (c == ' ') {
        }

        // Si encontramos un paréntesis, lo añadimos como un token separado.
        else if (c == '(' || c == ')') {
            std::string value;
            value.push_back(c);
            tokens.push_back(Token(Token::Operator, value));
        }

        // Si encontramos una coma, la añadimos como un token separado.
        else if (c == ',') {
            std::string value;
            value.push_back(c);
            tokens.push_back(Token(Token::Operator, value));
        }

        // Si encontramos un operador, lo añadimos como un token separado.
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '>' || c == '<') {
            std::string value;
            value.push_back(c);
            tokens.push_back(Token(Token::Operator, value));
        }

        // Si encontramos comillas dobles, leemos una cadena hasta la siguiente comilla doble.
        else if (c == '"') {
            i++;
            int cont = 0;
            std::string value = "";
            while(query[i] != '"'){
                if(i >= query.size()){
                    break;
                }
                cont++;
                value.push_back(query[i]);
                i++;
            }
            i--;
            tokens.push_back(Token(Token::Value, value));
        }

        // Si encontramos un dígito, leemos un número entero o decimal.
        else if (std::isdigit(c)) {
            int cont = 0;
            std::string value = "";
            while(query[i] != ' ' && query[i] != '(' && query[i] != ')' && query[i] != ',' && query[i] != '+' && query[i] != '-' && query[i] != '*' && query[i] != '/' && query[i] != '%' && query[i] != '>' && query[i] != '<' && query[i] != '=' && query[i] != '!'){
                if(i >= query.size()){
                    break;
                }
                cont++;
                value.push_back(query[i]);
                i++;
            }
            i--;
            tokens.push_back(Token(Token::Number, value));
        }

        else {
            // Si no es ninguna de las anteriores, leemos una palabra hasta el siguiente espacio en blanco o símbolo.
            int cont = 0;
            std::string value = "";
            while(query[i] != ' ' && query[i] != '(' && query[i] != ')' && query[i] != ',' && query[i] != '+' && query[i] != '-' && query[i] != '*' && query[i] != '/' && query[i] != '%' && query[i] != '>' && query[i] != '<' && query[i] != '=' && query[i] != '!'){
                if(i >= query.size()){
                    break;
                }
                cont++;
                value.push_back(query[i]);
                i++;
            }
            i--;
            if (value == "SELECT" || value == "INSERT" || value == "UPDATE" || value == "DELETE" || value == "TO" || value == "FROM" || value == "WHERE" || value == "ORDER" || value == "BY" || value == "AND" || value == "OR"){
                tokens.push_back(Token(Token::Identifier, value));
            }
            else{
                tokens.push_back(Token(Token::Keyword, value));
            }
        }
    }

    return tokens;
}

/*void SELECT(vector<Token> tokens, Hash temp){
    if(tokens[1].value == '*'){
        temp.get();
    }
}*/

void INSERT(vector<Token> tokens, Hash temp){
    if(tokens[3].value != "Tienda"){
        return cout<<"No admitido"<<endl;
    }
    vector<string> valores;
    for (int j = 5; j < tokens.size(); j++) {
        if (tokens[j].type == Token::Keyword){
            valores.push_back(tokens[j]);
        }
    }
    temp.set(valores[0], valores[1], valores[2], stoi(valores[3]), stoi(valores[4]), valores[5]);
    cout<<"se hizo la insercion"<<endl;
};

/*void UPDATE(vector<Token> tokens, Hash temp){

}*/

void DELETE(vector<Token> tokens, Hash temp){
    if (tokens[2].value != "Tienda"){
        cout<<"No admitido"<<endl;
        return;
    }
    string key_delete = "";
    for (int i = 0; i < tokens.size(); ++i) {
        if (tokens[i].type == Token::Value){
            key_delete = tokens[i];
        }
    }
    temp.remove(key_delete);
    cout<<"Eliminado"<<endl;
}

void Consulta(vector<Token> Tokens){
    Hash temp("Data_from_Structurs/dataHash.dat");
    /*if(Tokens[0].value == "SELECT"){
        return SELECT(Tokens,temp);
    }*/
    else if(Tokens[0].value == "INSERT"){
        return INSERT(Tokens,temp);
    }
    else if(Tokens[0].value == "UPDATE"){
        return UPDATE(Tokens,temp);
    }
    else if(Tokens[0].value == "DELETE"){
        return DELETE(Tokens,temp);
    }
    else{
        cout<<"no es una consulta admitida"
    }
}
