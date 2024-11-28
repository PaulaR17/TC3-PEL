#include <iostream>
#include "Dynarray.h"
#include "Vector.h"
using namespace std;

class Audio {
    private:
    string titulo;
    string artistas;
};

using namespace std;

// Clase base Multimedia
class Multimedia {
private:
    string* titulo;
    string* genero;
    string* calidad;
    bool disponible;

public:
    Multimedia() {
        titulo = new string("");
        genero = new string("");
        calidad = new string("");
        disponible = false;
    }

    Multimedia(string* titulo, string* genero, string* calidad) {
        this->titulo = titulo;
        this->genero = genero;
        this->calidad = calidad;
        this->disponible = true;
    }

    virtual ~Multimedia() {
        delete titulo;
        delete genero;
        delete calidad;
    }

    // Metodos Get
    const string& getTitulo() const { return *titulo; }
    const string& getGenero() const { return *genero; }
    const string& getCalidad() const { return *calidad; }
    bool isDisponible() const { return disponible; }

    void setDisponibilidad(bool estado) { disponible = estado; }

    // Metodos auxiliares
    virtual void mostrar() const {
        cout << "Titulo: " << *titulo
             << ", Genero: " << *genero
             << ", Calidad: " << *calidad
             << ", Disponible: " << (disponible ? "Si" : "No") << '\n';
    }

    virtual bool esSerie() const {
        return false; // Por defecto no es serie
    }
};

// Clase Pelicula
class Pelicula : public Multimedia {
public:
    Pelicula(string* titulo, string* genero, string* calidad)
        : Multimedia(titulo, genero, calidad) {}

    ~Pelicula() override = default;
};

// Clase Serie
class Serie : public Multimedia {
private:
    VectorPEL<Pelicula*> episodios; // Una serie es un conjunto de episodios (peliculas)

public:
    Serie(string* titulo, string* genero, string* calidad)
        : Multimedia(titulo, genero, calidad) {}

    ~Serie() override {
        for (auto& episodio : episodios) {
            delete episodio;
        }
    }

    void agregarEpisodio(Pelicula* episodio) {
        episodios.push_back(episodio);
    }

    void mostrarEpisodios() const {
        cout << "Episodios de la serie \"" << getTitulo() << endl;
        for (size_t i = 0; i < episodios.size(); ++i) {
            cout << "  Episodio " << i + 1 << ": ";
            episodios[i]->mostrar();
        }
    }

    bool esSerie() const override {
        return true; // Es una serie
    }

    void mostrar() const override {
        Multimedia::mostrar();
        mostrarEpisodios();
    }
};

// Clase Catalogo
class Catalogo {
private:
    VectorPEL<Multimedia*> items;

public:
    ~Catalogo() {
        for (auto& item : items) {
            delete item; // Liberar memoria de cada objeto Multimedia
        }
    }

    void agregarPelicula(Pelicula* pelicula) {
        items.push_back(pelicula);
    }

    void agregarSerie(Serie* serie) {
        items.push_back(serie);
    }

    void buscarPorTitulo(const string& titulo) const {
        bool encontrado = false;
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i]->getTitulo() == titulo) {
                items[i]->mostrar();
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            cout << "No se encontro una pelicula/serie con el titulo: " << titulo << endl;
        }
    }

    void alquilarPelicula(const string& titulo) {
        bool encontrado = false;
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i]->getTitulo() == titulo) {
                if (Pelicula* pelicula = dynamic_cast<Pelicula*>(items[i])) {
                    pelicula->setDisponibilidad(false);
                    cout << "La pelicula " << titulo << " ha sido alquilada."<<endl;
                    encontrado = true;
                } else {
                    cout << "El titulo " << titulo << " corresponde a una serie, no se puede alquilar como pelicula."<<endl;
                    encontrado = true;
                }
                break;
            }
        }
        if (!encontrado) {
            cout << "No se encontro una pelicula con el titulo: " << titulo << endl;
        }
    }
};

// Funcion de ejemplo
void EjemploCatalogo() {
    Catalogo catalogo;

    // Crear peliculas
    Pelicula* inception = new Pelicula(new string("Inception"), new string("Ciencia Ficcion"), new string("HD"));
    Pelicula* titanic = new Pelicula(new string("Titanic"), new string("Drama"), new string("Full HD"));

    // Crear una serie
    Serie* breakingBad = new Serie(new string("Breaking Bad"), new string("Crimen"), new string("4K"));
    breakingBad->agregarEpisodio(new Pelicula(new string("Piloto"), new string("Crimen"), new string("4K")));
    breakingBad->agregarEpisodio(new Pelicula(new string("El gato esta en la bolsa"), new string("Crimen"), new string("4K")));

    // Agregar peliculas y series al catalogo
    catalogo.agregarPelicula(inception);
    catalogo.agregarPelicula(titanic);
    catalogo.agregarSerie(breakingBad);

    // Buscar por titulo
    cout << "Buscando 'Inception'"<<endl;
    catalogo.buscarPorTitulo("Inception");

    cout << "Buscando 'Breaking Bad':"<<endl;
    catalogo.buscarPorTitulo("Breaking Bad");

    // Alquilar una pelicula
    cout << "Alquilando 'Inception: "<<endl;
    catalogo.alquilarPelicula("Inception");

    // Intentar alquilar una serie
    cout << "Intentando alquilar 'Breaking Bad':"<<endl;
    catalogo.alquilarPelicula("Breaking Bad");
}

int main()
{
    EjemploCatalogo();
    return 0;
}
