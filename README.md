# Traductor Infija a Postfija con Autómata de Pila (Algoritmo Shunting-Yard)
---
## **Integrantes** 
- Chamorro Figueroa, Andy Aaron
- Sánchez Mendoza, Diego Joaquín
- Verde Jara, Leonel Edwuar
- Zapata LLaxa, Jorge Andrés

---
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
## **Lógica del Algoritmo Shunting-Yard**
El programa debe iterar sobre los tokens de entrada de izquierda a derecha, aplicando las siguientes reglas clave de simulación del Autómata de Pila.  
| Token de entrada | Regla de transición (AP) |
| --- | --- |
| Operando (número) | Enviar directamente a la cola de salida |
| Paréntesis de apertura '(' | PUSH a la Pila de Operadores |
| Paréntesis de cierre ')' | Hacer POP de operadores de la pila a la cola de salida hasta encontrar el paréntesis de apertura. Descartar ambos paréntesis. |
| Operador ( +, -, *, / ) | Hacer POP de operadores de la pila a la cola de salida mientras el operador en la cima de la pila tenga mayor o igual precedencia que el operador entrante. Luego PUSH del operador entrante. |

---
## **Plan del proyecto**

Seguiremos los siguientes pasos para desarrollar el proyecto:

* Definir los tokens:  
  Veremos qué elementos aceptara el programa en este caso serían números, operadores (+ - * /) y paréntesis.  
  
* Crear las estructuras base:  
  Lo haremos con dos listas en C++:  
    * Una para el resultado en notaciòn postfija  
    * Otra que sería la pila donde se guardarán operadores y paréntesis.  
* Establecer la prioridad de los operadores:  
  Esto sería una pequeña tabla para indicar que * y / tienen más prioridad que + y -.
  | Operadores | Prioridad |
  | --- | --- |
  | / | 2 |
  | * | 2 |
  | + | 1 |
  | - | 1 |
* Implementar las reglas principales del método:  
  Hacer que el programa siga el método Shunting-Yard. Esto incluye decidir qué hacer cuando aparece un número, un operador o un paréntesis.  
* Hacer pruebas:  
  Ver si funciona correctamente con expresiones pequeñas, primero sin paréntesis y luego con ellos.  
* Intentar con expresiones erróneas:  
  Detectar expresiones incorrectas, como paréntesis sin cerrar.  
* Preparar el informe final:  
  Explicar cómo funciona el programa, cómo se usa y el código.

---
## **Ejemplos de ejecución**

**Ejemplo 1.**
El usuario ingresa una cadena con notación infija: (2 + 3) * 5 + 3  
El resultado en notación posfija que debe retornae el algoritmo es: 2 3 + 5 * 3 +

**Ejemplo 2.**
El usuario ingresa una cadena errónea: 2 + 3) * 5  
El programa debe mostrarle un mensaje de error al usuario: ¡ERROR! No se encontró un parentesis de apertura para ')', por lo tanto, la cadena es rechazada.

---
## **Entrada/Salida en el programa**

El presente programa recibe como entrada(input) dos archivos de texto,puede ingresar solo el nombre de los archivos si es que están en el mismo directorio que el ejecutable, o pueden ingresar la ruta absoluta de los archivos (recomendado); el primero tendrá como contenido la notación infija que quiere convertir, cabe recalcar que el programa ignora los espacios, por lo tanto acepta tanto `4 + 3 * ( 5 + 7 )` como `4+3*(5+7)`; el segundo archivo de entrada será el archivo de salida en el cuál se mostrará tanto el resultado como el procedimiento de la conversión. Puede usar el mismo archivo para la entrada y salida, pero tiene que saber que si hace esto, el archivo de salida se sobreescribirá.

     int main(){
        setlocale(LC_ALL, "");
        string inputFileName, outputFileName;
    
        cout<<"Ingrese el nombre del archivo de entrada: ";
        getline(cin, inputFileName);

        cout<<"Ingrese el nombre del archivo de salida: ";
        getline(cin, outputFileName);

        // Abrir archivo de entrada
        ifstream inputFile(inputFileName);
        if (!inputFile.is_open()) {
            cerr<<"Error: No se pudo abrir el archivo de entrada '"<<inputFileName<<"'"<<endl;
            return 1;
        }

        // Leer la expresión infija
        string expr;
        getline(inputFile, expr);
        inputFile.close();

        if (expr.empty()) {
            cerr<<"Error: El archivo de entrada está vacío"<<endl;
            return 1;
        }

        // Abrir archivo de salida
        ofstream outputFile(outputFileName);
        if (!outputFile.is_open()) {
            cerr<<"Error: No se pudo crear el archivo de salida '"<<outputFileName<<"'"<<endl;
            return 1;
        }

        // Escribir encabezado en el archivo de salida
        outputFile<<"----------------------------------------"<<endl;
        outputFile<<"ALGORITMO SHUNTING YARD"<<endl;
        outputFile<<"SIMULACIÓN DE AUTÓMATA DE PILA"<<endl;
        outputFile<<"----------------------------------------"<<endl<<endl;
        outputFile<<"Expresión infija de entrada: "<<expr<<endl;
        outputFile<<"----------------------------------------"<<endl<<endl;

        // Ejecutar el algoritmo y registrar pasos
        string result = shuntingYard(expr, outputFile);

        // Escribir resultado final
        outputFile<<endl<<"----------------------------------------"<<endl;
        outputFile<<"RESULTADO FINAL"<<endl;
        outputFile<<"----------------------------------------"<<endl;
        outputFile<<"Notación postfija: "<<result<<endl;

        outputFile.close();

        cout<<"Proceso completado exitosamente."<<endl;
        cout<<"Revise el archivo '"<<outputFileName<<"' para ver los resultados."<<endl;

        return 0;
    } 

