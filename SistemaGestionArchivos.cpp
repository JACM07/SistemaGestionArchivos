#include <iostream> // Librería estándar de entrada y salida
#include <string> // Librería para manejar cadenas de texto
#include <vector> // Librería para utilizar vectores dinámicos
#include <chrono> // Librería para medir el tiempo
#include <thread> // Librería para manejar hilos
#include <algorithm> // Librería para algoritmos genéricos
#include <fstream> // Librería para manejar archivos
#include <cstdlib>     // Para system
#include <cstring> // Para usar strcmp
#include <dirent.h> // Librería para manejar directorios en sistemas Unix
#ifdef _WIN32
#include <windows.h> // Librería específica de Windows para algunas operaciones de sistema
#else
#include <sys/stat.h>  // Para mkdir en sistemas Unix
#include <unistd.h> //Proporciona acceso a un conjunto de llamadas al sistema relacionadas con operaciones de entrada/salida
#endif

using namespace std;

// Definición de la estructura de un archivo
class Archivo {
public:
    string nombre; // Nombre del archivo
    string contenido; // Contenido del archivo
    string tipo;

    // Constructor de la estructura Archivo
    Archivo(const string& _nombre, const string& _tipo, const string& _contenido) : nombre(_nombre), tipo(_tipo), contenido(_contenido) {}

    // Operador de comparación <
    bool operator<(const Archivo& otroArchivo) const {
        return this->nombre < otroArchivo.nombre; // Comparación basada en el nombre del archivo
    }
};

// Implementación de la estructura de Lista utilizando vector
template<typename T, int MAX_SIZE>
class Lista {
private:
    vector<T> elementos; // Vector de elementos
    int tamano;

public:
    // Funciones miembro de la clase Lista

    // Constructor sin argumentos
    Lista() {}

    // Insertar un elemento en la lista
    void insertar(const T& elemento) {
        elementos.push_back(elemento);
    }

    // Verificar si la lista est� vac�a
    bool empty() const {
        return elementos.empty();
    }

    // Métodos para obtener iteradores al principio y al final de la lista
    typename vector<T>::iterator begin() {
        return elementos.begin();
    }

    typename vector<T>::iterator end() {
        return elementos.end();
    }

    // Métodos para obtener iteradores constantes al principio y al final de la lista
    typename vector<T>::const_iterator begin() const {
        return elementos.begin();
    }

    typename vector<T>::const_iterator end() const {
        return elementos.end();
    }

    // Obtener el tama�o de la lista
    size_t size() const {
        return elementos.size();
    }

    // Limpiar la lista
    void clear() {
        elementos.clear();
    }

    // Eliminar un elemento de la lista dado su iterador
    void erase(typename vector<T>::iterator it) {
        elementos.erase(it);
    }

    // Operador de corchetes para acceder a los elementos de la lista
    T& operator[](int index) {
        if (index < 0 || index >= tamano) {
            throw std::out_of_range("Indice fuera de rango");
        }
        return elementos[index];
    }

    Lista(typename vector<T>::iterator inicio, typename vector<T>::iterator fin) {
        elementos.assign(inicio, fin);
    }
};

// Implementación de la estructura de Pila utilizando vector
template<typename T, int MAX_SIZE>
class Pila {
private:
    vector<T> elementos; // Vector de elementos

public:
    // Funciones miembro de la clase Pila

    // Insertar un elemento en la pila
    void push(const T& elemento) {
        elementos.push_back(elemento);
    }

    // Eliminar el elemento superior de la pila
    void pop() {
        if (!empty()) {
            elementos.pop_back();
        } else {
            cout << "La pila esta vacia. No se puede realizar la operacion pop." << endl;
        }
    }

    // Verificar si la pila está vacía
    bool empty() const {
        return elementos.empty();
    }

    // Obtener una referencia al elemento superior de la pila
    const T& top() const {
        return elementos.back();
    }
};

// Implementación de la estructura de Cola utilizando vector
template<typename T, int MAX_SIZE>
class Cola {
private:
    vector<T> elementos; // Vector de elementos

public:
    // Funciones miembro de la clase Cola

    // Encolar un elemento
    void encolar(const T& elemento) {
        elementos.push_back(elemento);
    }

    // Desencolar el elemento frontal
    void desencolar() {
        if (!empty()) {
            elementos.erase(elementos.begin());
        } else {
            cout << "La cola esta vacia. No se puede realizar la operacion desencolar." << endl;
        }
    }

    // Verificar si la cola está vacía
    bool empty() const {
        return elementos.empty();
    }

    // Obtener una referencia al elemento frontal de la cola
    const T& frente() const {
        return elementos.front();
    }
};

    // Definición de la estructura del nodo del árbol binario de búsqueda
class NodoABB {
public:
    string nombre;               // Nombre del directorio o archivo representado por este nodo
    bool esDirectorio;           // Indica si el nodo representa un directorio (true) o un archivo (false)
    NodoABB* izquierda;          // Puntero al nodo hijo izquierdo
    NodoABB* derecha;            // Puntero al nodo hijo derecho
    vector<Archivo*> archivos;   // Vector que contiene los archivos en caso de que el nodo represente un directorio

    // Constructor de la clase NodoABB
    NodoABB(const string& nombre, bool esDirectorio) : nombre(nombre), esDirectorio(esDirectorio), izquierda(nullptr), derecha(nullptr) {}
};

// Clase ABB (Árbol Binario de Búsqueda)
class ABB {
public:
    NodoABB* raiz;  // Puntero al nodo raíz del árbol

    // Constructor de la clase ABB
    ABB() : raiz(nullptr) {}

    // Destructor de la clase ABB
    ~ABB() {
        destruirArbol(raiz); // Libera la memoria ocupada por todos los nodos del árbol
    }

    // Método para insertar un directorio en el árbol
    void insertarDirectorioEnArbol(const string& nombreDirectorio) {
        raiz = insertarDirectorio(raiz, nombreDirectorio);
    }

    // Método para insertar un archivo en un directorio del árbol
    void insertarArchivoEnArbol(const string& nombreDirectorio, const string& nombreArchivo, Archivo* archivo) {
        insertarArchivoEnArbol(raiz, nombreDirectorio, nombreArchivo, archivo);
    }

    // Método para eliminar un nodo del árbol
    NodoABB* eliminarNodo(NodoABB* nodo, const string& nombre) {
        if (nodo == nullptr) {
            return nullptr;
        }

        if (nombre < nodo->nombre) {
            nodo->izquierda = eliminarNodo(nodo->izquierda, nombre);
        } else if (nombre > nodo->nombre) {
            nodo->derecha = eliminarNodo(nodo->derecha, nombre);
        } else {
            // Nodo encontrado, proceder con la eliminación
            if (nodo->izquierda == nullptr) {
                NodoABB* temp = nodo->derecha;
                delete nodo;
                return temp;
            } else if (nodo->derecha == nullptr) {
                NodoABB* temp = nodo->izquierda;
                delete nodo;
                return temp;
            }

            // Nodo con dos hijos, obtener el sucesor en inorden
            NodoABB* temp = nodo->derecha;
            while (temp->izquierda != nullptr) {
                temp = temp->izquierda;
            }

            // Copiar los datos del sucesor en inorden al nodo actual
            nodo->nombre = temp->nombre;
            nodo->esDirectorio = temp->esDirectorio;
            nodo->archivos = temp->archivos;

            // Eliminar el sucesor en inorden
            nodo->derecha = eliminarNodo(nodo->derecha, temp->nombre);
        }

        return nodo;
    }

