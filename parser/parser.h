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
        else if (c 