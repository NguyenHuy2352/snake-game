#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <time.h>

#define MINX 2
#define MINY 2
#define MAXX 35
#define MAXY 20

using namespace std;

void gotoxy(int column, int line) {
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct Point {
    int x, y;
};

class CONRAN {
public:
    Point A[100];
    int DoDai;

    CONRAN() {
        DoDai = 4;
        A[0].x = 15; A[0].y = 10;
        A[1].x = 16; A[1].y = 10;
        A[2].x = 17; A[2].y = 10;
        A[3].x = 18; A[3].y = 10;
    }

    void Ve(Point Qua) {
        for (int i = 0; i < DoDai; i++) {
            gotoxy(A[i].x, A[i].y);
            if (i == 0)
                cout << "O";
            else cout << "o";
        }
        gotoxy(Qua.x, Qua.y);
        cout << "*";
    }

    void XoaDauCuoi() {
        gotoxy(A[DoDai - 1].x, A[DoDai - 1].y);
        cout << " ";
    }

    void DiChuyen(int Huong) {
        XoaDauCuoi();
        for (int i = DoDai - 1; i > 0; i--)
            A[i] = A[i - 1];
        if (Huong == 0) A[0].x = A[0].x + 1;
        if (Huong == 1) A[0].y = A[0].y + 1;
        if (Huong == 2) A[0].x = A[0].x - 1;
        if (Huong == 3) A[0].y = A[0].y - 1;
    }

    bool KiemTraVaChamKhung() {
        return (A[0].x <= MINX || A[0].x >= MAXX || A[0].y <= MINY || A[0].y >= MAXY);
    }

    bool KiemTraTuDam() {
        if (DoDai <= 4) return false;
        for (int i = 1; i < DoDai; i++) {
            if (A[0].x == A[i].x && A[0].y == A[i].y) {
                return true;
            }
        }
        return false;
    }

    void AnMoi(int Huong) {
        this->DoDai++;
        this->DiChuyen(Huong);
    }

    int GetHeadX() {
        return A[0].x;
    }

    int GetHeadY() {
        return A[0].y;
    }
};

void VeKhung() {
    for (int i = MINX; i <= MAXX; i++) {
        gotoxy(i, MINY);
        cout << "-";
        gotoxy(i, MAXY);
        cout << "-";
    }
    for (int j = MINY; j <= MAXY; j++) {
        gotoxy(MINX, j);
        cout << "|";
        gotoxy(MAXX, j);
        cout << "|";
    }
}

int main() {
    CONRAN r;
    int Huong = 0;
    char t;
    int score = 0;
    Point Qua;
    srand((int)time(0));
    Qua.x = rand() % (MAXX - MINX - 1) + MINX + 1;
    Qua.y = rand() % (MAXY - MINY - 1) + MINY + 1;

    system("cls");
    VeKhung();

    while (true) {
        if (_kbhit()) {
            t = _getch();
            // Chỉ thay đổi hướng khi hướng mới khác hướng hiện tại để tránh di chuyển ngược lại
            if ((t == 'a' || t == 'd') && (Huong == 1 || Huong == 3)) {
                if (t == 'a') Huong = 2;  // Di chuyển trái khi đang đi dọc
                if (t == 'd') Huong = 0;  // Di chuyển phải khi đang đi dọc
            }
            if ((t == 'w' || t == 's') && (Huong == 0 || Huong == 2)) {
                if (t == 'w') Huong = 3;  // Di chuyển lên khi đang đi ngang
                if (t == 's') Huong = 1;  // Di chuyển xuống khi đang đi ngang
            }
        }

        r.Ve(Qua);
        gotoxy(0, MAXY + 2);
        cout << "Score: " << score;

        if (r.KiemTraVaChamKhung() || r.KiemTraTuDam()) {
            gotoxy(0, MAXY + 4);
            cout << "Game Over!" << endl;
            cout << "Final Score: " << score << endl;
            return 0;
        }

        if (r.GetHeadX() == Qua.x && r.GetHeadY() == Qua.y) {
            r.AnMoi(Huong);
            Qua.x = rand() % (MAXX - MINX - 1) + MINX + 1;
            Qua.y = rand() % (MAXY - MINY - 1) + MINY + 1;
            score += 10;
        }
        else {
            r.DiChuyen(Huong);
        }

        Sleep(300 - score);
    }
}
