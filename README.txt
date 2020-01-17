Autor: Ignacio Loayza Campos
Rol: 201273604-8

Este programa es una implementación de la meta-heuristica Hill Climbing Mejor Mejora con Restart
para resolver el problema conocido como 2D-Strip Packing Problem.
Para compilar el programa se adjunta un archivo makefile con la instruccion necesaria. Para compilarlo,
basta ejecutar el comando "$ make" en la consola.


Se incluyen también las instancias entregadas por moodle, las cuales son especificadas
    en el paper "Neighborhood Search Algorithm for the Guillotine Non-Oriented Two-Dimensional
    Bin Packing Problem" de A. Lodi, S. Martello y D. Vigo.


Un ejemplo de ejecución de alguna de las instancias incluidas es:
				$./solver 2sp_martello/BENG04.TXT 
o
				$./solver HopperTurton_Instances/c4-p2    

Notar que las instancias de Lodi et al. están en formato .TXT.

El programa luego de compilar genera el ejecutable con el nombre de "solver, en la misma carpeta",
una vez que se ejecuta el programa sobre una instancia se generá un archivo con el nombre "nombre_instancia_output.txt"
el cual posee la siguiente sintaxis:

Altura_H
Espacio_Inutilizado
x_1	y_1	r_1
x_2	y_2	r_2
...

Donde x_i e y_i son las coordenadas de la esquina inferior izquierda del artículo i y r_i denota si
el artículo debe o no ser rotado (1: rotar, 0: no rotar).


################################ NOTAS TECNICAS ################################

* Se utilizo C++ 14 para compilar este programa.

* Las instancias deben estar simplemente como extensión 'file' o en su defecto en texto plano
    y en formato .txt.
* NO debe haber un salto de linea al final del archivo de la instancia, por alguna razón esto produce que el programa
    caiga en una lectura infinita del archivo, todas las instancias que se incluyen junto con el programa, en las carpetas
    2sp_martello y Hopper & Turton instances estan listas para ser ingresadas al programa.
* El nombre del archivo de la instancia debe ser ingresado de la siguiente forma:
    
                            $ ./solver carpeta_instancias/nombre_instancia

    O en su defecto para el formato .txt
                            $ ./solver carpeta_instancias/nombre_instancia.txt

* La lista completa de headers y librerías necesarias es la siguiente:

- iostream
- list
- fstream
- string
- sstream
- algorithm
- bits/stdc++.h
- chrono
 