//
// Created by Paula on 19/11/2024.
//

#ifndef VECTOR_H
#define VECTOR_H

#endif //VECTOR_H
#include <stdexcept>
#include <utility> // Para std::swap

template<typename T> // T debe ser un tipo default_initializable
class VectorPEL {
private:
    T* v_;        // Inicio del espacio de capacidad reservada para el vector
    T* space_;    // Fin de la secuencia de elementos almacenados
    T* last_;     // Fin de la capacidad reservada para el vector

    // Verifica si el índice está dentro de los límites
    void bounds_check(std::size_t i) const {
        if (i >= size())
            throw std::out_of_range{};
    }

public:
    // Constructor por defecto (vector vacío)
    VectorPEL()
        : v_{new T[0]}, space_{v_}, last_{v_} {}

    // Constructor de copia
    VectorPEL(VectorPEL<T> const& v)
        : v_{new T[v.capacity()]}, space_{v_ + v.size()}, last_{v_ + v.capacity()} {
        try {
            for (std::size_t i = 0; i < v.size(); ++i)
                v_[i] = v[i];
        } catch (...) {
            delete[] v_;
            throw;
        }
    }

    // Operador de asignación
    auto& operator=(VectorPEL<T> const& rhs) {
        auto tmp = VectorPEL<T>{rhs};
        std::swap(v_, tmp.v_);
        std::swap(space_, tmp.space_);
        std::swap(last_, tmp.last_);
        return *this;
    }

    // Destructor
    ~VectorPEL() { delete[] v_; }

    // Funciones de capacidad
    auto size() const -> std::size_t { return space_ - v_; }
    auto capacity() const -> std::size_t { return last_ - v_; }
    auto empty() const -> bool { return v_ == space_; }

    // Funciones de acceso
    // Sin bounds checking:
    auto operator[](std::size_t i) -> T& { return v_[i]; }
    auto operator[](std::size_t i) const -> T const& { return v_[i]; }

    // Con bounds checking:
    auto at(std::size_t i) -> T& { bounds_check(i); return v_[i]; }
    auto at(std::size_t i) const -> T const& { bounds_check(i); return v_[i]; }

    // Iteradores
    auto begin() -> T* { return v_; }
    auto begin() const -> T const* { return v_; }
    auto end() -> T* { return space_; }
    auto end() const -> T const* { return space_; }

    // Función push_back
    auto push_back(T const& val) {
        if (space_ == last_) { // Capacidad agotada o primera vez que se llama
            std::size_t cp = capacity();           // Capacidad actual
            std::size_t new_cp = (cp == 0) ? 2 : 2 * cp; // Nueva capacidad
            T* new_block = new T[new_cp];          // Nuevo bloque de memoria

            try {
                for (std::size_t i = 0; i < cp; ++i)
                    new_block[i] = v_[i];
                new_block[cp] = val; // Añadimos el nuevo valor
            } catch (...) {
                delete[] new_block;
                throw;
            }

            // Reasignamos los punteros tras destruir el bloque original
            delete[] v_;
            v_ = new_block;
            space_ = new_block + cp + 1;
            last_ = new_block + new_cp;
        } else {
            *space_ = val;
            ++space_;
        }
    }
};
