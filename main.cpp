#include <iostream> // Entrada y salida
#include <fstream>  // Manejo de archivos
#include <stack>    // Usar pilas
#include <queue>    // Usar colas
#include <cctype>   // Funciones para manejo de caracteres (isdigit)
#include <locale>   // Mostrar caracteres especiales en la consola
#include <string>   // Manejo de cadenas
#include <iomanip>  // Formato de salida (alinear con setw)
using namespace std;


// ---- FUNCIONES AUXILIARES ---- //


// Precedencia de operadores: Devuelve la precedencia de un operador
int precedence(char op);

// Es operador: Verifica si un caracter es un operador valido (+, -, *, /)
bool isOperator(char c);

// Algoritmo Shunting Yard: Implementación del algoritmo con registro de pasos
string shuntingYard(const string &expr, ofstream &outFile);

// Genera un string que representa visualmente el contenido de una pila
string visualizeStack(stack<char> s);

// Genera un string que representa visualmente el contenido de una cola.
string visualizeQueue(queue<string> q);


// ---- FUNCION PRINCIPAL ---- //


int main() {
    setlocale(LC_ALL, "");          // Habilita los caracteres especiales (#include locale)

    string inputFileName, outputFileName;
    // Crea dos variables, la primera es el nombre del archivo de entrada (lectura) y la segunda es el nombre del archivo de salida (grabar)

    cout<<"Ingrese el nombre del archivo de entrada: ";
    getline(cin, inputFileName);    // Puede tener espacios el nombre

    cout<<"Ingrese el nombre del archivo de salida: ";
    getline(cin, outputFileName);   // Puede tener espacios el nombre

    // Abrir archivo de entrada
    /* Declaro un archivo de entrada (input - leer) con el identificador inputFile en el cual el programa recibe la informacion
    guardada en el archivo .txt creado llamado inputFileName */
    ifstream inputFile(inputFileName);

    if (!inputFile.is_open()) {     // Si el archivo de entrada no esta abierto entonces:

        cerr<<"Error: No se pudo abrir el archivo de entrada '"<<inputFileName<<"'"<<endl;
        // Si devuelve false es porque el archivo de entrada no existe.
        // Se usa cerr para mostrar errores (mejor depuracion), no usa buffer en comparacion como cout.
        return 1;
    }

    // Leer la expresión infija
    string expr;    // Declaro de tipo string a la variable expr la cual representa la expresion infija
    getline(inputFile, expr);   // Ingreso por consola la expresion infija

    inputFile.close();  // Cerra archivo de entrada


    if (expr.empty()) { // Si la expresion infija esta vacia entonces:
        cerr<<"Error: El archivo de entrada está vacío"<<endl; // Muestra error
        return 1;
    }

    // Abrir archivo de salida
    /* Declaro una archivo de salida (output - salida) con el identificador outputFile en el cual el programa envia informacion
    al archivo .txt creado llamado outputFileName para mostrar el procedimiento y resultado */
    ofstream outputFile(outputFileName);

    if (!outputFile.is_open()) {    // El archivo de salida no esta abierto entonces:
        cerr<<"Error: No se pudo crear el archivo de salida '"<<outputFileName<<"'"<<endl;
        // Si es false es porque el archivo de salida no existe.
        return 1;
    }

    // Escribir encabezado en el archivo de salida
    // outputFile para escribir en el archivo de salida
    outputFile<<"----------------------------------------"<<endl;
    outputFile<<"ALGORITMO SHUNTING YARD"<<endl;
    outputFile<<"SIMULACIÓN DE AUTÓMATA DE PILA"<<endl;
    outputFile<<"----------------------------------------"<<endl<<endl;
    outputFile<<"Expresión infija de entrada: "<<expr<<endl;    // Escribe la notacion infija en el archivo de salida
    outputFile<<"----------------------------------------"<<endl<<endl;

    // Ejecutar el algoritmo y registrar pasos
    // Argumentos: expr (notacion infija), outputFile (identificador del archivo de salida en la funcion shuntingYard)
    string result = shuntingYard(expr, outputFile);

    // Escribir resultado final
    outputFile<<endl<<"----------------------------------------"<<endl;
    outputFile<<"RESULTADO FINAL"<<endl;
    outputFile<<"----------------------------------------"<<endl;
    outputFile<<"Notación postfija: "<<result<<endl;    // result es la notacion posfija, lo escribe en el archivo de salida

    // Cerrar archivo de salida
    outputFile.close();

    cout<<"Proceso completado exitosamente."<<endl;

    // Nombre del archivo .txt donde esta guardado el procedimiento y resultado de la notacion posfija
    cout<<"Revise el archivo '"<<outputFileName<<"' para ver los resultados."<<endl;

    return 0;
}

