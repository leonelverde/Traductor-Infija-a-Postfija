#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <cctype>
#include <locale>
#include <string>
#include <iomanip>
using namespace std;

// Precedencia de operadores
int precedence(char op);
// Verifica si uno de los operadores son válidos
bool isOperator(char c);
// Implementación del algoritmo con registro de pasos
string shuntingYard(const string &expr, ofstream &outFile);
// Función auxiliar para visualizar la pila
string visualizeStack(stack<char> s);
// Función auxiliar para visualizar la cola
string visualizeQueue(queue<string> q);

int main() {
    setlocale(LC_ALL, "");

    string inputFileName, outputFileName;
    string expr;

    // Pedir nombres de archivos
    cout << "Ingrese el nombre del archivo de entrada (se creara si no existe): ";
    getline(cin, inputFileName);

    cout << "Ingrese el nombre del archivo de salida: ";
    getline(cin, outputFileName);

    // ==== CREAR AUTOMÁTICAMENTE EL ARCHIVO DE ENTRADA ====
    ofstream createInput(inputFileName);  // Crea el archivo (o lo reemplaza)
    if (!createInput.is_open()) {
        cerr << "Error: No se pudo crear el archivo de entrada '" << inputFileName << "'" << endl;
        return 1;
    }

    // Pedir la expresión infija por consola
    cout << "Ingrese la expresion infija: ";
    getline(cin, expr);

    if (expr.empty()) {
        cerr << "Error: La expresion ingresada está vacía." << endl;
        return 1;
    }

    // Guardar la expresión infija dentro del archivo de entrada
    createInput << expr << endl;
    createInput.close();

    // ======= LEER EL ARCHIVO DE ENTRADA YA CREADO =======
    ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de entrada." << endl;
        return 1;
    }

    getline(inputFile, expr);
    inputFile.close();

    // Abrir archivo de salida (se crea si no existe)
    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cerr << "Error: No se pudo crear el archivo de salida '" << outputFileName << "'" << endl;
        return 1;
    }

    // Escribir encabezado
    outputFile<<"----------------------------------------"<<endl;
    outputFile<<"ALGORITMO SHUNTING YARD"<<endl;
    outputFile<<"SIMULACION DE AUTOMATA DE PILA"<<endl;
    outputFile<<"----------------------------------------"<<endl<<endl;
    outputFile<<"Expresion infija de entrada: "<<expr<<endl;
    outputFile<<"----------------------------------------"<<endl<<endl;

    // Ejecutar algoritmo
    string result = shuntingYard(expr, outputFile);

    // Resultado final
    outputFile<<endl<<"----------------------------------------"<<endl;
    outputFile<<"RESULTADO FINAL"<<endl;
    outputFile<<"----------------------------------------"<<endl;
    outputFile<<"Notacion postfija: "<<result<<endl;

    outputFile.close();

    cout << "\nProceso completado exitosamente." << endl;
    cout << "Revise el archivo '" << outputFileName << "' para ver los resultados." << endl;

    return 0;
}

int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
    }
    return 0;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

string visualizeStack(stack<char> s) {
    if (s.empty()) return "[ VACÍA ]";

    string result = "[ ";
    stack<char> temp;

    while (!s.empty()) {
        temp.push(s.top());
        s.pop();
    }

    while (!temp.empty()) {
        result += temp.top();
        temp.pop();
        if (!temp.empty()) result += ", ";
    }

    result += " ] ← tope";
    return result;
}

string visualizeQueue(queue<string> q) {
    if (q.empty()) return "[ VACÍA ]";

    string result = "[ ";
    while (!q.empty()) {
        result += q.front();
        q.pop();
        if (!q.empty()) result += ", ";
    }
    result += " ]";
    return result;
}

