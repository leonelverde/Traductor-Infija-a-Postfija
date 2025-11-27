#include <iostream>
#include <stack>
#include <cctype>
using namespace std;

// Precedencia de operadores
int precedence(char op) {
    switch(op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
    }
    return 0; // no es operador
}


// Verifica si es uno de los operadores válidos
bool isOperator(char c) {
    return c=='+' || c=='-' || c=='*' || c=='/';
}

// Implementación del algoritmo Shunting Yard
string shuntingYard(const string& expr) {
    stack<char> opStack;
    string output;

    for (size_t i = 0; i < expr.size(); i++) {
        char token = expr[i];

        if (token == ' ')
            continue;

        // Detectar números (enteros de varios dígitos)
        if (isdigit(token)) {
            while (i < expr.size() && isdigit(expr[i])) {
                output += expr[i];
                i++;
            }
            output += ' ';
            i--;
        }

        // Detectar operadores
        else if (isOperator(token)) {

            while (!opStack.empty() && isOperator(opStack.top()) &&
                   precedence(token) <= precedence(opStack.top())) {

                output += opStack.top();
                output += ' ';
                opStack.pop();
            }

            opStack.push(token);
        }

        // Paréntesis izquierdo
        else if (token == '(') {
            opStack.push(token);
        }

        // Paréntesis derecho
        else if (token == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                output += opStack.top();
                output += ' ';
                opStack.pop();
            }
            opStack.pop(); // eliminar '('
        }
    }

    // Vaciar pila restante
    while (!opStack.empty()) {
        output += opStack.top();
        output += ' ';
        opStack.pop();
    }

    return output;
}

int main() {
    string expr;
    cout << "Ingresa una expresion en notacion infija: ";
    getline(cin, expr);

    string result = shuntingYard(expr);
    cout << "Notacion postfija: " << result << endl;

    return 0;
} 