    // Método para mostrar los directorios y archivos utilizando recorrido inorden
    void mostrarInorden() {
        mostrarInorden(raiz);
        cout << endl;
    }

    // Nuevo método para mostrar los directorios con sus archivos
    void mostrarDirectoriosYArchivos() {
        mostrarInorden(raiz);
    }

private:
    // Método privado para insertar un directorio en el árbol
    NodoABB* insertarDirectorio(NodoABB* nodo, const string& nombreDirectorio) {
        if (nodo == nullptr) {
            return new NodoABB(nombreDirectorio, true); // Crear un nuevo nodo si el nodo actual es nulo
        }

        if (nombreDirectorio < nodo->nombre) {
            nodo->izquierda = insertarDirectorio(nodo->izquierda, nombreDirectorio); // Insertar en el subárbol izquierdo
        } else if (nombreDirectorio > nodo->nombre) {
            nodo->derecha = insertarDirectorio(nodo->derecha, nombreDirectorio); // Insertar en el subárbol derecho
        }

        return nodo;
    }

    // Método privado para insertar un archivo en un directorio del árbol
    void insertarArchivoEnArbol(NodoABB* nodo, const string& nombreDirectorio, const string& nombreArchivo, Archivo* archivo) {
        if (nodo == nullptr) {
            cout << "Error: No se encontró el directorio \"" << nombreDirectorio << "\"" << endl;
            return;
        }

        if (nodo->nombre == nombreDirectorio) {
            // Verificar si el archivo ya existe en el directorio
            for (auto existingArchivo : nodo->archivos) {
                if (existingArchivo->nombre == nombreArchivo) {
                    return;
                }
            }

            // Si el archivo no existe, agregarlo al directorio
            nodo->archivos.push_back(archivo);
            return;
        }

        // Recorrer el árbol de manera recursiva para encontrar el directorio
        if (nombreDirectorio < nodo->nombre) {
            insertarArchivoEnArbol(nodo->izquierda, nombreDirectorio, nombreArchivo, archivo); // Buscar en el subárbol izquierdo
        } else {
            insertarArchivoEnArbol(nodo->derecha, nombreDirectorio, nombreArchivo, archivo); // Buscar en el subárbol derecho
        }
    }

    // Método privado para liberar la memoria ocupada por los nodos del árbol
    void destruirArbol(NodoABB* nodo) {
        if (nodo == nullptr) {
            return;
        }

        destruirArbol(nodo->izquierda); // Liberar memoria del subárbol izquierdo
        destruirArbol(nodo->derecha);   // Liberar memoria del subárbol derecho
        delete nodo;                    // Liberar memoria del nodo actual
    }

    // Método privado que implementa el algoritmo de ordenamiento selection sort para ordenar los archivos en un directorio
    void ordenarArchivosEnDirectorio(NodoABB* nodo) {
        if (nodo == nullptr) {
            return; // Salir si el nodo es nulo
        }

        ordenarArchivosEnDirectorio(nodo->izquierda); // Ordenar archivos en el subárbol izquierdo

        // Verificar si el nodo es un directorio y ordenar los archivos en orden alfabético
        if (nodo->esDirectorio) {
            sort(nodo->archivos.begin(), nodo->archivos.end(), [](const Archivo* a, const Archivo* b) {
                return a->nombre < b->nombre; // Comparar los nombres de los archivos
            });
        }

        ordenarArchivosEnDirectorio(nodo->derecha); // Ordenar archivos en el subárbol derecho
    }

    // Método privado para mostrar los directorios con sus archivos utilizando recorrido inorden
    void mostrarInorden(NodoABB* nodo) {
        if (nodo == nullptr) {
            return; // Salir si el nodo es nulo
        }

        mostrarInorden(nodo->izquierda); // Recorrer el subárbol izquierdo

        // Mostrar el directorio actual y sus archivos (si es un directorio)
        if (nodo->esDirectorio) {
            cout << "- " << nodo->nombre << " (Directorio):\n";
            ordenarArchivosEnDirectorio(nodo); // Ordenar los archivos en este directorio
            for (auto archivo : nodo->archivos) {
                cout << "  - " << archivo->nombre << "\n"; // Mostrar los archivos ordenados
            }
        } else {
            cout << "  - " << nodo->nombre; // Mostrar el nombre del archivo
        }

        mostrarInorden(nodo->derecha); // Recorrer el subárbol derecho
    }
};

    // Definición de la estructura del nodo del Árbol AVL
class NodoAVL {
public:
    string nombre;               // Nombre del directorio o archivo representado por este nodo
    string tipoArchivo;          // Tipo de archivo
    vector<Archivo*> archivos;   // Vector que contiene los archivos en caso de que el nodo represente un directorio
    int altura;                  // Altura del nodo en el árbol
    NodoAVL* izquierda;          // Puntero al hijo izquierdo
    NodoAVL* derecha;            // Puntero al hijo derecho

    // Constructor de la clase NodoAVL
    NodoAVL(const string& _nombre, const string& _tipoArchivo, Archivo* _archivo) : nombre(_nombre), tipoArchivo(_tipoArchivo), altura(1), izquierda(nullptr), derecha(nullptr) {
        archivos.push_back(_archivo); // Insertar el archivo en el vector de archivos del nodo
    }
};

// Clase AVL (Árbol AVL)
class AVL {
public:
    // Constructor de la clase AVL
    AVL() : raiz(nullptr) {}

    // Destructor de la clase AVL
    ~AVL() {
        destruirArbol(raiz); // Liberar la memoria ocupada por todos los nodos del árbol
    }

    // Método para insertar un nodo en el árbol AVL
    void insertar(const string& nombre, const string& tipoArchivo, Archivo* archivo) {
        raiz = insertar(raiz, nombre, tipoArchivo, archivo); // Llamar a la función privada de inserción
    }

    // Método para buscar un nodo en el árbol AVL
    NodoAVL* buscar(const string& nombre) {
        return buscar(raiz, nombre); // Llamar a la función privada de búsqueda
    }

    // Método para mostrar el contenido del árbol AVL en orden
    void mostrarInorden() {
        mostrarInorden(raiz); // Llamar a la función privada de recorrido inorden
        cout << endl;
    }

    // Método para obtener la raíz del árbol AVL
    NodoAVL* obtenerRaiz() {
        return raiz; // Devolver el puntero a la raíz del árbol
    }

