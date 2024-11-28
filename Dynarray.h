//
// Created by Paula on 22/11/2024.
//

#ifndef DYNARRAY_H
#define DYNARRAY_H
#include <iostream>
#include <stdexcept>
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class Dynarray {
private:
    T* data;
    size_t size;     //tamaño actual del array
    size_t capacity; //capacidad total del array

    void resize(size_t new_capacity) { //para redimensionar el array
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i]; //aqui copia los elementos actuales
        }
        delete[] data;
        data = new_data;  //para apuntar al nuevo bloque
        capacity = new_capacity;
    }

public:
    explicit Dynarray(size_t initial_capacity = 2) //para inicializarlo con una capacidad inicial
        : data(new T[initial_capacity]), size(0), capacity(initial_capacity) {}
    ~Dynarray() {
        delete[] data;
    }

    void push_back(const T& value) { //metodo para añadir un elemento al final del array
        if (size == capacity) { //si no hay espacio suficiente duplicamos la capacidad del array
            resize(capacity * 2);
        }
        data[size++] = value;
    }
    void pop_back() { //quitamos el ultimo elemeneto
        if (size > 0) {
            --size;
        }
    }
    size_t get_size() const {
        return size;
    }

    size_t get_capacity() const {
        return capacity;
    }

    //operador [] para acceso a elementos (con validación de rango)
    T& operator[](size_t index) {
        if (index >= size) {
            throw out_of_range("Índice fuera de rango");
        }
        return data[index];
    }

    //operador [] para acceso constante
    const T& operator[](size_t index) const {
        if (index >= size) {
            throw out_of_range("Índice fuera de rango");
        }
        return data[index];
    }

    // Función de búsqueda: devuelve el índice del elemento si se encuentra, o -1 si no
    int find(const T& value) const {
        for (size_t i = 0; i < size; ++i) {
            if (data[i] == value) {
                return static_cast<int>(i);
            }
        }
        return -1;  // Elemento no encontrado
    }

    // Función para eliminar un elemento en un índice específico
    void remove_at(size_t index) {
        if (index >= size) {
            throw std::out_of_range("Índice fuera de rango");
        }
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = data[i + 1]; // Desplazar elementos hacia la izquierda
        }
        --size; // Reducir el tamaño
    }

    // Función para ordenar el Dynarray en orden alfabético/ascendente
    void sort() {
        for (size_t i = 0; i < size - 1; ++i) {
            for (size_t j = 0; j < size - 1 - i; ++j) {
                if (data[j] > data[j + 1]) { // Comparación alfabética/ascendente
                    T temp = data[j];
                    data[j] = data[j + 1];
                    data[j + 1] = temp;
                }
            }
        }
    }
};

#endif //DYNARRAY_H
