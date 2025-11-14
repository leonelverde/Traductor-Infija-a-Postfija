# **Traductor Infija a Postfija con Autómata de Pila (Algoritmo Shunting-Yard)**
---
## **¿De qué trata el proyecto?**
---
El proyecto que realizaremos en Python se encargará de convertir las expresiones infijas (Notación natural, la que usamos comúnmente) a postfijas (Notación donde los operadores van después de los operandos) 

Para esto usaremos el método “Shunting-Yard” creado por el científico en computación Edsger Dijkstra. Su nombre significa “Estación de clasificación” esto es así por su forma en la que hace que los operadores y paréntesis 
se muevan de manera que respeten la precedencia correcta.

Para resumir, lo que buscaremos es que el programa analice la expresión en notación infija que el usuario le dé paso por paso cumpliendo la prioridad de los operadores y el uso de paréntesis pasando de infijas a postfijas.
---
## **Plan del proyecto**
Seguiremos los siguientes pasos para desarrollar el proyecto:

•Definir los tokens:
  Veremos qué elementos aceptara el programa en este caso serían números, operadores (+ - * /) y paréntesis.
  
•Crear las estructuras base:
  Lo haremos con dos listas en Python:
    -Una para el resultado en notaciòn postfija
    -Otra que sería la pila donde se guardarán operadores y paréntesis.
•Establecer la prioridad de los operadores:
  Esto sería una pequeña tabla para indicar que * y / tienen más prioridad que + y -.
•Implementar las reglas principales del método:
  Hacer que el programa siga el método Shunting-Yard. Esto incluye decidir qué hacer cuando aparece un número, un operador o un paréntesis.
•Hacer pruebas:
  Ver si funciona correctamente con expresiones pequeñas, primero sin paréntesis y luego con ellos.
•Intentar con expresiones erróneas:
  Detectar expresiones incorrectas, como paréntesis sin cerrar.
•Preparar el informe final:
  Explicar cómo funciona el programa, cómo se usa y el código.

## **Ejecución**