    // Método para buscar archivos por tipo en el árbol AVL
    vector<Archivo*> buscarArchivosPorTipo(const string& tipoArchivo) {
        return buscarArchivosPorTipoPrivado(raiz, tipoArchivo); // Llamar a la función privada de búsqueda por tipo
    }

private:
    NodoAVL* raiz;  // Puntero al nodo raíz del árbol
    // Función privada para calcular la altura de un nodo
    int altura(NodoAVL* nodo) {
        if (nodo == nullptr) {
            return 0; // Si el nodo es nulo, la altura es 0
        }
        return nodo->altura; // Devolver la altura del nodo
    }

    // Función privada para calcular el factor de balance de un nodo
    int factorBalance(NodoAVL* nodo) {
        return altura(nodo->derecha) - altura(nodo->izquierda); // Calcular la diferencia de alturas entre los hijos derecho e izquierdo
    }

    // Función privada para realizar una rotación a la izquierda
    NodoAVL* rotarIzquierda(NodoAVL* nodo) {
        // Realizar la rotación
        NodoAVL* nodoDerecho = nodo->derecha;
        nodo->derecha = nodoDerecho->izquierda;
        nodoDerecho->izquierda = nodo;

        // Actualizar las alturas de los nodos afectados por la rotación
        nodoDerecho->altura = max(altura(nodoDerecho->izquierda), altura(nodoDerecho->derecha)) + 1;
        nodo->altura = max(altura(nodo->izquierda), altura(nodo->derecha)) + 1;

        return nodoDerecho; // Devolver el nuevo nodo raíz después de la rotación
    }

    // Función privada para realizar una rotación a la derecha
    NodoAVL* rotarDerecha(NodoAVL* nodo) {
        // Realizar la rotación
        NodoAVL* nodoIzquierdo = nodo->izquierda;
        nodo->izquierda = nodoIzquierdo->derecha;
        nodoIzquierdo->derecha = nodo;

        // Actualizar las alturas de los nodos afectados por la rotación
        nodoIzquierdo->altura = max(altura(nodoIzquierdo->izquierda), altura(nodoIzquierdo->derecha)) + 1;
        nodo->altura = max(altura(nodo->izquierda), altura(nodo->derecha)) + 1;

        return nodoIzquierdo; // Devolver el nuevo nodo raíz después de la rotación
    }

    // Función privada para realizar una rotación a la izquierda seguida de una rotación a la derecha
    NodoAVL* rotarIzquierdaDerecha(NodoAVL* nodo) {
        nodo->izquierda = rotarIzquierda(nodo->izquierda); // Realizar rotación a la izquierda en el hijo izquierdo
        return rotarDerecha(nodo); // Realizar rotación a la derecha en el nodo original
    }

    // Función privada para realizar una rotación a la derecha seguida de una rotación a la izquierda
    NodoAVL* rotarDerechaIzquierda(NodoAVL* nodo) {
        nodo->derecha = rotarDerecha(nodo->derecha); // Realizar rotación a la derecha en el hijo derecho
        return rotarIzquierda(nodo); // Realizar rotación a la izquierda en el nodo original
    }

    // Función privada para equilibrar un nodo después de la inserción
    NodoAVL* equilibrar(NodoAVL* nodo) {
        // Actualizar la altura del nodo
        nodo->altura = 1 + max(altura(nodo->izquierda), altura(nodo->derecha));

        // Verificar el factor de balance y realizar las rotaciones necesarias para equilibrar el nodo
        if (factorBalance(nodo) == 2) {
            if (factorBalance(nodo->derecha) < 0) {
                nodo->derecha = rotarDerecha(nodo->derecha);
            }
            return rotarIzquierda(nodo);
        }
        if (factorBalance(nodo) == -2) {
            if (factorBalance(nodo->izquierda) > 0) {
                nodo->izquierda = rotarIzquierda(nodo->izquierda);
            }
            return rotarDerecha(nodo);
        }
        return nodo; // Devolver el nodo equilibrado
    }

    // Función privada para insertar un nodo en el árbol AVL
    NodoAVL* insertar(NodoAVL* nodo, const string& nombre, const string& tipoArchivo, Archivo* archivo) {
        // Insertar el nodo de manera recursiva
        if (nodo == nullptr) {
            return new NodoAVL(nombre, tipoArchivo, archivo); // Caso base: nodo nulo, crear un nuevo nodo
        }
        if (nombre < nodo->nombre) {
            nodo->izquierda = insertar(nodo->izquierda, nombre, tipoArchivo, archivo); // Insertar en el subárbol izquierdo
        } else if (nombre > nodo->nombre) {
            nodo->derecha = insertar(nodo->derecha, nombre, tipoArchivo, archivo); // Insertar en el subárbol derecho
        } else {
            nodo->archivos.push_back(archivo); // El nodo ya existe, agregar el archivo al vector de archivos
        }

        return equilibrar(nodo); // Equilibrar el árbol después de la inserción
    }

    // Función privada para buscar un nodo en el árbol AVL
    NodoAVL* buscar(NodoAVL* nodo, const string& nombre) {
        if (nodo == nullptr || nombre == nodo->nombre) {
            return nodo; // Caso base: nodo nulo o nombre encontrado, devolver el nodo actual
        }
        if (nombre < nodo->nombre) {
            return buscar(nodo->izquierda, nombre); // Buscar en el subárbol izquierdo
        } else {
            return buscar(nodo->derecha, nombre); // Buscar en el subárbol derecho
        }
    }

    // Función privada para buscar archivos por tipo en el árbol AVL
    vector<Archivo*> buscarArchivosPorTipoPrivado(NodoAVL* nodo, const string& tipo) {
        vector<Archivo*> archivosEncontrados;

        if (nodo == nullptr) {
            return archivosEncontrados; // Caso base: nodo nulo, devolver vector vacío
        }

        // Buscar recursivamente en los subárboles izquierdo y derecho
        vector<Archivo*> archivosIzquierda = buscarArchivosPorTipoPrivado(nodo->izquierda, tipo);
        vector<Archivo*> archivosDerecha = buscarArchivosPorTipoPrivado(nodo->derecha, tipo);

        // Agregar los archivos encontrados en los subárboles izquierdo y derecho al vector de archivos
        archivosEncontrados.insert(archivosEncontrados.end(), archivosIzquierda.begin(), archivosIzquierda.end());
        archivosEncontrados.insert(archivosEncontrados.end(), archivosDerecha.begin(), archivosDerecha.end());

        // Agregar los archivos del nodo actual al vector si coinciden con el tipo buscado
        for (Archivo* archivo : nodo->archivos) {
            if (archivo->tipo == tipo) {
                archivosEncontrados.push_back(archivo);
            }
        }

        return archivosEncontrados; // Devolver el vector de archivos encontrados
    }