// Determinar la precedencia de un operador
int precedence(char op) {   // Recibe como parametro el char op que es un operador
    switch (op) {
        case '+':
        case '-':
            return 1;   // Si el operador es + o - retorna 1 (precedencia 1)
        case '*':
        case '/':
            return 2;   // Si el operador es * o / retorna 1 (precedencia 1)
    }
    return 0;   // Si no es ninguno de los operadores anteriores, retorna 0 (precedencia 0) pueden ser parentesis, numeros o caracteres especiales
}

// Verificar si un caracter es un operador
bool isOperator(char c) {   // Recibe como parametro el caracter con identificador c
    return c == '+' || c == '-' || c == '*' || c == '/'; // Devuelve true si el carácter es +, -, *, /; de lo contrario false
}

// Funcion Visualializar Pila
string visualizeStack(stack<char> s) { // Recibe como parametro la variable s que es de tipo pila de caracteres
    // Se pasa por valor, es decir que la pila original no se modifica, solo sirve para escribir el procedimiento de la pila en el archivo de salida

    if (s.empty()){     // Si la pila con identificador s esta vacio entonces:
        // True
        return "[ VACÍA ]";
    }

    string result = "[ ";   // Empieza a construir la representacion de la pila en la variable tipo string con identificador result
    stack<char> temp; // Definimos una pila de caracteres temp para mostrar la pila s desde la base hasta el tope

    while (!s.empty()) {    // Mientras la pila con identificador s no esta vacia hacer:
        temp.push(s.top()); // Apilamos (push) en la pila temp el elemento que se encuentra en la cima de la pila s
        s.pop();    // Se desapila el elemento de la cima de la pila s
    }

    while (!temp.empty()) { // Mientras la pila con identificador temp no esta vacia hacer:
        result += temp.top();   // Añade a result el token de la cima de la pila temp (base de la pila con identificador s)
        temp.pop();        // Desapila el token de la cima de la pila temp
        if (!temp.empty()) {    // Si la pila temp no esta vacia entonces:
            result += ", ";  // Añadir al result una coma seguido de un espacio en blanco
        }
    }

    result += " ] ← tope";  // Añadir al result el cierre de corechetes e indicar que se encuentra en el tope
    return result;  // Retorna la representacion del funcionamiento de la pila
}

// Funcion Visualializar Cola
string visualizeQueue(queue<string> q) {  // Recibe como parametro la variable q que es de tipo cola de cadenas
    // Se pasa por valor, es decir que la cola original no se modifica, solo sirve para escribir el procedimiento de la cola en el archivo de salida

    if (q.empty()) {    // Si la cola con identificador q esta vacia entonces:
        // True
        return "[ VACÍA ]";
    }

    string result = "[ ";
    // Empieza a construir la representacion de la cola en la variable tipo string con identificador result
    // (usada antes para la representacion de la pila s)

    while (!q.empty()) {    // Mientras la cola q no esta vacia hacer:
        result += q.front();    // Añade a result el token del frente de la cola q
        q.pop();    // Desapila el  token que se encuentra al frente de la cola q (primero en entrar, primero en salir)
        if (!q.empty()) { // Si la cola q no esta vacia entonces:
            result += ", "; // Añadimos al result una coma seguido de un espacio en blanco
        }
    }
    result += " ]"; // Añadir al result el cierre de corechetes
    return result;  // Retorna la representacion del funcionamiento de la cola escrito despues de lo que retornaba result (funcionamiento de la pila)
}

