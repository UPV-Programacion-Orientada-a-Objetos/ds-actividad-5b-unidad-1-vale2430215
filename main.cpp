#include <iostream>
using namespace std;

// Clase Base Abstracta
template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;
public:
    MatrizBase(int f, int c) : _filas(f), _columnas(c) {}
    virtual ~MatrizBase() {}

    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;

    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }
};

// Clase Matriz Dinámica
template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T** _datos;

    void liberarMemoria() {
        for (int i = 0; i < this->_filas; i++)
            delete[] _datos[i];
        delete[] _datos;
    }

    void copiarDatos(const MatrizDinamica& otra) {
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; i++) {
            _datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; j++)
                _datos[i][j] = otra._datos[i][j];
        }
    }

public:
    // Constructor
    MatrizDinamica(int f, int c) : MatrizBase<T>(f, c) {
        _datos = new T*[f];
        for (int i = 0; i < f; i++)
            _datos[i] = new T[c];
    }

    // Destructor
    ~MatrizDinamica() override {
        cout << "Liberando memoria de Matriz Dinamica..." << endl;
        liberarMemoria();
    }

    // Constructor de copia
    MatrizDinamica(const MatrizDinamica& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        copiarDatos(otra);
    }

    // Operador de asignación
    MatrizDinamica& operator=(const MatrizDinamica& otra) {
        if (this != &otra) {
            liberarMemoria();
            copiarDatos(otra);
        }
        return *this;
    }

    // Cargar valores manuales
    void cargarValores() override {
        cout << "Ingrese los valores de la matriz (" << this->_filas << "x" << this->_columnas << "):\n";
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                cout << "Elemento [" << i << "][" << j << "]: ";
                cin >> _datos[i][j];
            }
        }
    }

    // Imprimir
    void imprimir() const override {
        for (int i = 0; i < this->_filas; i++) {
            cout << "| ";
            for (int j = 0; j < this->_columnas; j++) {
                cout << _datos[i][j] << " | ";
            }
            cout << endl;
        }
    }

    // Sumar (solo entre dinámicas)
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            cout << "Error: Dimensiones incompatibles." << endl;
            return nullptr;
        }

        auto* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);
        const MatrizDinamica<T>* ptrOtra = dynamic_cast<const MatrizDinamica<T>*>(&otra);

        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                resultado->_datos[i][j] = this->_datos[i][j] + ptrOtra->_datos[i][j];
            }
        }
        return resultado;
    }

    // Sobrecarga del operador +
    MatrizDinamica operator+(const MatrizDinamica& otra) const {
        MatrizDinamica resultado(this->_filas, this->_columnas);
        for (int i = 0; i < this->_filas; i++)
            for (int j = 0; j < this->_columnas; j++)
                resultado._datos[i][j] = this->_datos[i][j] + otra._datos[i][j];
        return resultado;
    }

    // Método auxiliar para asignar valores
    void setValor(int f, int c, T valor) {
        _datos[f][c] = valor;
    }

    T getValor(int f, int c) const {
        return _datos[f][c];
    }
};

// Clase Matriz Estática
template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N];
public:
    MatrizEstatica() : MatrizBase<T>(M, N) {}

    void cargarValores() override {
        cout << "Ingrese los valores de la matriz (" << M << "x" << N << "):\n";
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                cout << "Elemento [" << i << "][" << j << "]: ";
                cin >> _datos[i][j];
            }
        }
    }

    void imprimir() const override {
        for (int i = 0; i < M; i++) {
            cout << "| ";
            for (int j = 0; j < N; j++) {
                cout << _datos[i][j] << " | ";
            }
            cout << endl;
        }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            cout << "Error: Dimensiones incompatibles." << endl;
            return nullptr;
        }

        auto* resultado = new MatrizDinamica<T>(M, N);
        const MatrizEstatica<T, M, N>* ptrOtra = dynamic_cast<const MatrizEstatica<T, M, N>*>(&otra);

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                resultado->setValor(i, j, _datos[i][j] + ptrOtra->_datos[i][j]);

        return resultado;
    }

    // Método auxiliar para asignar valores sin entrada de usuario
    void setValor(int f, int c, T valor) {
        _datos[f][c] = valor;
    }
};

// Función principal
int main() {
    cout << "Sistema generico de Algebra Lineal\n\n";
    cout << "Demostracion de Genericidad (Tipo FLOAT)\n\n";

    // Matriz Dinámica A
    MatrizDinamica<float>* A = new MatrizDinamica<float>(3, 2);
    cout << "Creando Matriz Dinamica A (3x2)...\n";
    A->setValor(0,0,1.5); A->setValor(0,1,2.0);
    A->setValor(1,0,0.0); A->setValor(1,1,1.0);
    A->setValor(2,0,4.5); A->setValor(2,1,3.0);
    cout << "A = \n"; A->imprimir(); cout << endl;

    // Matriz Estática B
    MatrizEstatica<float,3,2>* B = new MatrizEstatica<float,3,2>();
    cout << "Creando Matriz Estática B (3x2)...\n";
    B->setValor(0,0,0.5); B->setValor(0,1,1.0);
    B->setValor(1,0,2.0); B->setValor(1,1,3.0);
    B->setValor(2,0,1.0); B->setValor(2,1,1.0);
    cout << "B = \n"; B->imprimir(); cout << endl;

    cout << "SUMANDO: Matriz C = A + A ... (demostracion polimorfica)\n";
    MatrizBase<float>* C = A->sumar(*A);
    cout << "\nMatriz Resultado C (3x2, Tipo FLOAT):\n";
    C->imprimir();

    cout << "\n>> Demostracion de Limpieza de Memoria <<\n";
    delete C;
    delete A;
    delete B;
    cout << "Sistema cerrado.\n";

    return 0;
}