    // Función privada para liberar la memoria ocupada por los nodos del árbol AVL
    void destruirArbol(NodoAVL* nodo) {
        if (nodo == nullptr) {
            return; // Caso base: nodo nulo, no hacer nada
        }

        // Liberar la memoria de los subárboles izquierdo y derecho
        destruirArbol(nodo->izquierda);
        destruirArbol(nodo->derecha);

        // Liberar la memoria ocupada por los archivos del nodo actual
        for (auto archivo : nodo->archivos) {
            delete archivo;
        }

        // Liberar la memoria ocupada por el nodo actual
        delete nodo;
    }

    // Función privada para realizar un recorrido inorden del árbol AVL
    void mostrarInorden(NodoAVL* nodo) {
        if (nodo == nullptr) {
            return; // Caso base: nodo nulo, no hacer nada
        }

        // Realizar un recorrido inorden recursivo en los subárboles izquierdo y derecho
        mostrarInorden(nodo->izquierda);
        cout << "- " << nodo->nombre << " (" << nodo->archivos.size() << "): "; // Mostrar el nombre del nodo y el número de archivos
        for (auto archivo : nodo->archivos) {
            cout << archivo->nombre << " "; // Mostrar el nombre de cada archivo
        }
        mostrarInorden(nodo->derecha);
    }
};

// Definición de la estructura de un directorio que contiene archivos
class Directorio {
public:
    string nombre; // Nombre del directorio
    vector<Archivo*> archivos; // Vector de punteros a archivos
    string contenido; // Contenido del directorio (no utilizado en esta implementaci�n)
    bool conectado;
    vector<Directorio*> directoriosConectados;

    // Constructores de la estructura Directorio
    Directorio(const string& _nombre) : nombre(_nombre) {} // Constructor b�sico
    Directorio(const string& _nombre, const vector<Archivo*>& _archivos, const string& _contenido)
        : nombre(_nombre), archivos(_archivos), contenido(_contenido) {} // Constructor con par�metros

    // Sobrecarga del operador < para comparar Directorios
    bool operator<(const Directorio& otroDirectorio) const {
        return this->nombre < otroDirectorio.nombre; // O cualquier otro criterio de comparación deseado
    }

    // Destructor para liberar la memoria de los archivos
    ~Directorio() {
        for (auto archivo : archivos) {
            delete archivo;
        }
    }
};

    // Definición de la clase NodoGrafo
class NodoGrafo {
public:
    string nombre; // Nombre del nodo
    vector<string> adyacentes; // Nombres de los nodos adyacentes
    bool visited; // Estado de visita del nodo

    // Constructor de la clase
    NodoGrafo(const string& _nombre) : nombre(_nombre), visited(false) {}
};

// Clase Grafo (Grafo no dirigido)
class Grafo {
public:
    vector<NodoGrafo> listaNodos; // Lista de nodos en el grafo
    vector<Directorio*> listaDirectorios; // Lista de directorios asociados al grafo

    // Constructor de la clase
    Grafo() {}

    // Destructor de la clase
    ~Grafo() {}

    // Método para agregar un nuevo nodo al grafo
    void agregarNodo(const string& nombre) {
        listaNodos.emplace_back(nombre);
    }

    // Método para buscar un nodo en el grafo por su nombre
    NodoGrafo* buscarNodo(const string& nombre) {
        auto iterNodo = std::find_if(listaNodos.begin(), listaNodos.end(), [&](const NodoGrafo& nodo) {
            return nodo.nombre == nombre;
        });

        return (iterNodo != listaNodos.end()) ? &(*iterNodo) : nullptr;
    }

    // Método para conectar dos nodos en el grafo
    void conectar(NodoGrafo* nodo1, NodoGrafo* nodo2) {
        nodo1->adyacentes.push_back(nodo2->nombre);
        nodo2->adyacentes.push_back(nodo1->nombre);
    }

    // Método para mostrar la estructura del grafo
    void mostrarGrafo() {
        for (const auto& nodo : listaNodos) {
            cout << "- " << nodo.nombre << ": ";
            for (const auto& adyacente : nodo.adyacentes) {
                cout << adyacente << " ";
            }
            cout << endl;
        }
    }

    // Método para realizar búsqueda en amplitud (BFS) en el grafo
    bool busquedaEnAnchura(const string& nombreArchivo) {
        Cola<NodoGrafo*, 50> cola;
        for (auto& nodo : listaNodos) {
            nodo.visited = false;
        }

        bool encontrado = false; // Indica si el archivo ha sido encontrado

        for (auto& nodo : listaNodos) {
            if (!nodo.visited) {
                cola.encolar(&nodo);
                while (!cola.empty()) {
                    NodoGrafo* actual = cola.frente();
                    cola.desencolar();
                    if (actual->nombre == nombreArchivo) {
                        encontrado = true;
                    }
                    actual->visited = true;
                    for (const auto& adyacente : actual->adyacentes) {
                        auto* ady = buscarNodo(adyacente);
                        if (ady && !ady->visited) {
                            cola.encolar(ady);
                        }
                    }
                }
            }
        }
        return encontrado; // Devuelve si el archivo ha sido encontrado o no
    }

    // Método para realizar búsqueda en profundidad (DFS) en el grafo
    bool busquedaEnProfundidad(const string& nombreArchivo) {
        Pila<NodoGrafo*, 50> pila;
        for (auto& nodo : listaNodos) {
            nodo.visited = false;
        }

        bool encontrado = false; // Indica si el archivo ha sido encontrado

        for (auto& nodo : listaNodos) {
            if (!nodo.visited) {
                pila.push(&nodo);
                while (!pila.empty()) {
                    NodoGrafo* actual = pila.top();
                    pila.pop();
                    if (actual->nombre == nombreArchivo) {
                        encontrado = true;
                    }
                    actual->visited = true;
                    for (const auto& adyacente : actual->adyacentes) {
                        auto* ady = buscarNodo(adyacente);
                        if (ady && !ady->visited) {
                            pila.push(ady);
                        }
                    }
                }
            }
        }
        return encontrado; // Devuelve si el archivo ha sido encontrado o no
    }

    // Método utilitario para búsqueda en profundidad (DFS)
    bool dfsUtil(NodoGrafo& nodo, const string& nombreArchivo) {
        nodo.visited = true;
        if (nodo.nombre == nombreArchivo) {
            return true; // Archivo encontrado
        }
        for (const auto& adyacente : nodo.adyacentes) {
            auto* ady = buscarNodo(adyacente);
            if (ady && !ady->visited) {
                if (dfsUtil(*ady, nombreArchivo)) {
                    return true; // Archivo encontrado
                }
            }
        }
        return false; // Archivo no encontrado
    }

    // Método para obtener el camino hasta un archivo en el grafo
    vector<string> obtenerCaminoHastaArchivo(const string& nombreArchivo) {
        vector<string> camino;
        for (auto& nodo : listaNodos) {
            nodo.visited = false;
        }
        for (auto& nodo : listaNodos) {
            if (!nodo.visited) {
                if (obtenerCaminoUtil(nodo, nombreArchivo, camino)) {
                    return camino; // Retorna el camino al archivo
                }
            }
        }
        return {}; // Archivo no encontrado
    }

