#include <ncurses.h>
#include <vector>
#include <string>
#include <unistd.h>

using namespace std;

int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    // Peta maze
    vector<string> maze = {
        "###############################",
        "#S    #       #       #       #",
        "##### # ##### # ##### # ##### #",
        "#     # #     #     # #     # #",
        "# ##### # ######### # ##### # #",
        "#       #       #   #       # #",
        "# ########### # # ######### # #",
        "#           # # #         # # #",
        "########### # # ######### # # #",
        "#         # # #         # #   #",
        "# ####### # # ######### # #####",
        "#       # # #         # #     #",
        "##### # # # ######### # ##### #",
        "#     #   #           #      E#",
        "###############################"
    };

    int rows = maze.size();
    int cols = maze[0].size();

    int playerY = 1;
    int playerX = 1;

    int exitY = 13;
    int exitX = 29;

    int offsetY = 2;
    int offsetX = 2;

    bool gameOver = false;
    bool win = false;

    // Warna
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    // =========================
    // HALAMAN AWAL
    // =========================

    clear();

    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(1, 2, "=== MEMORY MAZE ===");
    attroff(COLOR_PAIR(1) | A_BOLD);

    mvprintw(3, 2, "Ingat posisi tembok dan jalur maze!");
    mvprintw(4, 2, "Kamu hanya punya waktu 7 detik.");
    mvprintw(6, 2, "Tekan tombol apa saja untuk memulai.");

    refresh();
    getch();

    // =========================
    // TAHAP OBSERVASI
    // =========================

    clear();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            char c = maze[i][j];

            if (c == '#') {
                attron(COLOR_PAIR(1));
                mvaddch(offsetY + i, offsetX + j, '#');
                attroff(COLOR_PAIR(1));
            }
            else if (c == 'S') {
                attron(COLOR_PAIR(2) | A_BOLD);
                mvaddch(offsetY + i, offsetX + j, '@');
                attroff(COLOR_PAIR(2) | A_BOLD);
            }
            else if (c == 'E') {
                attron(COLOR_PAIR(4) | A_BOLD);
                mvaddch(offsetY + i, offsetX + j, 'E');
                attroff(COLOR_PAIR(4) | A_BOLD);
            }
            else {
                mvaddch(offsetY + i, offsetX + j, ' ');
            }
        }
    }

    mvprintw(0, 2, "HAFALKAN MAZE! Waktu: 7 detik");
    refresh();

    napms(7000);

    // =========================
    // TAHAP PERMAINAN
    // =========================

    clear();

    mvprintw(0, 2, "MEMORY MAZE - TEMUKAN PINTU KELUAR!");
    mvprintw(1, 2, "Gunakan panah / WASD | Q = Keluar");

    // Maze disembunyikan
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mvaddch(offsetY + i, offsetX + j, ' ');
        }
    }

    // Tampilkan pintu keluar
    attron(COLOR_PAIR(4) | A_BOLD);
    mvaddch(offsetY + exitY, offsetX + exitX, 'E');
    attroff(COLOR_PAIR(4) | A_BOLD);

    // Tampilkan pemain
    attron(COLOR_PAIR(2) | A_BOLD);
    mvaddch(offsetY + playerY, offsetX + playerX, '@');
    attroff(COLOR_PAIR(2) | A_BOLD);

    refresh();

    // =========================
    // LOOP PERGERAKAN
    // =========================

    while (!gameOver) {

        int key = getch();

        int nextY = playerY;
        int nextX = playerX;

        switch (key) {

            case KEY_UP:
            case 'w':
            case 'W':
                nextY--;
                break;

            case KEY_DOWN:
            case 's':
            case 'S':
                nextY++;
                break;

            case KEY_LEFT:
            case 'a':
            case 'A':
                nextX--;
                break;

            case KEY_RIGHT:
            case 'd':
            case 'D':
                nextX++;
                break;

            case 'q':
            case 'Q':
                gameOver = true;
                break;
        }

        if (gameOver) break;

        // Cek tabrakan dengan tembok
        if (maze[nextY][nextX] == '#') {
            gameOver = true;
            win = false;
            break;
        }

        // Hapus posisi pemain sebelumnya
        mvaddch(offsetY + playerY, offsetX + playerX, ' ');

        playerY = nextY;
        playerX = nextX;

        // Cek kemenangan
        if (playerY == exitY && playerX == exitX) {
            gameOver = true;
            win = true;
        }

        // Gambar pemain
        attron(COLOR_PAIR(2) | A_BOLD);
        mvaddch(offsetY + playerY, offsetX + playerX, '@');
        attroff(COLOR_PAIR(2) | A_BOLD);

        // Gambar kembali pintu keluar jika belum tercapai
        if (!win) {
            attron(COLOR_PAIR(4) | A_BOLD);
            mvaddch(offsetY + exitY, offsetX + exitX, 'E');
            attroff(COLOR_PAIR(4) | A_BOLD);
        }

        refresh();
    }

    // =========================
    // HASIL PERMAINAN
    // =========================

    clear();

    if (win) {
        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(5, 5, "SELAMAT! KAMU BERHASIL!");
        attroff(COLOR_PAIR(2) | A_BOLD);
    }
    else {
        attron(COLOR_PAIR(3) | A_BOLD);
        mvprintw(5, 5, "GAME OVER! KAMU MENABRAK TEMBOK!");
        attroff(COLOR_PAIR(3) | A_BOLD);
    }

    mvprintw(7, 5, "Tekan tombol apa saja untuk keluar.");
    refresh();

    getch();

    endwin();

    return 0;
}
