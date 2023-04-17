# Proyecto 1: Organización de Archivos

Este proyecto consiste en implementar dos técnicas de organización de archivos para datos reales que se encuentran en al menos dos archivos planos. Las técnicas que se implementarán son: Sequential File, AVL File, ISAM-Sparse Index, Extendible Hashing y B+ Tree.

## Logro del estudiante
El objetivo del estudiante es entender y aplicar los algoritmos de almacenamiento de archivos físicos y acceso concurrente.

## Enunciado
En grupos de hasta cuatro integrantes, se deberá elegir un dominio de datos que conste de al menos dos archivos planos con datos reales que se encuentran en Kaggle. Luego, sobre ellos se implementarán dos técnicas de organización de archivos, junto con sus principales operaciones: inserción, eliminación y búsqueda.

## Requerimientos generales
a. Se deben implementar dos de las siguientes técnicas de organización de archivos en memoria secundaria:
1. Sequential File
2. AVL File
3. ISAM-Sparse Index
4. Extendible Hashing
5. B+ Tree

b. Se debe escoger una de las siguientes combinaciones de técnicas de organización de archivos:
- Sequential File o AVL File o ISAM-Sparse Index
- Extendible Hash o B+Tree

Además, se deberán implementar las siguientes operaciones:
1. Búsqueda específica que puede retornar más de un elemento que coincide con la clave: `vector<Registro> search(T key)`
2. Búsqueda por rango que retorna todos los registros que calzan entre dos claves de búsqueda: `vector<Registro> rangeSearch(T begin_key, T end_key)`
3. Agregar un registro al archivo respetando la técnica de organización: `bool add(Registro registro)`
4. Proponer un algoritmo de eliminación para cada técnica: `bool remove(T key)`

c. Se debe implementar un parser de código SQL a sentencias ejecutables.

d. Se debe mostrar los resultados de forma amigable al usuario, utilizando la siguiente GUI de referencia.

## GUI de referencia
La GUI debe mostrar los resultados de las operaciones realizadas de forma amigable al usuario. Se sugiere la siguiente GUI de referencia:

![GUI de referencia](https://i.imgur.com/rOuO7yE.png)