    // Método utilitario para obtener el camino hasta un archivo en el grafo
    bool obtenerCaminoUtil(NodoGrafo& nodo, const string& nombreArchivo, vector<string>& camino) {
        nodo.visited = true;
        camino.push_back(nodo.nombre);
        if (nodo.nombre == nombreArchivo) {
            return true; // Archivo encontrado
        }
        for (const auto& adyacente : nodo.adyacentes) {
            auto* ady = buscarNodo(adyacente);
            if (ady && !ady->visited) {
                if (obtenerCaminoUtil(*ady, nombreArchivo, camino)) {
                    return true; // Archivo encontrado
                }
            }
        }
        camino.pop_back();
        return false; // Archivo no encontrado
    }

    // Método para construir un directorio a partir de un camino en el grafo
    string construirDirectorioDesdeCamino(const vector<string>& camino) {
        string directorio;
        for (const auto& nodo : camino) {
            directorio += "/" + nodo;
        }
        return directorio;
    }

    // Método para limpiar el grafo
    void limpiarGrafo() {
        listaNodos.clear();
        listaDirectorios.clear();
    }

    // Método para agregar una relación entre dos nodos en el grafo
    void agregarRelacion(const string& nodo1, const string& nodo2) {
        NodoGrafo* ptrNodo1 = buscarNodo(nodo1);
        NodoGrafo* ptrNodo2 = buscarNodo(nodo2);
        if (ptrNodo1 && ptrNodo2) {
            conectar(ptrNodo1, ptrNodo2);
        } else {
            // Manejar el caso en el que uno o ambos nodos no se encuentran en el grafo
            cout << "Error: Al menos uno de los nodos no se encuentra en el grafo." << endl;
        }
    }

    // Método para actualizar el grafo desde una lista de directorios
    void actualizarDesdeLista(const Lista<Directorio, 50>& listaDirectorios) {
        // Limpiar el grafo actual
        this->limpiarGrafo();

        // Iterar sobre la lista de directorios
        for (const auto& directorio : listaDirectorios) {
            // Agregar un nodo para el directorio actual
            this->agregarNodo(directorio.nombre);

            // Iterar sobre los archivos en el directorio y agregar nodos para ellos
            for (const auto& archivo : directorio.archivos) {
                this->agregarNodo(archivo->nombre);
                // Establecer una relación entre el directorio y el archivo
                this->agregarRelacion(directorio.nombre, archivo->nombre);
            }
        }
    }
};

// Función para limpiar pantalla y regresar al menú principal
void limpiarPantalla() {
    cout << "\nRegresando al menu principal";
    for(int i = 0; i < 3; ++i) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << endl;
    system("clear || cls"); // Limpiar la pantalla (compatible con sistemas Unix y Windows)
}

// Función para crear un directorio si no existe
bool crearDirectorio(const string& nombreDirectorio) {
    #ifdef _WIN32
    if (mkdir(nombreDirectorio.c_str()) == 0) {
        cout << "Directorio \"" << nombreDirectorio << "\" creado exitosamente." << endl;

        return true;
    } else {
        cout << "Error al crear el directorio \"" << nombreDirectorio << "\"." << endl;
        return false;
    }
    #else
    if (mkdir(nombreDirectorio.c_str(), 0777) == 0) {
        cout << "Directorio \"" << nombreDirectorio << "\" creado exitosamente." << endl;
        return true;
    } else {
        cout << "Error al crear el directorio \"" << nombreDirectorio << "\"." << endl;
        return false;
    }
    #endif
}

// Búsqueda lineal cuando la lista de directorios es pequeña
template<typename T, int MAX_SIZE>
T* busquedaLineal(Lista<T, MAX_SIZE>& lista, const string& nombreDirectorio) {
    for (auto& directorio : lista) {
        if (directorio.nombre == nombreDirectorio) {
            return &directorio; // Se encontró el directorio
        }
    }
    return nullptr; // No se encontró el directorio
}


// Búsqueda binaria en la lista de directorios cuando la lista sobrepasa el umbral que definimos para la búsqueda lineal
template<typename T, int MAX_SIZE>
T* busquedaBinaria(Lista<T, MAX_SIZE>& lista, const string& nombreDirectorio) {
    int inicio = 0;
    int fin = lista.size() - 1;

    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;
        if (lista[medio].nombre == nombreDirectorio) {
            cout << "Directorio encontrado en la posicion (donde 0 es el directorio menor en orden alfabetico): " << medio << endl; // Mensaje de depuración
            return &lista[medio]; // Se encontró el directorio
        }
        if (lista[medio].nombre < nombreDirectorio) {
            inicio = medio + 1; // Si el nombre buscado es mayor, buscar en la mitad derecha
        } else {
            fin = medio - 1; // Si el nombre buscado es menor, buscar en la mitad izquierda
        }
    }
    return nullptr; // No se encontró el directorio
}


// Ordenamiento iterativo de una lista de archivos (utiliza el algoritmo de seleccion)
template<typename T, int MAX_SIZE>
void ordenamientoIterativo(Lista<T, MAX_SIZE>& lista) {
    for (auto it = lista.begin(); it != lista.end(); ++it) {
        sort(it->archivos.begin(), it->archivos.end(), [](const Archivo* a, const Archivo* b) {
            return a->nombre < b->nombre;
        });
    }
}

// Función auxiliar para el ordenamiento recursivo (mezcla dos vectores ordenados)
vector<Archivo*> mezclarVectores(const vector<Archivo*>& left, const vector<Archivo*>& right) {
    vector<Archivo*> result;
    size_t leftIdx = 0, rightIdx = 0;
    while (leftIdx < left.size() && rightIdx < right.size()) {
        if (left[leftIdx]->nombre < right[rightIdx]->nombre) {
            result.push_back(left[leftIdx++]);
        } else {
            result.push_back(right[rightIdx++]);
        }
    }
    while (leftIdx < left.size()) {
        result.push_back(left[leftIdx++]);
    }
    while (rightIdx < right.size()) {
        result.push_back(right[rightIdx++]);
    }
    return result;
}

// Ordenamiento recursivo de una lista de archivos (utiliza el algoritmo de merge sort)
template<typename T, int MAX_SIZE>
void ordenamientoRecursivo(Lista<T, MAX_SIZE>& lista) {
    if (lista.size() <= 1) {
        return;
    }

    auto mitad = lista.begin() + lista.size() / 2;
    Lista<T, MAX_SIZE> izquierda(lista.begin(), mitad);
    Lista<T, MAX_SIZE> derecha(mitad, lista.end());

    ordenamientoRecursivo(izquierda);
    ordenamientoRecursivo(derecha);

    auto iterIzquierda = izquierda.begin();
    auto iterDerecha = derecha.begin();
    auto iterLista = lista.begin();

    while (iterIzquierda != izquierda.end() && iterDerecha != derecha.end()) {
        if (*iterIzquierda < *iterDerecha) {
            *iterLista = *iterIzquierda;
            ++iterIzquierda;
        } else {
            *iterLista = *iterDerecha;
            ++iterDerecha;
        }
        ++iterLista;
    }

    // Agregar los elementos restantes de la parte izquierda, si los hay
    while (iterIzquierda != izquierda.end()) {
        *iterLista = *iterIzquierda;
        ++iterIzquierda;
        ++iterLista;
    }

    // Agregar los elementos restantes de la parte derecha, si los hay
    while (iterDerecha != derecha.end()) {
        *iterLista = *iterDerecha;
        ++iterDerecha;
        ++iterLista;
    }
}

