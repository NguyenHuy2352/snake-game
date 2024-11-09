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
                cout << "o";
            else cout << "=";
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
};



int main() {
    CONRAN r;
    int Huong = 0;
    char t;
    

    while (true) {
        if (_kbhit()) {
            t = _getch();
            if (t == 'a') Huong = 2;
            if (t == 'w') Huong = 3;
            if (t == 'd') Huong = 0;
            if (t == 's') Huong = 1;
        }

        r.Ve(Qua);
        gotoxy(0, MAXY + 2);
       


       
        if (r.GetHeadX() == Qua.x && r.GetHeadY() == Qua.y) {
            r.AnMoi(Huong);
            Qua.x = rand() % (MAXX - MINX - 1) + MINX + 1;
            Qua.y = rand() % (MAXY - MINY - 1) + MINY + 1;
              
        }
        else {
            r.DiChuyen(Huong);
        }

        Sleep(300);
    }
}