#include <iostream>
#include "Dynarray.h"
#include "Vector.h"
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

// Clase bade Audio
class Audio {
private:
    string* titulo;
    string* album;
    string* grupo;
    string* genero;

public:
    // Constructor
    Audio(const string& titulo, const string& album, const string& grupo, const string& genero) {
        this->titulo = new string(titulo);
        this->album = new string(album);
        this->grupo = new string(grupo);
        this->genero = new string(genero);
    }

    // Destructor
    ~Audio() {
        delete titulo;
        delete album;
        delete grupo;
        delete genero;
    }

    // Métodos de acceso
    const string& getTitulo() const { return *titulo; }
    const string& getAlbum() const { return *album; }
    const string& getGrupo() const { return *grupo; }
    const string& getGenero() const { return *genero; }

    // Mostrar información del audio
    void mostrar() const {
        cout << "Titulo: " << *titulo
             << ", Album: " << *album
             << ", Grupo: " << *grupo
             << ", Genero: " << *genero << endl;
    }
};

class CatalogoMusica {
private:
    Dynarray<Audio*> audios;

public:
    ~CatalogoMusica() {
        for (size_t i = 0; i < audios.get_size(); ++i) {
            delete audios[i];
        }
    }

    void agregarAudio(Audio* audio) {
        audios.push_back(audio);
    }

    void mostrarTodas() const {
        if (audios.get_size() == 0) {
            cout << "El catálogo de música está vacío." << endl;
            return;
        }
        for (size_t i = 0; i < audios.get_size(); ++i) {
            audios[i]->mostrar();
        }
    }

    // Métodos de búsqueda
    void buscarPorTitulo(const string& titulo) const {
        buscar([&titulo](const Audio* a) { return a->getTitulo() == titulo; }, "titulo", titulo);
    }

    void buscarPorAlbum(const string& album) const {
        buscar([&album](const Audio* a) { return a->getAlbum() == album; }, "álbum", album);
    }

    void buscarPorGenero(const string& genero) const {
        buscar([&genero](const Audio* a) { return a->getGenero() == genero; }, "género", genero);
    }

    void buscarPorGrupo(const string& grupo) const {
        buscar([&grupo](const Audio* a) { return a->getGrupo() == grupo; }, "grupo", grupo);
    }

private:
    // Método general para búsquedas
    template<typename Func>
    void buscar(Func criterio, const string& tipo, const string& valor) const {
        bool encontrado = false;
        for (size_t i = 0; i < audios.get_size(); ++i) {
            if (criterio(audios[i])) {
                audios[i]->mostrar();
                encontrado = true;
            }
        }
        if (!encontrado) {
            cout << "No se encontró ninguna canción con " << tipo << ": " << valor << endl;
        }
    }
};



void mostrarMenuPeliculasSeries(Catalogo& catalogo) {
    int opcion;
    string titulo;
    do {
        cout << "\n--- Menu de Peliculas/Series ---\n";
        cout << "1. Buscar por titulo\n";
        cout << "2. Alquilar pelicula\n";
        cout << "3. Volver al menu principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        cin.ignore(); // Limpiar el buffer de entrada para getline
        switch (opcion) {
            case 1:
                cout << "Ingrese el titulo a buscar: ";
                getline(cin, titulo);
                catalogo.buscarPorTitulo(titulo);
                break;
            case 2:
                cout << "Ingrese el titulo de la pelicula para alquilar: ";
                getline(cin, titulo);
                catalogo.alquilarPelicula(titulo);
                break;
            case 3:
                cout << "Volviendo al menu principal...\n";
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo.\n";
        }
    } while (opcion != 3);
}

void mostrarMenuMusica(CatalogoMusica& catalogo) {
    int opcion;
    string input;

    do {
        cout << "\n--- Menu de Música ---\n";
        cout << "1. Buscar por título\n";
        cout << "2. Buscar por álbum\n";
        cout << "3. Buscar por grupo\n";
        cout << "4. Buscar por género\n";
        cout << "5. Mostrar todas las canciones\n";
        cout << "6. Volver al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        cin.ignore();  // Limpiar el buffer de entrada para getline
        switch (opcion) {
            case 1:
                cout << "Ingrese el título a buscar: ";
            getline(cin, input);
            catalogo.buscarPorTitulo(input);
            break;
            case 2:
                cout << "Ingrese el álbum a buscar: ";
            getline(cin, input);
            catalogo.buscarPorAlbum(input);
            break;
            case 3:
                cout << "Ingrese el grupo a buscar: ";
            getline(cin, input);
            catalogo.buscarPorGrupo(input);
            break;
            case 4:
                cout << "Ingrese el género a buscar: ";
            getline(cin, input);
            catalogo.buscarPorGenero(input);
            break;
            case 5:
                cout << "\n--- Lista de todas las canciones ---\n";
            catalogo.mostrarTodas();
            break;
            case 6:
                cout << "Volviendo al menú principal...\n";
            break;
            default:
                cout << "Opción no válida. Intente de nuevo.\n";
        }
    } while (opcion != 6);
}

void mostrarMenuPrincipal(Catalogo& catalogo, CatalogoMusica& catalogoMusica) {
    int opcion ;
    do {
        cout << "\n--- Menu Principal ---" << endl;
        cout << "1. Catalogo de Musica" << endl;
        cout << "2. Catalogo de Peliculas/Series" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                mostrarMenuMusica(catalogoMusica);
            break;
            case 2:
                mostrarMenuPeliculasSeries(catalogo);
            break;
            case 3:
                cout << "Saliendo del programa..." << endl;
            break;
            default:
                cout << "Opcion invalida. Por favor, intente nuevamente." << endl;
        }
    } while (opcion != 3);
}

int main() {
    Catalogo catalogo;

    // Cargar datos iniciales
    Pelicula* inception = new Pelicula(new string("Inception"), new string("Ciencia Ficcion"), new string("HD"));
    Pelicula* titanic = new Pelicula(new string("Titanic"), new string("Drama"), new string("Full HD"));
    Serie* breakingBad = new Serie(new string("Breaking Bad"), new string("Crimen"), new string("4K"));
    breakingBad->agregarEpisodio(new Pelicula(new string("Piloto"), new string("Crimen"), new string("4K")));
    breakingBad->agregarEpisodio(new Pelicula(new string("El gato esta en la bolsa"), new string("Crimen"), new string("4K")));

    catalogo.agregarPelicula(inception);
    catalogo.agregarPelicula(titanic);
    catalogo.agregarSerie(breakingBad);

    CatalogoMusica catalogoMusica;

    // Cargar datos iniciales
    Audio* starWarsTheme = new Audio("Star Wars Theme", "Star Wars Soundtrack", "John Williams", "Instrumental");
    Audio* doraemonOpening = new Audio("Doraemon Opening", "Doraemon OST", "Takumi Kusube", "Anime");
    Audio* thriller = new Audio("Thriller", "Thriller", "Michael Jackson", "Pop");

    catalogoMusica.agregarAudio(starWarsTheme);
    catalogoMusica.agregarAudio(doraemonOpening);
    catalogoMusica.agregarAudio(thriller);

    // Mostrar menú principal
    mostrarMenuPrincipal(catalogo, catalogoMusica);

    return 0;
}