string shuntingYard(const string &expr, ofstream &outFile) {
    stack<char> opStack;
    queue<string> outputQueue;
    string output;
    int stepNumber = 0;
    string currentState = "q0";

    outFile<<"TRANSICIONES DEL AUTÓMATA DE PILA:"<<endl;
    outFile<<"-------------------------------------------------------------------------------------------------"<<endl;
    outFile<<left<<setw(8)<<"PASO"
            <<setw(25)<<"CONFIG. INICIAL"
            <<setw(30)<<"TRANSICIÓN APLICADA"
            <<setw(25)<<"CONFIG. FINAL"
            <<"PILA"<<endl;
    outFile<<"-------------------------------------------------------------------------------------------------"<<endl;

    // Estado inicial
    outFile<<left<<setw(8)<<stepNumber
            <<setw(25)<<"(q0, " + expr + ", Z0)"
            <<setw(30)<<"Inicial"
            <<setw(25)<<""
            <<"Z0"<<endl;
    outFile<<"-------------------------------------------------------------------------------------------------"<<endl;

    string remainingInput = expr;

    for (size_t i = 0; i < expr.size(); i++) {
        char token = expr[i];

        if (token == ' ') {
            remainingInput = remainingInput.substr(1);
            continue;
        }

        stepNumber++;

        string configInicial, transition, configFinal, pilaViz;
        string prevState = currentState; // Guardar estado previo

        // Detectar números
        if (isdigit(token)) {
            string number;
            while (i < expr.size() && isdigit(expr[i])) {
                number += expr[i];
                i++;
            }
            i--;

            string topStack = opStack.empty() ? "Z0" : string(1, opStack.top());
            configInicial = "(" + currentState + ", " + remainingInput + ", " + topStack + ")";

            outputQueue.push(number);

            // Enviar a salida cambia el estado a q1
            transition = "δ(" + currentState + ", " + number + ", " + topStack + ") → (q1, " + topStack + ")";
            currentState = "q1";

            remainingInput = remainingInput.substr(number.length());
            configFinal = "(q1, " + remainingInput + ", " + topStack + ")";

            pilaViz = opStack.empty() ? "Z0" : visualizeStack(opStack);

            outFile<<left<<setw(8)<<stepNumber
                    <<setw(25)<<configInicial
                    <<setw(30)<<transition
                    <<setw(25)<<configFinal
                    <<pilaViz<<endl;
        }
        // Operadores
        else if (isOperator(token)) {
            // Desapilar operadores de mayor o igual precedencia
            while (!opStack.empty() && isOperator(opStack.top()) &&
                   precedence(token) <= precedence(opStack.top())) {

                string topOp(1, opStack.top());
                configInicial = "(" + currentState + ", " + remainingInput + ", " + topOp + ")";

                outputQueue.push(topOp);
                opStack.pop();

                // Desapilar y enviar a salida permanece en q1
                transition = "δ(" + currentState + ", ε, " + topOp + ") → (q1, ";
                currentState = "q1";

                string newTop = opStack.empty() ? "Z0" : string(1, opStack.top());
                transition += newTop + ")";
                configFinal = "(q1, " + remainingInput + ", " + newTop + ")";

                pilaViz = opStack.empty() ? "Z0" : visualizeStack(opStack);

                outFile<<left<<setw(8)<<stepNumber++
                        <<setw(25)<<configInicial
                        <<setw(30)<<transition
                        <<setw(25)<<configFinal
                        <<pilaViz<<endl;
            }

            // Apilar el nuevo operador (transición de q1 a q0 para lectura)
            string topStack = opStack.empty() ? "Z0" : string(1, opStack.top());
            configInicial = "(" + currentState + ", " + remainingInput + ", " + topStack + ")";

            opStack.push(token);

            transition = "δ(" + currentState + ", " + string(1, token) + ", " + topStack + ") → (q0, " + string(1, token) + topStack + ")";
            currentState = "q0";

            remainingInput = remainingInput.substr(1);
            configFinal = "(q0, " + remainingInput + ", " + string(1, token) + ")";

            pilaViz = visualizeStack(opStack);

            outFile<<left<<setw(8)<<stepNumber
                    <<setw(25)<<configInicial
                    <<setw(30)<<transition
                    <<setw(25)<<configFinal
                    <<pilaViz<<endl;
        }
        // Paréntesis izquierdo
        else if (token == '(') {
            string topStack = opStack.empty() ? "Z0" : string(1, opStack.top());
            configInicial = "(" + currentState + ", " + remainingInput + ", " + topStack + ")";

            opStack.push(token);

            // Apilar paréntesis mantiene o cambia a q0
            transition = "δ(" + currentState + ", (, " + topStack + ") → (q0, (" + topStack + ")";
            currentState = "q0";

            remainingInput = remainingInput.substr(1);
            configFinal = "(q0, " + remainingInput + ", ()";

            pilaViz = visualizeStack(opStack);

            outFile<<left<<setw(8)<<stepNumber
                    <<setw(25)<<configInicial
                    <<setw(30)<<transition
                    <<setw(25)<<configFinal
                    <<pilaViz<<endl;
        }
        // Paréntesis derecho
        else if (token == ')') {
            // Desapilar hasta encontrar '('
            while (!opStack.empty() && opStack.top() != '(') {
                string topOp(1, opStack.top());
                configInicial = "(" + currentState + ", " + remainingInput + ", " + topOp + ")";

                outputQueue.push(topOp);
                opStack.pop();

                // Desapilar y enviar a salida es q1
                transition = "δ(" + currentState + ", ε, " + topOp + ") → (q1, ";
                currentState = "q1";

                string newTop = opStack.empty() ? "Z0" : string(1, opStack.top());
                transition += newTop + ")";
                configFinal = "(q1, " + remainingInput + ", " + newTop + ")";

                pilaViz = opStack.empty() ? "Z0" : visualizeStack(opStack);

                outFile<<left<<setw(8)<<stepNumber++
                        <<setw(25)<<configInicial
                        <<setw(30)<<transition
                        <<setw(25)<<configFinal
                        <<pilaViz<<endl;
            }

            // Eliminar el '(' (transición de q1 a q0)
            if (!opStack.empty()) {
                configInicial = "(" + currentState + ", " + remainingInput + ", ()";
                opStack.pop();

                string newTop = opStack.empty() ? "Z0" : string(1, opStack.top());
                transition = "δ(" + currentState + ", ), () → (q0, " + newTop + ")";
                currentState = "q0";

                remainingInput = remainingInput.substr(1);
                configFinal = "(q0, " + remainingInput + ", " + newTop + ")";

                pilaViz = opStack.empty() ? "Z0" : visualizeStack(opStack);

                outFile<<left<<setw(8)<<stepNumber
                        <<setw(25)<<configInicial
                        <<setw(30)<<transition
                        <<setw(25)<<configFinal
                        <<pilaViz<<endl;
            }
        }
    }

    // Vaciar la pila
    outFile<<"-------------------------------------------------------------------------------------------------"<<endl;
    outFile<<"VACIANDO LA PILA RESTANTE:"<<endl;
    outFile<<"-------------------------------------------------------------------------------------------------"<<endl;

    currentState = "qf";

    while (!opStack.empty()) {
        stepNumber++;

        string topOp(1, opStack.top());
        string configInicial = "(" + currentState + ", ε, " + topOp + ")";

        outputQueue.push(topOp);
        opStack.pop();

        string newTop = opStack.empty() ? "Z0" : string(1, opStack.top());
        string transition = "δ(" + currentState + ", ε, " + topOp + ") → (" + currentState + ", " + newTop + ")";
        string configFinal = "(" + currentState + ", ε, " + newTop + ")";

        string pilaViz = opStack.empty() ? "Z0" : visualizeStack(opStack);

        outFile<<left<<setw(8)<<stepNumber
                <<setw(25)<<configInicial
                <<setw(30)<<transition
                <<setw(25)<<configFinal
                <<pilaViz<<endl;
    }

    // Construir salida
    queue<string> tempQueue = outputQueue;
    while (!tempQueue.empty()) {
        output += tempQueue.front() + " ";
        tempQueue.pop();
    }

    outFile<<endl<<"-------------------------------------------------------------------------------------------------"<<endl;
    outFile<<"COLA DE SALIDA (RESULTADO): "<<visualizeQueue(outputQueue)<<endl;
    outFile<<"-------------------------------------------------------------------------------------------------"<<endl;

    return output;
}
