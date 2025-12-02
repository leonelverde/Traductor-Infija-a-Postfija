# Traductor Infija a Postfija con Autómata de Pila (Algoritmo Shunting-Yard)
---
## **Integrantes** 
- Chamorro Figueroa, Andy Aaron
- Sánchez Mendoza, Diego Joaquín
- Verde Jara, Leonel Edwuar
- Zapata LLaxa, Jorge Andrés

---
# **Introducción**

## **Descripción del proyecto**
El proyecto que realizaremos en C++ se encargará de convertir las expresiones infijas (Notación natural, la que usamos comúnmente) a postfijas (Notación donde los operadores van después de los operandos) 

Para esto usaremos el método “Shunting-Yard” creado por el científico en computación Edsger Dijkstra. Su nombre significa “Estación de clasificación” esto es así por su forma en la que hace que los operadores y paréntesis 
se muevan de manera que respeten la precedencia correcta.

Para resumir, lo que buscaremos es que el programa analice la expresión en notación infija que el usuario le dé paso por paso cumpliendo la prioridad de los operadores y el uso de paréntesis pasando de infijas a postfijas.

---
## **Necesidad de la traducción**

La principal razón por qué se necesita la traducción del algoritmo Shunting-Yard es porque las computadoras no son capaces de interpretar por sí mismas las expresiones que utilizamos generalmente los humanos. Nosotros escribimos en notación infija. 

Por ejemplo:	3 + 4 * (2 - 1)

La notación infija requiere de reglas de precedencia como que la multiplicación se hace antes que la suma o que los paréntesis alteran el orden, llegando a ser ambiguas para las computadoras debido a que estas reglas no vienen “incluidas” en la expresión matemática, por lo tanto la computadora tendría que interpretarlas cada vez. Además que las traducciones directas desde infijo a código ejecutable son difíciles de realizar por los lenguajes de programación lo que llega a ser ineficiente y vulnerable a errores.

---
## **El rol del algoritmo Shunting-Yard como un Autómata de Pila**

El funcionamiento del algoritmo Shunting-Yard se relaciona adecuadamente con las características de un autómata de pila debido a las siguientes causas:

- **Lee una expresión infija símbolo por símbolo**:
  Cada token (número, operador o paréntesis) se procesa uno seguido del otro semejante a un autómata de pila que procesa una cadena.

- **Utiliza una pila para controlar la estructura** :
  Se guardan en una pila los operadores y paréntesis. Es decir, se almacena información necesaria acerca de la estructura del lenguaje similar a lo que hace un autómata de pila.

- **Responde de acuerdo el símbolo actual + el tope de la pila**:
  Sigue ciertas reglas de acuerdo al operador que se encuentra en el tope de la pila y el símbolo actual.

- **La salida se obtiene mientras se utiliza la pila**:
  Puede generar cadenas (autómata de pila con salida) como un AP.

---
# **Diseño del AP (Shunting-Yard)**

## **Estructuras**

* **Pila de operadores (Stack)**  

*Declaración:*  

```cpp
stack<char> opStack;
```  

 *Propósito:* Almacenar temporalmente los operadores y paréntesis durante el procesamiento de la expresión infija, respetando la precedencia de operadores.  
 *Operaciones utilizadas:*  
1. `push (char operador)`: Apila un operador o paréntesis  
```cpp
opStack.push('+');  // Apila el operador +
```
2. `pop()`: Desapila el operador del tope (lo elimina)
```cpp
opStack.pop();  // Elimina el operador del tope
```
3. `top()`: Consulta el operador del tope sin eliminarlo
```cpp
char tope = opStack.top();  // Lee el tope sin desapilar
```
4. `empty()`: Verifica si la pila está vacía  
```cpp
if (opStack.empty()) { /* Pila vacía */ }
```

* **Cola de salida (Queue)**

*Declaración:*

```cpp
queue<string> outputQueue;
```

  *Propósito:* Construir la expresión en notación postfija (RPN) agregando operandos y operadores en el orden correcto.  
  *Operaciones utilizadas:*  
1. `push (string elemento)`: Agrega un elemento al final de la cola  
```cpp
outputQueue.push("3");     // Agrega el número 3
outputQueue.push("+");     // Agrega el operador +
```
2. `front()`: Consulta el primer elemento sin eliminarlo  
```cpp
string primero = outputQueue.front();
```
3. `pop()`: Elimina el primer elemento de la cola  
```cpp
outputQueue.pop();  // Elimina el primer elemento
```
4. `empty()`: Verifica si la cola está vacía  
```cpp
if (outputQueue.empty()) { /* Cola vacía */ }
```
## **Tabla de precedencia**  

