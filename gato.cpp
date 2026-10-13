#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Matriz global con todas las combinaciones ganadoras (filas, columnas, diagonales)
const int COMBOS[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Filas
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columnas
    {0, 4, 8}, {2, 4, 6}             // Diagonales
};

void dibujar(const vector<char>& t) {
    cout << "\n " << t[0] << " | " << t[1] << " | " << t[2] << "\n---|---|---\n"
        << " " << t[3] << " | " << t[4] << " | " << t[5] << "\n---|---|---\n"
        << " " << t[6] << " | " << t[7] << " | " << t[8] << "\n\n";
}

bool comprobarGanador(const vector<char>& t, char p) {
    for (int i = 0; i < 8; i++) {
        if (t[COMBOS[i][0]] == p && t[COMBOS[i][1]] == p && t[COMBOS[i][2]] == p) return true;
    }
    return false;
}

// Lógica de la IA Inteligente (Modo Difícil)
int obtenerMovimientoDificil(const vector<char>& t) {
    // 1. PRIORIDAD: Buscar si la IA (O) puede ganar en este turno
    for (int i = 0; i < 8; i++) {
        int c0 = COMBOS[i][0], c1 = COMBOS[i][1], c2 = COMBOS[i][2];
        if (t[c0] == 'O' && t[c1] == 'O' && t[c2] != 'X' && t[c2] != 'O') return c2 + 1;
        if (t[c0] == 'O' && t[c2] == 'O' && t[c1] != 'X' && t[c1] != 'O') return c1 + 1;
        if (t[c1] == 'O' && t[c2] == 'O' && t[c0] != 'X' && t[c0] != 'O') return c0 + 1;
    }

    // 2. PRIORIDAD: Bloquear al jugador humano (X) si está por ganar
    for (int i = 0; i < 8; i++) {
        int c0 = COMBOS[i][0], c1 = COMBOS[i][1], c2 = COMBOS[i][2];
        if (t[c0] == 'X' && t[c1] == 'X' && t[c2] != 'X' && t[c2] != 'O') return c2 + 1;
        if (t[c0] == 'X' && t[c2] == 'X' && t[c1] != 'X' && t[c1] != 'O') return c1 + 1;
        if (t[c1] == 'X' && t[c2] == 'X' && t[c0] != 'X' && t[c0] != 'O') return c0 + 1;
    }

    // 3. PRIORIDAD: Tomar el centro si está libre
    if (t[4] != 'X' && t[4] != 'O') return 5;

    // 4. PRIORIDAD: Tomar esquinas libres (1, 3, 7, 9)
    int esquinas[] = { 0, 2, 6, 8 };
    for (int e : esquinas) {
        if (t[e] != 'X' && t[e] != 'O') return e + 1;
    }

    // 5. PRIORIDAD: Tomar cualquier casilla libre al azar (por si acaso)
    int pos;
    do {
        pos = (rand() % 9) + 1;
    } while (t[pos - 1] == 'X' || t[pos - 1] == 'O');
    return pos;
}

int main() {
    srand(time(0));
    vector<char> t = { '1','2','3','4','5','6','7','8','9' };
    char j = 'X';
    int pos, turnos = 0;
    int modo = 0, dificultad = 1;

    cout << "=== BIENVENIDO AL JUEGO DEL GATO ===\n";
    cout << "Selecciona el modo de juego:\n1. Un solo jugador (Contra la IA)\n2. Dos jugadores (Local)\nOpción: ";
    cin >> modo;

    if (modo == 1) {
        cout << "\nSelecciona la dificultad de la IA:\n1. Fácil (Movimientos al azar)\n2. Difícil (IA Estratégica)\nOpción: ";
        cin >> dificultad;
    }
    else if (modo != 2) {
        cout << "Opción inválida. Modo 2 jugadores por defecto.\n";
        modo = 2;
    }

    while (turnos < 9) {
        dibujar(t);

        if (j == 'X' || modo == 2) {
            cout << "Jugador " << j << ", elige posicion (1-9): ";
            cin >> pos;
            if (pos < 1 || pos > 9 || t[pos - 1] == 'X' || t[pos - 1] == 'O') {
                cout << "Movimiento invalido. Intenta de nuevo.\n";
                continue;
            }
        }
        else {
            cout << "Turno de la IA (O)...\n";
            if (dificultad == 2) {
                pos = obtenerMovimientoDificil(t); // Lógica avanzada
            }
            else {
                do {
                    pos = (rand() % 9) + 1; // Lógica fácil (al azar)
                } while (t[pos - 1] == 'X' || t[pos - 1] == 'O');
            }
        }

        t[pos - 1] = j;
        turnos++;

        if (comprobarGanador(t, j)) {
            dibujar(t);
            if (modo == 1 && j == 'O') cout << "¡La IA ha ganado! Más suerte la próxima vez.\n";
            else cout << "¡El jugador " << j << " ha ganado el juego!\n";
            return 0;
        }

        j = (j == 'X') ? 'O' : 'X';
    }

    dibujar(t);
    cout << "¡Es un empate!\n";
    return 0;
}
