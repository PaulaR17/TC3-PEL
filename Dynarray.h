template <typename T>
class Dynarray {
private:
    T* data;
    size_t size;
    size_t capacity;

    // Método privado para redimensionar el almacenamiento
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];  //asignación de nueva memoria
        for (size_t i = 0; i < size; ++i) { //copia de elementos
            new_data[i] = data[i];
        }
        delete[] data;      // Liberación de la memoria antigua
        data = new_data;    // Asignación del nuevo bloque
        capacity = new_capacity;
    }

public:
    // Constructor: inicializa el Dynarray con capacidad inicial
    Dynarray(size_t initial_capacity = 2)
        : data(new T[initial_capacity]), size(0), capacity(initial_capacity) {}

    // Destructor: libera la memoria reservada
    ~Dynarray() {
        delete[] data;
    }

    // Función para insertar un elemento al final del Dynarray
    void push_back(const T& value) {
        if (size == capacity) {           // Si se alcanza la capacidad
            resize(capacity * 2);         // Duplicar la capacidad
        }
        data[size++] = value;             // Insertar el nuevo valor
    }

    // Función para eliminar el último elemento del Dynarray
    void pop_back() {
        if (size > 0) {                   // Solo si hay elementos
            --size;                       // Decrementar el tamaño
        }
    }

    // Función para obtener el tamaño actual del Dynarray
    size_t get_size() const {
        return size;
    }

    // Función para obtener la capacidad actual del Dynarray
    size_t get_capacity() const {
        return capacity;
    }

    // Acceso a un elemento específico (con verificación de límites)
    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Índice fuera de rango");
        }
        return data[index];
    }

    // Acceso constante a un elemento específico (con verificación de límites)
    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Índice fuera de rango");
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