// Función principal del algoritmo Shunting Yard.
string shuntingYard(const string &expr, ofstream &outFile) {  // Recibe como paramtro expr (notacion infija) y outFile (archivo de salida)
    // Devuelve por referencia la variable expr, la funcion shuntingYard no puede modifcar la variable expr porque es const
    // Devuelve por referencia el archivo de salida outFile, puede escribir en el archivo de salida orignal sin hacer copias

    stack<char> opStack;    // Pila de caracteres (simula la pila de operadores del autómata)
    queue<string> outputQueue;  // Cola para almacenar temporalmente la notación postfija
    string output;      // Cadena que almacenará finalmente la notación postfija
    int stepNumber = 0;     // Contador de pasos para el registro en archivo
    string currentState = "q0";     // Estado inicial del autómata de pila

    // Escribir cabecera de la tabla de transiciones
    outFile<<"TRANSICIONES DEL AUTÓMATA DE PILA:"<<endl;
    outFile<<"-------------------------------------------------------------------------------------------------"<<endl;
    outFile<<left<<setw(8)<<"PASO"  // setw(8) indica que la siguiente valor que se imprime ocupara 8 espacios en blanco
            <<setw(25)<<"CONFIG. INICIAL"
            <<setw(30)<<"TRANSICIÓN APLICADA"
            <<setw(25)<<"CONFIG. FINAL"
            <<"PILA"<<endl;
    outFile<<"-------------------------------------------------------------------------------------------------"<<endl;

    // Registrar la configuración inicial del autómata
    outFile<<left<<setw(8)<<stepNumber      // left alinea hacia la izquierda
            <<setw(25)<<"(q0, " + expr + ", Z0)"
            <<setw(30)<<"Inicial"
            <<setw(25)<<""
            <<"Z0"<<endl;
    outFile<<"-------------------------------------------------------------------------------------------------"<<endl;

    string remainingInput = expr; // Declaramos la variable remainingInput tipo string para guardar la expresion infija que falta procesar

    // Para la variable i=0 de tipo size_t, i es menos que el tamaño de la expresion infija, incrementa el contador i
    for (size_t i = 0; i < expr.size(); i++) {
        /* size_t es un tipo de dato usado para tamaños o indices (Para una mayor depuracion del codigo). No almacena buffer, siempre positivo */

        char token = expr[i];   // Almacenar el valor de la exp[i] en la variable tipo char con identificador token

        // Ignorar espacios en blanco
        if (token == ' ') {
            remainingInput = remainingInput.substr(1); // reemplaza la variable remainingInput por la subcadena remainingInput.substr(1) que elimina el primer caracter (poes eso el 1) de remainingInput (espacio en blanco)
            continue;
        }

        stepNumber++;   // Incrementar contador de pasos

        string configInicial, transition, configFinal, pilaViz; // Variables para registrar transiciones
        string prevState = currentState; // Guardar estado previo

        // Detectar números enteros con varios dígitos
        if (isdigit(token)) {   // Si el caracter actual (token=expr[i]) es un numero de 0 - 9 entonces;

            string number; // Declaramos una variable tipo string con identificador number para guardar los numeros de la expresion posfija

            // Permitir leer un numero de mas de un digito
            while (i < expr.size() && isdigit(expr[i])) { // Mientra i es menor al tamaño de la expresio ninfija y expr[i] es un digito hacer:
                number += expr[i];  // La variable number almacena expr[i]
                i++;
            }
            i--;    // Disminuye i para evitar el salto del caracter que no es un numero

            string topStack = opStack.empty() ? "Z0" : string(1, opStack.top()); // Tope de pila
            configInicial = "(" + currentState + ", " + remainingInput + ", " + topStack + ")";

            outputQueue.push(number);  // Enviar número a la cola de salida

            // Registrar transición del autómata
            transition = "δ(" + currentState + ", " + number + ", " + topStack + ") → (q1, " + topStack + ")";
            currentState = "q1";

            remainingInput = remainingInput.substr(number.length()); // Actualizar entrada restante
            configFinal = "(q1, " + remainingInput + ", " + topStack + ")";
            pilaViz = opStack.empty() ? "Z0" : visualizeStack(opStack);

            // Escribir paso en la tabla
            outFile << left << setw(8) << stepNumber
                    << setw(25) << configInicial
                    << setw(30) << transition
                    << setw(25) << configFinal
                    << pilaViz << endl;
        }

        // Detectar operadores
        else if (token == '+' || token == '-' || token == '*' || token == '/') {
            // Desapilar operadores de mayor o igual precedencia
            while (!opStack.empty() && (opStack.top() == '+' || opStack.top() == '-' || opStack.top() == '*' || opStack.top() == '/') &&
                   ((token == '+' || token == '-') && (opStack.top() == '+' || opStack.top() == '-')) ||
                   ((token == '*' || token == '/') && (opStack.top() == '*' || opStack.top() == '/'))) {

                string topOp(1, opStack.top());
                configInicial = "(" + currentState + ", " + remainingInput + ", " + topOp + ")";
                outputQueue.push(topOp);
                opStack.pop();

                // Transición ε, no se consume entrada
                transition = "δ(" + currentState + ", ε, " + topOp + ") → (q1, ";
                currentState = "q1";

                string newTop = opStack.empty() ? "Z0" : string(1, opStack.top());
                transition += newTop + ")";
                configFinal = "(q1, " + remainingInput + ", " + newTop + ")";
                pilaViz = opStack.empty() ? "Z0" : visualizeStack(opStack);

                outFile << left << setw(8) << stepNumber++
                        << setw(25) << configInicial
                        << setw(30) << transition
                        << setw(25) << configFinal
                        << pilaViz << endl;
            }

            // Apilar el operador actual
            string topStack = opStack.empty() ? "Z0" : string(1, opStack.top());
            configInicial = "(" + currentState + ", " + remainingInput + ", " + topStack + ")";
            opStack.push(token);

            transition = "δ(" + currentState + ", " + string(1, token) + ", " + topStack + ") → (q0, " + string(1, token) + topStack + ")";
            currentState = "q0";

            remainingInput = remainingInput.substr(1);
            configFinal = "(q0, " + remainingInput + ", " + string(1, token) + ")";
            pilaViz = visualizeStack(opStack);

            outFile << left << setw(8) << stepNumber
                    << setw(25) << configInicial
                    << setw(30) << transition
                    << setw(25) << configFinal
                    << pilaViz << endl;
        }

        // Paréntesis izquierdo
        else if (token == '(') {
            string topStack = opStack.empty() ? "Z0" : string(1, opStack.top());
            configInicial = "(" + currentState + ", " + remainingInput + ", " + topStack + ")";
            opStack.push(token);

            transition = "δ(" + currentState + ", (, " + topStack + ") → (q0, (" + topStack + ")";
            currentState = "q0";

            remainingInput = remainingInput.substr(1);
            configFinal = "(q0, " + remainingInput + ", ()";
            pilaViz = visualizeStack(opStack);

            outFile << left << setw(8) << stepNumber
                    << setw(25) << configInicial
                    << setw(30) << transition
                    << setw(25) << configFinal
                    << pilaViz << endl;
        }

        // Paréntesis derecho
        else if (token == ')') {
            // Desapilar hasta encontrar '('
            while (!opStack.empty() && opStack.top() != '(') {
                string topOp(1, opStack.top());
                configInicial = "(" + currentState + ", " + remainingInput + ", " + topOp + ")";
                outputQueue.push(topOp);
                opStack.pop();

                transition = "δ(" + currentState + ", ε, " + topOp + ") → (q1, ";
                currentState = "q1";

                string newTop = opStack.empty() ? "Z0" : string(1, opStack.top());
                transition += newTop + ")";
                configFinal = "(q1, " + remainingInput + ", " + newTop + ")";
                pilaViz = opStack.empty() ? "Z0" : visualizeStack(opStack);

                outFile << left << setw(8) << stepNumber++
                        << setw(25) << configInicial
                        << setw(30) << transition
                        << setw(25) << configFinal
                        << pilaViz << endl;
            }

            // Eliminar '(' de la pila
            if (!opStack.empty()) {
                configInicial = "(" + currentState + ", " + remainingInput + ", ()";
                opStack.pop();

                string newTop = opStack.empty() ? "Z0" : string(1, opStack.top());
                transition = "δ(" + currentState + ", ), () → (q0, " + newTop + ")";
                currentState = "q0";

                remainingInput = remainingInput.substr(1);
                configFinal = "(q0, " + remainingInput + ", " + newTop + ")";
                pilaViz = opStack.empty() ? "Z0" : visualizeStack(opStack);

                outFile << left << setw(8) << stepNumber
                        << setw(25) << configInicial
                        << setw(30) << transition
                        << setw(25) << configFinal
                        << pilaViz << endl;
            }
        }
    }

    // Vaciar la pila restante
    outFile << "-------------------------------------------------------------------------------------------------" << endl;
    outFile << "VACIANDO LA PILA RESTANTE:" << endl;
    outFile << "-------------------------------------------------------------------------------------------------" << endl;

    currentState = "qf"; // Estado final

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

        outFile << left << setw(8) << stepNumber
                << setw(25) << configInicial
                << setw(30) << transition
                << setw(25) << configFinal
                << pilaViz << endl;
    }

    // Construir salida final como cadena
    queue<string> tempQueue = outputQueue;
    while (!tempQueue.empty()) {
        output += tempQueue.front() + " ";
        tempQueue.pop();
    }

    outFile << endl << "-------------------------------------------------------------------------------------------------" << endl;
    outFile << "COLA DE SALIDA (RESULTADO): " << visualizeQueue(outputQueue) << endl;
    outFile << "-------------------------------------------------------------------------------------------------" << endl;

    return output; // Devolver notación postfija
}
