#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Definimos una estructura para representar un token, que puede ser una palabra clave, un identificador, un número o un operador.
struct Token {
    enum Type { Keyword, Identifier, Number, Operator } type;
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
            size_t start = i + 1;
            size_t end = query.find('"', start);
            if (end == std::string::npos) {
                end = query.size();
            }
            std::string value = query.substr(start, end - start);
            tokens.push_back(Token(Token::Number, value));
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
            if (value == "SELECT" || value == "INSERT" || value == "UPDATE" || value == "DELETE" || value == "FROM" || value == "WHERE" || value == "ORDER" || value == "BY" || value == "AND" || value == "OR"){
                tokens.push_back(Token(Token::Identifier, value));
            }
            else{
                tokens.push_back(Token(Token::Keyword, value));
            }
        }
    }

    return tokens;
}

void Consulta(vector<Token> Tokens){
    for (const auto& token : Tokens) {

    }
}