// Función para liberar la memoria de una lista de archivos
template<typename T, int MAX_SIZE>
void liberarMemoria(Lista<T, MAX_SIZE>& lista) {
    for (auto& elemento : lista) {
        for (auto archivo : elemento.archivos) {
            delete archivo;
        }
        elemento.archivos.clear();
    }
    lista.clear();
}

// Función para eliminar un directorio (compatible con Windows y Mac/Linux)
template<int MAX_SIZE>
bool eliminarDirectorio(const string& path, Lista<Directorio, MAX_SIZE>& listaDirectorios, ABB& arbolABB) {
#ifdef _WIN32 // Implementación para Windows

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }

    do {
        if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
            string filePath = path + "\\" + findData.cFileName;
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (!eliminarDirectorio(filePath, listaDirectorios, arbolABB)) { // Modificación aquí
                    FindClose(hFind);
                    return false;
                }
            } else {
                if (!DeleteFile(filePath.c_str())) {
                    FindClose(hFind);
                    return false;
                }
            }
        }
    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);

    auto iter = find_if(listaDirectorios.begin(), listaDirectorios.end(), [&](const Directorio& dir) {
        return dir.nombre == path;
    });
    if (iter != listaDirectorios.end()) {
        listaDirectorios.erase(iter);
    }

    arbolABB.raiz = arbolABB.eliminarNodo(arbolABB.raiz, path); // Modificación aquí
    if (!RemoveDirectory(path.c_str())) {
        return false;
    }

    return true;

#else // Implementación para Mac/Linux
    DIR* dir = opendir(path.c_str());
    if (dir == nullptr) {
        return false;
    }

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            string filePath = path + "/" + entry->d_name;
            if (entry->d_type == DT_DIR) {
                if (!eliminarDirectorio(filePath, listaDirectorios, arbolABB)) { // Modificación aquí
                    closedir(dir);
                    return false;
                }
            } else {
                if (remove(filePath.c_str()) != 0) {
                    closedir(dir);
                    return false;
                }
            }
        }
    }
    closedir(dir);

    auto iter = find_if(listaDirectorios.begin(), listaDirectorios.end(), [&](const Directorio& dir) {
        return dir.nombre == path;
    });
    if (iter != listaDirectorios.end()) {
        listaDirectorios.erase(iter);
    }
    arbolABB.raiz = arbolABB.eliminarNodo(arbolABB.raiz, path); // Modificación aquí
    if (rmdir(path.c_str()) != 0) {
        return false;
    }

    return true;
#endif
}


