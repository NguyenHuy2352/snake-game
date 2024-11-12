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
// Hàm di chuyển con trỏ đến vị trí (column, line) trong console
void gotoxy(int column, int line) {
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// Cấu trúc biểu diễn một điểm trên màn hình
struct Point {
    int x, y;
};
// Lớp CONRAN để quản lý con rắn
class CONRAN {
public:
    Point A[100]; // Mảng các điểm của con rắn
    int DoDai;  // Độ dài hiện tại của con rắn
// Hàm khởi tạo ban đầu cho con rắn
    CONRAN() {
        DoDai = 4;    // Đặt độ dài mặc định là 4
        // Đặt vị trí ban đầu của các phần con rắn
        A[0].x = 15; A[0].y = 10;
        A[1].x = 16; A[1].y = 10;
        A[2].x = 17; A[2].y = 10;
        A[3].x = 18; A[3].y = 10;
    }
 void setColor(int color) {
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
 }

 // Hàm vẽ con rắn và quả trên màn hình
 void Ve(Point Qua) {
     for (int i = 0; i < DoDai; i++) {
         gotoxy(A[i].x, A[i].y);  // Di chuyển con trỏ đến vị trí từng điểm của rắn
         if (i == 0) {
             setColor(14);        // Đặt màu vàng cho đầu rắn
             cout << "0";         // Sử dụng ký tự "o" cho đầu rắn
         }
         else {
             setColor(10);        // Đặt màu xanh lá cây cho thân rắn
             cout << "o";         // Sử dụng ký tự "O" cho thân rắn
         }
     }
     setColor(12);               // Đặt màu đỏ cho quả
     gotoxy(Qua.x, Qua.y);       // Vẽ quả
     cout << "*";
     setColor(7);                // Trả lại màu mặc định
 }
     // Xóa điểm cuối của con rắn (dùng khi di chuyển)
    void XoaDauCuoi() {
        gotoxy(A[DoDai - 1].x, A[DoDai - 1].y);
        cout << " ";
    }
// Hàm di chuyển con rắn theo hướng chỉ định
    void DiChuyen(int Huong) {
        XoaDauCuoi();
         // Dịch chuyển các điểm của con rắn lên một vị trí
        for (int i = DoDai - 1; i > 0; i--)
            A[i] = A[i - 1];
         // Di chuyển đầu rắn theo hướng chỉ định
        if (Huong == 0) A[0].x = A[0].x + 1; // Sang phải
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
    // đừng xóa nha ae
   /*while (true) {
        if (_kbhit()) {
            t = _getch();
            if (t == 'a') Huong = 2;
            if (t == 'w') Huong = 3;
            if (t == 'd') Huong = 0;
            if (t == 's') Huong = 1;
        }
    }*/
    //Con rắn đi ngang theo hướng D mà  nhấn A(ngược hướng với D) phát đuổi đầu với đuôi luôn
    /*
    while (true) {
       if (_kbhit()) {
    t = _getch();
    
    if ((t == 'a' || t == 'd') && (Huong == 1 || Huong == 3)) {
        if (t == 'a') Huong = 2;  
        if (t == 'd') Huong = 0;  
    }
    if ((t == 'w' || t == 's') && (Huong == 0 || Huong == 2)) {
        if (t == 'w') Huong = 3;  
        if (t == 's') Huong = 1; 
    }
}*/
    // sử dụng cách phím di chuyển bằng phím mũi tên và các phím W, A,S,D
    while (true) {
    if (_kbhit()) {
        int t = _getch();
        if (t == 224) { //  Điều khiển bằng phím mũi tên; Mã phím mũi tên bắt đầu bằng 224
            t = _getch();
            if ((t == 75 || t == 77) && (Huong == 1 || Huong == 3)) { // Trái hoặc phải
                if (t == 75) Huong = 2;  // Mũi tên trái
                if (t == 77) Huong = 0;  // Mũi tên phải
            }
            if ((t == 72 || t == 80) && (Huong == 0 || Huong == 2)) { //  Lên hoặc xuống
                if (t == 72) Huong = 3;  // Mũi tên lên
                if (t == 80) Huong = 1;  // Mũi tên xuống
            }
        }
        else {
            // Điều khiển bằng phím W, A, S, D
            if ((t == 'a' || t == 'd') && (Huong == 1 || Huong == 3)) { // Trái hoặc phải
                if (t == 'a') Huong = 2;  // Phím A
                if (t == 'd') Huong = 0;  // Phím D
            }
            if ((t == 'w' || t == 's') && (Huong == 0 || Huong == 2)) { // Lên hoặc xuống
                if (t == 'w') Huong = 3;  // Phím W
                if (t == 's') Huong = 1;  // Phím S
            }
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


