#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <clocale>

using namespace std;

// Códigos de color ANSI para la terminal
const string RESET   = "\033[0m";
const string ROJO    = "\033[1;31m"; // Color para la X
const string AZUL    = "\033[1;34m"; // Color para la O
const string GRIS    = "\033[0;90m"; // Color para los números guía
const string VERDE   = "\033[1;32m"; // Color para mensajes de victoria

const int COMBOS[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Filas
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columnas
    {0, 4, 8}, {2, 4, 6}             // Diagonales
};

// Función para darle color a cada casilla individualmente
string obtenerCasillaConColor(char c) {
    if (c == 'X') return ROJO + "X" + RESET;
    if (c == 'O') return AZUL + "O" + RESET;
    return GRIS + c + RESET; // Si es un número del 1 al 9, lo pinta en gris oscuro
}

// Función para dibujar el tablero limpio en la terminal
void dibujar(const vector<char>& t) {
    system("cls"); // <--- LIMPIA LA PANTALLA EN CADA TURNO
    cout << "=== JUEGO DEL GATO ===\n";
    cout << "\n " << obtenerCasillaConColor(t[0]) << " | " << obtenerCasillaConColor(t[1]) << " | " << obtenerCasillaConColor(t[2]) << "\n---|---|---\n"
         << " " << obtenerCasillaConColor(t[3]) << " | " << obtenerCasillaConColor(t[4]) << " | " << obtenerCasillaConColor(t[5]) << "\n---|---|---\n"
         << " " << obtenerCasillaConColor(t[6]) << " | " << obtenerCasillaConColor(t[7]) << " | " << obtenerCasillaConColor(t[8]) << "\n\n";
}

bool comprobarGanador(const vector<char>& t, char p) {
    for (int i = 0; i < 8; i++) {
        if (t[COMBOS[i][0]] == p && t[COMBOS[i][1]] == p && t[COMBOS[i][2]] == p) return true;
    }
    return false;
}

int obtenerMovimientoDificil(const vector<char>& t) {
    for (int i = 0; i < 8; i++) {
        int c0 = COMBOS[i][0], c1 = COMBOS[i][1], c2 = COMBOS[i][2];
        if (t[c0] == 'O' && t[c1] == 'O' && t[c2] != 'X' && t[c2] != 'O') return c2 + 1;
        if (t[c0] == 'O' && t[c2] == 'O' && t[c1] != 'X' && t[c1] != 'O') return c1 + 1;
        if (t[c1] == 'O' && t[c2] == 'O' && t[c0] != 'X' && t[c0] != 'O') return c0 + 1;
    }
    for (int i = 0; i < 8; i++) {
        int c0 = COMBOS[i][0], c1 = COMBOS[i][1], c2 = COMBOS[i][2];
        if (t[c0] == 'X' && t[c1] == 'X' && t[c2] != 'X' && t[c2] != 'O') return c2 + 1;
        if (t[c0] == 'X' && t[c2] == 'X' && t[c1] != 'X' && t[c1] != 'O') return c1 + 1;
        if (t[c1] == 'X' && t[c2] == 'X' && t[c0] != 'X' && t[c0] != 'O') return c0 + 1;
    }
    if (t[4] != 'X' && t[4] != 'O') return 5;
    int esq[4] = {0, 2, 6, 8};
    for (int e : esq) {
        if (t[e] != 'X' && t[e] != 'O') return e + 1;
    }
    int pos;
    do {
        pos = (rand() % 9) + 1;
    } while (t[pos - 1] == 'X' || t[pos - 1] == 'O');
    return pos;
}

int main() {
    setlocale(LC_ALL, "spanish"); // Mantener los acentos corregidos
    srand(time(0));
    vector<char> t = {'1','2','3','4','5','6','7','8','9'};
    char j = 'X'; 
    int pos, turnos = 0, modo = 0, dif = 1;

    cout << "=== BIENVENIDO AL JUEGO DEL GATO ===\n";
    cout << "Selecciona el modo de juego:\n1. Un solo jugador (Contra la IA)\n2. Dos jugadores (Local)\nOpción: ";
    cin >> modo;

    if (modo == 1) {
        cout << "\nSelecciona la dificultad de la IA:\n1. Fácil\n2. Difícil\nOpción: ";
        cin >> dif;
    } else if (modo != 2) {
        modo = 2;
    }
    
    while (turnos < 9) {
        dibujar(t);
        if (j == 'X' || modo == 2) {
            cout << "Jugador " << (j == 'X' ? ROJO : AZUL) << j << RESET << ", elige posición (1-9): ";
            cin >> pos;
            if (pos < 1 || pos > 9 || t[pos-1] == 'X' || t[pos-1] == 'O') {
                cout << "Inválido. Intenta de nuevo.";
                cin.ignore(); cin.get(); // Pausa breve para que lean el error
                continue;
            }
        } else {
            cout << "Turno de la IA (O)...\n";
            _sleep(600); // Pausa de medio segundo para simular que la IA piensa
            pos = (dif == 2) ? obtenerMovimientoDificil(t) : (rand() % 9) + 1;
            if (t[pos-1] == 'X' || t[pos-1] == 'O') {
                do {
                    pos = (rand() % 9) + 1;
                } while (t[pos-1] == 'X' || t[pos-1] == 'O');
            }
        }
        t[pos-1] = j;
        turnos++;
        
        if (comprobarGanador(t, j)) {
            dibujar(t);
            if (modo == 1 && j == 'O') cout << VERDE << "¡La IA ha ganado!\n" << RESET;
            else cout << VERDE << "¡El jugador " << j << " ha ganado el juego!\n" << RESET;
            cout << "Presiona Enter para salir...";
            cin.ignore(); cin.get();
            return 0;
        }
        j = (j == 'X') ? 'O' : 'X';
    }
    dibujar(t);
    cout << VERDE << "¡Es un empate!\n" << RESET;
    cout << "Presiona Enter para salir...";
    cin.ignore(); cin.get();
    return 0;
}