// Función principal (main)
int main() {
    // Declaración de las estructuras de datos principales
    Lista<Directorio, 50> listaDirectorios;  // Lista de directorios
    Pila<Directorio, 50> pilaDirectorios;    // Pila de directorios
    Cola<Directorio, 50> colaDirectorios;    // Cola de directorios
    ABB arbolABB;
    AVL arbolAVL;
    Grafo grafo;

    // Variable para almacenar la opción del usuario
    int opcion;

    // Bucle principal del programa
    do {
        // Menú principal
        cout << "\n----- Sistema de Gestion de Archivos -----\n";
        cout << "1. Crear un nuevo directorio\n";
        cout << "2. Crear un nuevo archivo\n";
        cout << "3. Mostrar archivos y directorios\n";
        cout << "4. Buscar archivo por nombre\n";
        cout << "5. Buscar por tipo de archivo\n";
        cout << "6. Buscar un directorio en el sistema\n";
        cout << "7. Salir\n";
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        cin.ignore(); // Limpiar el buffer de entrada

        // Switch para manejar las opciones del usuario
        switch (opcion) {
            case 1: {
                // Crear un nuevo directorio
                string nombreDirectorio;
                cout << "\nIngrese el nombre del directorio: ";
                getline(cin, nombreDirectorio);
                Directorio* nuevoDirectorio = new Directorio(nombreDirectorio);
                if (crearDirectorio(nombreDirectorio)) {
                    listaDirectorios.insertar(*nuevoDirectorio);
                    pilaDirectorios.push(*nuevoDirectorio);
                    colaDirectorios.encolar(*nuevoDirectorio);
                    grafo.agregarNodo(nombreDirectorio);
                }

                limpiarPantalla();
                break;
            }

            case 2: {
                // Crear un nuevo archivo
                if (!listaDirectorios.empty()) {
                // Mostrar los directorios disponibles
                cout << "\nDirectorios disponibles:\n";
                for (const auto& directorio : listaDirectorios) {
                    cout << "- " << directorio.nombre << endl;
                }

                // Solicitar al usuario que seleccione un directorio existente
                string nombreDirectorio;
                cout << "\nSeleccione el directorio donde desea crear el archivo: ";
                getline(cin, nombreDirectorio);

                // Buscar el directorio en la lista
                auto iterDirectorio = find_if(listaDirectorios.begin(), listaDirectorios.end(), [&](const Directorio& dir) {
                    return dir.nombre == nombreDirectorio;
                });

                // Verificar si se encontró el directorio
                if (iterDirectorio != listaDirectorios.end()) {
                    // Pedir al usuario el nombre, tipo y contenido del archivo
                    string nombreArchivo, tipoArchivo, contenidoArchivo;
                    cout << "Ingrese el nombre del archivo: ";
                    getline(cin, nombreArchivo);
                    cout << "Ingrese el tipo del archivo: ";
                    getline(cin, tipoArchivo);
                    cout << "Ingrese el contenido del archivo (escriba 'END' en una linea nueva para terminar):\n";

                    // Lectura del contenido del archivo hasta que se ingrese "END"
                    string linea;
                    while (getline(cin, linea)) {
                        // Eliminar los caracteres de nueva línea al final de la entrada
                        linea.erase(std::remove(linea.begin(), linea.end(), '\n'), linea.end());

                        if (linea == "END") {
                            break;
                        } else {
                            contenidoArchivo += linea + '\n';
                        }
                    }

                // Crear el archivo y agregarlo al directorio
                Archivo* nuevoArchivo = new Archivo(nombreArchivo, tipoArchivo, contenidoArchivo);
                iterDirectorio->archivos.push_back(nuevoArchivo);
                grafo.actualizarDesdeLista(listaDirectorios);

                // Guardar el archivo en el disco
                string rutaArchivo = nombreDirectorio + "/" + nombreArchivo;
                ofstream archivo(rutaArchivo.c_str());
                if (archivo.is_open()) {
                    archivo << contenidoArchivo;
                    archivo.close();
                    cout << "\nArchivo creado exitosamente en la ruta \"" << rutaArchivo << "\"." << endl;
                } else {
                    cout << "\nError: No se pudo crear el archivo en la ruta \"" << rutaArchivo << "\". Asegúrese de que el directorio exista." << endl;
                }

                // Insertar el archivo en el Árbol AVL
                arbolAVL.insertar(nombreArchivo, tipoArchivo, nuevoArchivo);

                // Ordenar archivos dentro del directorio después de agregar uno nuevo
                ordenamientoIterativo(listaDirectorios);

                limpiarPantalla();
                } else {
                    cout << "El directorio especificado no existe." << endl;
                    limpiarPantalla();
                }
                } else {
                    cout << "No hay directorios disponibles para crear el archivo." << endl;
                    limpiarPantalla();
                }
                break;
            }


            case 3: {
                // Mostrar archivos y directorios
                system("clear || cls"); // Limpiar la pantalla

                // Construir el árbol binario
                NodoABB* raiz = nullptr;
                for (const auto& directorio : listaDirectorios) {
                    arbolABB.insertarDirectorioEnArbol(directorio.nombre);
                    for (const auto& archivo : directorio.archivos) {
                        arbolABB.insertarArchivoEnArbol(directorio.nombre, archivo->nombre, archivo);
                    }
                }

                // Mostrar los directorios y archivos utilizando el recorrido inorden
                cout << "Directorios disponibles (ordenados alfabeticamente):\n";
                if (arbolABB.raiz == nullptr) {
                    cout << "No hay elementos en el sistema" << endl;
                } else {
                    arbolABB.mostrarDirectoriosYArchivos();
                }

                cout <<"\n----- Opciones -----\n";
                cout << "1. Abrir un archivo\n";
                cout << "2. Eliminar un archivo\n";
                cout << "3. Eliminar un directorio\n";
                cout << "4. Regresar al menu principal\n";
                cout << "Ingrese su opcion: ";
                int opcionSubMenu;
                cin >> opcionSubMenu;
                switch (opcionSubMenu) {
                    case 1: {
                        // Abrir un archivo (con apoyo de Pila)
                        if (!pilaDirectorios.empty()) {
                            // Mostrar los directorios disponibles
                            cout << "\nDirectorios disponibles:\n";
                            for (const auto& directorio : listaDirectorios) {
                                cout << "- " << directorio.nombre << endl;
                            }

                            // Solicitar al usuario que seleccione un directorio existente
                            string nombreDirectorio;
                            cout << "\nSeleccione el directorio donde se encuentra el archivo que desea abrir: ";
                            cin.ignore();
                            getline(cin, nombreDirectorio);

                            // Buscar el directorio en la lista
                            auto iterDirectorio = find_if(listaDirectorios.begin(), listaDirectorios.end(), [&](const Directorio& dir) {
                                return dir.nombre == nombreDirectorio;
                            });

                            // Verificar si se encontró el directorio
                            if (iterDirectorio != listaDirectorios.end()) {
                                // Mostrar los archivos en el directorio seleccionado
                                cout << "\nArchivos en el directorio \"" << nombreDirectorio << "\":" << endl;
                                if (!iterDirectorio->archivos.empty()) {
                                    for (size_t i = 0; i < iterDirectorio->archivos.size(); ++i) {
                                        cout << i + 1 << ". " << iterDirectorio->archivos[i]->nombre << endl;
                                    }

                                    // Permitir al usuario seleccionar un archivo
                                    int seleccion;
                                    cout << "\nSeleccione el archivo que desea abrir: ";
                                    cin >> seleccion;
                                    cin.ignore();

                                    if (seleccion >= 1 && seleccion <= iterDirectorio->archivos.size()) {
                                        cout << "Abriendo el archivo \"" << iterDirectorio->archivos[seleccion - 1]->nombre << "\"" << endl;
                                        // Leer el contenido del archivo desde el disco
                                        ifstream archivo((nombreDirectorio + "/" + iterDirectorio->archivos[seleccion - 1]->nombre).c_str());
                                        string contenido;
                                        if (archivo.is_open()) {
                                            string linea;
                                            while (getline(archivo, linea)) {
                                                contenido += linea + "\n";
                                            }
                                            archivo.close();
                                            // Mostrar el contenido del archivo
                                            cout << "\nContenido del archivo:" << endl;
                                            cout << contenido << endl;
                                            cout << "\nFin del contenido del archivo '" << iterDirectorio->archivos[seleccion - 1]->nombre << "'" << endl;
                                        } else {
                                            cout << "No se pudo abrir el archivo." << endl;
                                        }
                                        limpiarPantalla();
                                    } else {
                                        cout << "Seleccion invalida." << endl;
                                        limpiarPantalla();
                                    }
                                } else {
                                    cout << "No hay archivos en este directorio." << endl;
                                    limpiarPantalla();
                                }
                            } else {
                                cout << "El directorio especificado no existe." << endl;limpiarPantalla();
                            }
                        } else {
                            cout << "No hay directorios en el sistema." << endl;
                            limpiarPantalla();
                        }
                        break;
                    }

                    case 2: {
                        // Eliminar un archivo (con apoyo de Cola)
                        if (!colaDirectorios.empty()) {
                            // Mostrar los directorios disponibles
                            cout << "\nDirectorios disponibles:\n";
                            for (const auto& directorio : listaDirectorios) {
                                cout << "- " << directorio.nombre << endl;
                            }

                            // Solicitar al usuario que seleccione un directorio existente
                            string nombreDirectorio;
                            cout << "\nSeleccione el directorio donde se encuentra el archivo que desea eliminar: ";
                            cin.ignore();
                            getline(cin, nombreDirectorio);

                            // Buscar el directorio en la lista
                            auto iterDirectorio = find_if(listaDirectorios.begin(), listaDirectorios.end(), [&](const Directorio& dir) {
                                return dir.nombre == nombreDirectorio;
                            });

                            // Verificar si se encontró el directorio
                            if (iterDirectorio != listaDirectorios.end()) {
                                // Mostrar los archivos en el directorio seleccionado
                                cout << "\nArchivos en el directorio \"" << nombreDirectorio << "\":" << endl;
                                if (!iterDirectorio->archivos.empty()) {
                                    for (size_t i = 0; i < iterDirectorio->archivos.size(); ++i) {
                                        cout << i + 1 << ". " << iterDirectorio->archivos[i]->nombre << endl;
                                    }

                                    // Permitir al usuario seleccionar un archivo
                                    int seleccion;
                                    cout << "\nSeleccione el archivo que desea eliminar: ";
                                    cin >> seleccion;
                                    cin.ignore();

                                    if (seleccion >= 1 && seleccion <= iterDirectorio->archivos.size()) {
                                        string nombreArchivoAEliminar = iterDirectorio->archivos[seleccion - 1]->nombre;
                                        // Eliminar el archivo del directorio y de la lista de archivos
                                        delete iterDirectorio->archivos[seleccion - 1];
                                        iterDirectorio->archivos.erase(iterDirectorio->archivos.begin() + seleccion - 1);

                                        // Eliminar el archivo del disco
                                        string rutaArchivoAEliminar = nombreDirectorio + "/" + nombreArchivoAEliminar;
                                        if (remove(rutaArchivoAEliminar.c_str()) == 0) {
                                            cout << "\nArchivo \"" << nombreArchivoAEliminar << "\" eliminado exitosamente." << endl;
                                            limpiarPantalla();
                                        } else {
                                            cout << "\nError: No se pudo eliminar el archivo \"" << nombreArchivoAEliminar << "\"." << endl;
                                        }
                                    } else {
                                        cout << "Seleccion invalida." << endl;
                                        limpiarPantalla();
                                    }
                                } else {
                                    cout << "No hay archivos en este directorio." << endl;
                                    limpiarPantalla();
                                }
                            } else {
                                cout << "El directorio especificado no existe." << endl;
                                limpiarPantalla();
                            }
                        } else {
                            cout << "No hay directorios en el sistema." << endl;
                            limpiarPantalla();
                        }
                        break;
                    }

                    case 3: {
                        // Eliminar un directorio (con apoyo de Lista)
                        if (!listaDirectorios.empty()) {
                            // Mostrar los directorios disponibles
                            cout << "\nDirectorios disponibles:\n";
                            for (const auto& directorio : listaDirectorios) {
                                cout << "- " << directorio.nombre << endl;
                            }

                            // Solicitar al usuario que seleccione un directorio existente
                            string nombreDirectorio;
                            cout << "\nSeleccione el directorio que desea eliminar: ";
                            cin.ignore();
                            getline(cin, nombreDirectorio);

                            // Buscar el directorio en la lista
                            auto iterDirectorio = find_if(listaDirectorios.begin(), listaDirectorios.end(), [&](const Directorio& dir) {
                                return dir.nombre == nombreDirectorio;
                            });

                            // Verificar si se encontró el directorio
                            if (iterDirectorio != listaDirectorios.end()) {
                                // Eliminar el directorio
                                if (eliminarDirectorio(nombreDirectorio, listaDirectorios, arbolABB)) {
                                    cout << "\nDirectorio \"" << nombreDirectorio << "\" eliminado exitosamente." << endl;

                                    limpiarPantalla();
                                } else {
                                    cout << "\nError: No se pudo eliminar el directorio \"" << nombreDirectorio << "\"." << endl;
                                    limpiarPantalla();
                                }
                            } else {
                                cout << "El directorio especificado no existe." << endl;
                                limpiarPantalla();
                            }
                        } else {
                            cout << "No hay directorios en el sistema." << endl;
                            limpiarPantalla();
                        }
                        break;
                    }

                    case 4: {
                        // Regresar al menú principal
                        limpiarPantalla();
                        break;
                    }

                    default: {
                        // Opcion invalida
                        cout << "Opcion invalida." << endl;
                        limpiarPantalla();
                    }
                }
                break;
            }

            case 4: {
                // Buscar archivo por nombre
                string nombreArchivo;
                cout << "Ingrese el nombre del archivo que desea buscar: ";
                getline(cin, nombreArchivo);

                // Realizar la búsqueda en el sistema de archivos
                bool encontrado = false;
                string directorioArchivo;
                // Utilizar búsqueda en profundidad o en anchura en el grafo para encontrar el archivo
                if (grafo.busquedaEnAnchura(nombreArchivo) || grafo.busquedaEnProfundidad(nombreArchivo)) {
                    encontrado = true;
                    // Obtener el camino desde el nodo raíz hasta el nodo que contiene el archivo
                    vector<string> camino = grafo.obtenerCaminoHastaArchivo(nombreArchivo);
                    // Construir la cadena que representa el camino
                    directorioArchivo = grafo.construirDirectorioDesdeCamino(camino);

                    // Mostrar el directorio donde se encontró el archivo
                    cout << "El archivo '" << nombreArchivo << "' se encuentra en el directorio: " << directorioArchivo << endl;
                    } else {
                        cout << "El archivo '" << nombreArchivo << "' no fue encontrado." << endl;
                    }

                limpiarPantalla();
                break;
            }

            case 5: {
                // Búsqueda de archivos en el sistema de archivos utilizando el árbol AVL (archivos por tipo)
                string tipoArchivo;
                cout << "Ingrese el tipo de archivo que desea buscar: ";
                getline(cin, tipoArchivo);

                // Buscar los archivos en el árbol AVL por tipo
                vector<Archivo*> archivosEncontrados = arbolAVL.buscarArchivosPorTipo(tipoArchivo);

                if (!archivosEncontrados.empty()) {
                    // Mostrar los archivos encontrados del tipo especificado
                    cout << "\nArchivos de tipo " << tipoArchivo << " encontrados en el sistema de archivos:" << endl;
                    for (Archivo* archivo : archivosEncontrados) {
                        cout << "- " << archivo->nombre << endl;
                    }
                } else {
                    cout << "No se encontraron archivos de tipo " << tipoArchivo << " en el sistema de archivos." << endl;
                }

                limpiarPantalla();
                break;
            }

            case 6: {
                string nombreDirectorio;
                cout << "Ingrese el nombre del directorio que desea buscar: ";
                getline(cin, nombreDirectorio);

                if (listaDirectorios.size() < 5) {
                    // Si la lista de directorios es pequeña, utilizar búsqueda lineal
                    Directorio* iterDirectorio = busquedaLineal(listaDirectorios, nombreDirectorio);
                    if (iterDirectorio != nullptr) {
                        cout << "El directorio \"" << iterDirectorio->nombre << "\" fue encontrado en el sistema" << endl;
                        // Mostrar los archivos y subdirectorios del directorio encontrado
                    } else {
                        cout << "El directorio especificado no existe." << endl;
                    }
                } else {
                    // Si la lista de directorios es grande, utilizar búsqueda binaria
                    ordenamientoRecursivo(listaDirectorios); // Asegurarse de que la lista esté ordenada
                    Directorio* iterDirectorio = busquedaBinaria(listaDirectorios, nombreDirectorio);
                    if (iterDirectorio != nullptr) {
                        cout << "El directorio \"" << iterDirectorio->nombre << "\" fue encontrado en el sistema" << endl;
                        // Mostrar los archivos y subdirectorios del directorio encontrado
                    } else {
                        cout << "El directorio especificado no existe." << endl;
                    }
                }
                limpiarPantalla();
                break;
            }

            case 7: {
                // Salir del programa
                cout << "Saliendo del programa..." << endl;
                break;
            }

            default: {
                // Opción inválida
                cout << "Opcion invalida." << endl;
                limpiarPantalla();
                break;
            }
        }
    } while (opcion != 7);

    // Liberar la memoria utilizada por los archivos
    liberarMemoria(listaDirectorios);

    return 0;
}