*Jerarquía de operadores:* Esto sería una pequeña tabla para indicar que * y / tienen más prioridad que + y -.
  | Operadores | Prioridad |
  | --- | --- |
  | / | 2 |
  | * | 2 |
  | + | 1 |
  | - | 1 |

*Implementación en el código:*  
```cpp
int precedence(char op) {
    switch (op) {
        case '+':
        case '-': return 1;  // Menor precedencia
        case '*':
        case '/': return 2;  // Mayor precedencia
    }
    return 0; // No es operador
}
```  

## **Funciones de transición**  

## 1. Definición Formal del Autómata

**M = (Q, Σ, Γ, δ, q0, Z0, F)**

- **Q** = {q0, q1, qf} → Conjunto de estados
- **Σ** = {números, +, -, *, /, (, )} → Alfabeto de entrada
- **Γ** = {+, -, *, /, (, Z0} → Alfabeto de la pila
- **δ** → Función de transición (ver tabla abajo)
- **q0** → Estado inicial (lectura)
- **Z0** → Símbolo inicial de la pila
- **F** = {qf} → Estado final (aceptación)

---

## 2. Estados del Autómata

| Estado | Descripción |
|--------|-------------|
| **q0** | Estado de lectura - Esperando leer el siguiente símbolo de entrada |
| **q1** | Estado de procesamiento - Después de enviar un elemento a la cola de salida |
| **qf** | Estado final - Vaciando la pila al terminar la entrada |

---

## 3. Tabla Resumen de Transiciones

| Función | Estado Actual | Entrada | Tope Pila | Acción | Nuevo Estado | Nueva Pila |
|---------|---------------|---------|-----------|--------|--------------|------------|
| δ₁ | q0 | número | X | Enviar a cola | q1 | X |
| δ₂ | q1 | ε | op | Desapilar a cola | q1 | X |
| δ₃ | q1 | op | X | Apilar op | q0 | opX |
| δ₄ | q0 | ( | X | Apilar ( | q0 | (X |
| δ₅ | q0 | ) | op | Desapilar a cola | q0 | X |
| δ₆ | q0 | ) | ( | Eliminar ( | q0 | X |
| δ₇ | qf | ε | op | Desapilar a cola | qf | X |

---
## 4. Descripción Detallada de Transiciones

### δ₁: Procesamiento de Operandos
```
δ(q0, número, X) → (q1, X)
```
**Descripción:** Al leer un número, se envía directamente a la cola de salida. La pila permanece sin cambios.

**Código:**
```cpp
if (isdigit(token)) {
    outputQueue.push(number);
    currentState = "q1";
}
```

---

### δ₂: Desapilar Operadores (POP Condicional)
```
δ(q1, ε, op) → (q1, X)
```
**Descripción:** Desapila operadores de mayor o igual precedencia y los envía a la cola de salida.

**Código:**
```cpp
while (!opStack.empty() && isOperator(opStack.top()) &&
       precedence(token) <= precedence(opStack.top())) {
    outputQueue.push(opStack.top());
    opStack.pop();
    currentState = "q1";
}
```

**Condición clave:**
```cpp
precedence(token) <= precedence(opStack.top())
```
- Si **TRUE** → Desapilar (el tope tiene mayor prioridad)
- Si **FALSE** → Apilar directamente

**Ejemplo:**
```
Llega: +
Pila: [*]
precedence(+) = 1 ≤ precedence(*) = 2 → TRUE
Acción: Desapilar * primero
```

---

### δ₃: Apilar Operadores
```
δ(q1, op, X) → (q0, opX)
```
**Descripción:** Después de desapilar los operadores necesarios, se apila el nuevo operador.

**Código:**
```cpp
opStack.push(token);
currentState = "q0";
```

---

### δ₄: Apilar Paréntesis Izquierdo
```
δ(q0, (, X) → (q0, (X)
```
**Descripción:** Los paréntesis izquierdos se apilan directamente, marcando el inicio de una sub-expresión.

**Código:**
```cpp
if (token == '(') {
    opStack.push('(');
    currentState = "q0";
}
```

---

### δ₅: Desapilar hasta encontrar '('
```
δ(q0, ), op) → (q0, X)   donde op ≠ (
```
**Descripción:** Al encontrar un paréntesis derecho, se desapilan todos los operadores hasta encontrar el '(' correspondiente.

**Código:**
```cpp
while (!opStack.empty() && opStack.top() != '(') {
    outputQueue.push(opStack.top());
    opStack.pop();
}
```

---

### δ₆: Eliminar Paréntesis Izquierdo
```
δ(q0, ), () → (q0, X)
```
**Descripción:** Una vez encontrado el '(', se elimina de la pila sin enviarlo a la cola.

**Código:**
```cpp
if (!opStack.empty() && opStack.top() == '(') {
    opStack.pop();  // No se envía a la cola
}
```

---

### δ₇: Vaciar Pila Final
```
δ(qf, ε, op) → (qf, X)
```
**Descripción:** Al terminar la entrada, se desapilan todos los operadores restantes a la cola.

**Código:**
```cpp
while (!opStack.empty()) {
    outputQueue.push(opStack.top());
    opStack.pop();
}
```

---

# **Instrucciones de uso**  

## Requisitos Previos
- Compilador C++ compatible con C++11 o superior (g++, clang, MinGW, Visual Studio)
- Sistema operativo: Linux, macOS o Windows

---

## Compilación

**Linux / macOS:**
```bash
g++ -o shunting_yard shunting_yard.cpp -std=c++11
```

**Windows (MinGW):**
```cmd
g++ -o shunting_yard.exe shunting_yard.cpp -std=c++11
```

**Windows (Visual Studio):**
```cmd
cl shunting_yard.cpp /EHsc
```
# **Guia de ejecución**


# **Resultados de prueba**  

## Caso Básico

**Entrada:** `2+3*4`

**Salida Postfija:** `2 3 4 * +`

**Transiciones del Autómata:**
```
PASO    CONFIG. INICIAL          TRANSICIÓN APLICADA           CONFIG. FINAL            PILA
=================================================================================================
0       (q0, 2+3*4, Z0)          Inicial                                                Z0
1       (q0, 2+3*4, Z0)          δ(q0, 2, Z0) → (q1, Z0)       (q1, +3*4, Z0)           Z0
2       (q1, +3*4, Z0)           δ(q1, +, Z0) → (q0, +Z0)      (q0, 3*4, +)             [ + ]
3       (q0, 3*4, +)             δ(q0, 3, +) → (q1, +)         (q1, *4, +)              [ + ]
4       (q1, *4, +)              δ(q1, *, +) → (q0, *+)        (q0, 4, *)               [ +, * ]
5       (q0, 4, *)               δ(q0, 4, *) → (q1, *)         (q1, ε, *)               [ +, * ]
6       (qf, ε, *)               δ(qf, ε, *) → (qf, +)         (qf, ε, +)               [ + ]
7       (qf, ε, +)               δ(qf, ε, +) → (qf, Z0)        (qf, ε, Z0)              Z0

Cola de salida: [ 2, 3, 4, *, + ]
```

## Caso con Paréntesis

**Entrada:** `(2+3)*4`

**Salida Postfija:** `2 3 + 4 *`

**Transiciones del Autómata:**
```
PASO    CONFIG. INICIAL          TRANSICIÓN APLICADA           CONFIG. FINAL            PILA
=================================================================================================
0       (q0, (2+3)*4, Z0)        Inicial                                                Z0
1       (q0, (2+3)*4, Z0)        δ(q0, (, Z0) → (q0, (Z0)      (q0, 2+3)*4, ()          [ ( ]
2       (q0, 2+3)*4, ()          δ(q0, 2, () → (q1, ()         (q1, +3)*4, ()           [ ( ]
3       (q1, +3)*4, ()           δ(q1, +, () → (q0, +()        (q0, 3)*4, +)            [ (, + ]
4       (q0, 3)*4, +)            δ(q0, 3, +) → (q1, +)         (q1, )*4, +)             [ (, + ]
5       (q1, )*4, +)             δ(q1, ε, +) → (q1, ()         (q1, )*4, ()             [ ( ]
6       (q1, )*4, ()             δ(q1, ), () → (q0, Z0)        (q0, *4, Z0)             Z0
7       (q0, *4, Z0)             δ(q0, *, Z0) → (q0, *Z0)      (q0, 4, *)               [ * ]
8       (q0, 4, *)               δ(q0, 4, *) → (q1, *)         (q1, ε, *)               [ * ]
9       (qf, ε, *)               δ(qf, ε, *) → (qf, Z0)        (qf, ε, Z0)              Z0

Cola de salida: [ 2, 3, +, 4, * ]
```
## Caso Rechazado

**Entrada:** `(2+3*4`

**Transiciones del Autómata:**
```
PASO    CONFIG. INICIAL          TRANSICIÓN APLICADA           CONFIG. FINAL            PILA
=================================================================================================
0       (q0, (2+3*4, Z0)         Inicial                                                Z0
1       (q0, (2+3*4, Z0)         δ(q0, (, Z0) → (q0, (Z0)      (q0, 2+3*4, ()           [ ( ]
2       (q0, 2+3*4, ()           δ(q0, 2, () → (q1, ()         (q1, +3*4, ()            [ ( ]
3       (q1, +3*4, ()            δ(q1, +, () → (q0, +()        (q0, 3*4, +)             [ (, + ]
4       (q0, 3*4, +)             δ(q0, 3, +) → (q1, +)         (q1, *4, +)              [ (, + ]
5       (q1, *4, +)              δ(q1, *, +) → (q0, *+)        (q0, 4, *)               [ (, +, * ]
6       (q0, 4, *)               δ(q0, 4, *) → (q1, *)         (q1, ε, *)               [ (, +, * ]
7       (qf, ε, *)               δ(qf, ε, *) → (qf, +)         (qf, ε, +)               [ (, + ]
8       (qf, ε, +)               δ(qf, ε, +) → (qf, ()         (qf, ε, ()               [ ( ]

ERROR: Pila final contiene '(' → Paréntesis desbalanceados 
```

**Resultado:**  **Expresión rechazada - Paréntesis sin cerrar**  

---
# **Simulación en JFLAP**  

![](https://raw.githubusercontent.com/leonelverde/Traductor-Infija-a-Postfija/refs/heads/main/JFLAP_infija_to_posfija.png)  

## Explicación de la simulación en JFLAP

**Definimos el autómata de pila**

* **Q (Conjunto de Estados):** {q0, q1, q_fin}
* **∑ (Alfabeto de Entrada):** {x, +, *, (, )}
* **Γ (Alfabeto de la Pila):** {Z, (, +, *}
* **q0 (Estado Inicial):** q0
* **Z0 (Símbolo Inicial de Pila):** Z
* **F (Conjunto de Estados Finales):** {q_fin}
* **δ (Función de Transición):** Es el conjunto de flechas que se ven en la imagen.
  
**Explicación de transiciones**

**A. ¿Qué significa x, λ ; λ? (La transición de q0 a q1)**
* Representa cuando leemos un número (x). En el algoritmo Shunting Yard, los números pasan directamente a la salida. Por eso leemos 'x', no sacamos nada de la pila  y no metemos nada a la pila. Simplemente cambiamos de estado para indicar que ya leímos un operando.
  
**B. El "Bucle" en q1 (Manejo de Precedencia)**
* Hay transiciones  λ, * ; λ o λ, + ; λ que se quedan en q1.
* Estas son transiciones (epsilon). Significan que sin leer nada de la entrada, el autómata decide sacar operadores de la pila para enviarlos a la salida. Esto ocurre cuando el operador que queremos insertar tiene menor jerarquía que el que está en la pila.
  
**C. El viaje de q1 de regreso a q0**
* Flechas con + , Z ; +Z o * , + ; *+.
* Cuando encontramos un operador (como + o *), volvemos al estado inicial q0 para esperar el siguiente número. En ese viaje, apilamos al operador sobre lo que ya había en la pila (por ejemplo, ponemos + sobre Z).
  
**D. Los Paréntesis**
* **Apertura “(“:** En q0, si viene (, se apila y nos quedamos en q0.
* **Cierre “)”:** En q1, si viene ), empezamos a desapilar (sacar cosas de la pila) hasta encontrar el paréntesis de apertura.
  
**E. El Estado Final (q_fin)**
* Cuando se acaba la entrada y llegamos al fondo de la pila (Z), el autómata acepta la cadena, saca el fondo de pila y termina el proceso con éxito.
* También si quedó algún símbolo en la pila desapilamos todo antes de sacar el Z del fondo de la pila.
  
---
# **Conclusiones**  


* **Traducción Eficiente (RPN):** El algoritmo Shunting-Yard transforma la expresión infija (ambigua) a **Notación Polaca Inversa (RPN)**, la cual es no ambigua y elimina la necesidad de tablas de precedencia para la evaluación computacional.

* **Modelo AP Adecuado:** El diseño como **Autómata de Pila (AP)** es ideal. La **Pila de Operadores** es la herramienta clave para **gestionar y recordar la jerarquía de los operadores** y el orden de la expresión.

* **Validación de Sintaxis:** El AP no solo traduce, sino que también funciona como un validador. Es capaz de **detectar automáticamente errores sintácticos**, como el **desbalanceo de paréntesis**, si al finalizar el proceso la pila no está vacía.

* **Diseño Robusto:** Al formalizar las reglas de procesamiento como **Funciones de Transición ($\delta$)**, se logra un código C++ **modular, robusto y fácil de entender** que implementa correctamente la lógica de precedencia.
