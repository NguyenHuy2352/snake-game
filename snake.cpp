#include <iostream>     // cin, cout
#include <windows.h>    // ConsoleCursorPosition, ConsoleTextAttribute
#include <cstdlib>      // rand(), srand()
#include <conio.h>      // _kbhit(), _getch()
#include <time.h>       // time()

// Tọa độ tối thiểu và tối đa của khung chơi
#define MINX 2
#define MINY 1
#define MAXX 75
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

// Lưu hướng di chuyển của từng phần thân
enum Direction { UP, DOWN, LEFT, RIGHT };

// Lưu lại hướng di chuyển của đầu rắn trước khi chuyển hướng
// Vì có lỗi xảy ra khi chuyển hướng rắn lúc rắn đang đi lên hoặc đi xuống
// dẫn đến hình ảnh rắn không được cập nhật đúng
Direction previousHead;

// Lớp CONRAN để quản lý con rắn
class CONRAN {
public:
    Point A[100]; // Mảng các điểm của con rắn
    Direction directions[100]; // Mảng lưu hướng của từng phần rắn, hoạt động cùng với enum Direction
    int DoDai;  // Độ dài hiện tại của con rắn

    // Hàm khởi tạo ban đầu cho con rắn
    CONRAN() {
        DoDai = 4;    // Đặt độ dài mặc định là 4
        // Đặt vị trí ban đầu của các phần con rắn
        A[0].x = 18; A[0].y = 10;
        A[1].x = 17; A[1].y = 10;
        A[2].x = 16; A[2].y = 10;
        A[3].x = 15; A[3].y = 10;
        // Đặt hướng ban đầu của đầu rắn
        directions[0] = RIGHT;
        directions[1] = RIGHT;
        directions[2] = RIGHT;
        directions[3] = RIGHT;
        previousHead = directions[0];
    }

    // Hàm setColor thay đổi màu sắc của văn bản trong console.
    // Mọi ký tự in ra sau khi gọi setColor sẽ có màu sắc theo mã được truyền vào.
    // Ví dụ: setColor(12) là màu đỏ, setColor(7) là màu trắng
    void setColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    // Hàm vẽ con rắn và quả trên màn hình
    void Ve(Point Qua) {
        for (int i = 0; i < DoDai; i++) {
            gotoxy(A[i].x, A[i].y);  // Di chuyển con trỏ đến vị trí từng điểm của rắn
            if (i == 0) {
                setColor(14);        // Đặt màu vàng cho đầu rắn
                char _220 = char(220);
                char _223 = char(223);
                if (directions[i] == LEFT || directions[i] == RIGHT || directions[i] == UP) {
                    cout << _220;  // Sử dụng ký tự có mã CCSID 437 là 220 cho thân rắn khi đi ngang
                }
                else {
                    cout << _223;  // Sử dụng ký tự có mã CCSID 437 là 223 cho thân rắn khi đi dọc
                }
            }
            else {
                setColor(10);        // Đặt màu xanh lá cây cho thân rắn
                char _220 = char(220);
                char _219 = char(219);
                if (directions[i] == LEFT || directions[i] == RIGHT) {
                    cout << _220;  // Sử dụng ký tự có mã CCSID 437 là 220 cho thân rắn khi đi ngang
                }
                else {
                    cout << _219;  // Sử dụng ký tự có mã CCSID 437 là 219 cho thân rắn khi đi dọc
                }
            }
        }
        setColor(12);               // Đặt màu đỏ cho quả
        gotoxy(Qua.x, Qua.y);       // Vẽ quả
        cout << char(220);
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
        {
            A[i] = A[i - 1];
            directions[i] = directions[i - 1]; // Cập nhật hướng của mỗi phần thân
        }

        // Di chuyển đầu rắn theo hướng chỉ định
        if (Huong == 0) A[0].x = A[0].x + 1, directions[0] = RIGHT; // Sang phải
        if (Huong == 1) A[0].y = A[0].y + 1, directions[0] = DOWN;; // Xuống
        if (Huong == 2) A[0].x = A[0].x - 1, directions[0] = LEFT;; // Sang trái
        if (Huong == 3) A[0].y = A[0].y - 1, directions[0] = UP;; // Lên

        // Chỉnh directions[1] cho hợp lý
        if (previousHead == UP) // Nếu mới chuyển hướng qua ngang khi đang đi lên
        {
            directions[1] = directions[0];
            previousHead = directions[0];
        }
        else if ((previousHead == LEFT || previousHead == RIGHT) && directions[0] == UP) // Nếu mới chuyển hướng lên trên
        {
            directions[1] = directions[0];
            previousHead = directions[0];
        }
        else // Chuyển hướng xuống/chuyển hướng qua ngang khi đang đi xuống => Không cần sửa đổi
            previousHead = directions[0];
    }

    // Kiểm tra va chạm với khung viền
    bool KiemTraVaChamKhung() {
        return (A[0].x <= MINX || A[0].x >= MAXX || A[0].y <= MINY || A[0].y >= MAXY);
    }

    // Kiểm tra va chạm với chính thân rắn
    bool KiemTraTuDam() {
        if (DoDai <= 4) return false; // Nếu rắn ngắn, không thể tự đâm
        // Kiểm tra xem đầu rắn có đụng vào thân không
        for (int i = 1; i < DoDai; i++) {
            if (A[0].x == A[i].x && A[0].y == A[i].y) {
                return true;
            }
        }
        return false;
    }

    // Hàm tăng độ dài của con rắn khi ăn mồi
    void AnMoi(int Huong) {
        this->DoDai++; // Tăng độ dài rắn
        this->DiChuyen(Huong); // Di chuyển tiếp theo hướng
    }

    // Lấy tọa độ x của đầu rắn
    int GetHeadX() {
        return A[0].x;
    }

    // Lấy tọa độ y của đầu rắn
    int GetHeadY() {
        return A[0].y;
    }
};

// Vẽ khung giới hạn cho trò chơi
void VeKhung() {
    for (int i = MINX; i <= MAXX; i++) {
        gotoxy(i, MINY); // Dòng trên
        cout << "-";
        gotoxy(i, MAXY); // Dòng dưới
        cout << "-";
    }
    for (int j = MINY; j <= MAXY; j++) {
        gotoxy(MINX, j); // Cột trái
        cout << "|";
        gotoxy(MAXX, j); // Cột phải
        cout << "|";
    }
}

// Hàm ẩn con trỏ
void HideCursor() {
    // Lấy thông tin của console hiện tại
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Ẩn con trỏ
    SetConsoleCursorInfo(hConsole, &cursorInfo); // Áp dụng thay đổi
}

// Hàm hiển thị con trỏ
void ShowCursor() {
    // Lấy thông tin của console hiện tại
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE;  // Hiển thị con trỏ
    SetConsoleCursorInfo(hConsole, &cursorInfo); // Áp dụng thay đổi
}

void showMenu() {
    cout << "===== MENU =====" << endl;
    cout << "1. Play" << endl;
    cout << "2. Exit" << endl;
    cout << "3. Choose snake's speed" << endl;
    cout << "Enter your choice: ";
}

void changeSnakeSpeed(int& speed) {
    cout << "\nNew speed (1-3, 1: Default, 2: Normal, 3: Fast): ";
    cin >> speed;
    if (speed < 1) speed = 300;
    if (speed > 3) speed = 100;
    switch (speed) {
    case 1: speed = 300; break;
    case 2: speed = 200; break;
    case 3: speed = 100; break;
    }
    cout << "Snake's speed changed succesfully!" << endl;
}

void playGame(int speed) {
    // Logic chơi game ở đây (sử dụng biến speed để điều khiển tốc độ của game)
    cout << "Start game with the speed of " << speed << "!" << endl;
    // Thực hiện các bước chơi game...
}

int main() {
    int choice;
    int snakeSpeed = 300; // Tốc độ mặc định của rắn

    while (true) {
        showMenu();
        choice = _getch() - '0'; // Đọc lựa chọn từ người chơi

        switch (choice) {
        case 1:
            playGame(snakeSpeed);
            break;
        case 2:
            cout << "Exiting..." << endl;
            return 0;
        case 3:
            changeSnakeSpeed(snakeSpeed);
            break;
        default:
            cout << "Your choice is not available!! Please choose again." << endl;
        }
        if (choice == 1)
            break;
    }

    // Biến lưu giá trị snakeSpeed đã chọn
    // snakeSpeed sẽ thay đổi khi chạy chương trình
    // Vì thế cần một biến lưu lại giá trị snakeSpeed ban đầu
    int initialSnakeSpeed = snakeSpeed;

_batDauLaiGame:
    HideCursor(); // Ẩn con trỏ khi chơi game
	snakeSpeed = initialSnakeSpeed;
    
    // Vẽ và di chuyển rắn

    CONRAN r; // Tạo đối tượng rắn
    int Huong = 0; // Hướng di chuyển mặc định
    char t;
    int score = 0; // Điểm số ban đầu
    Point Qua; // Tọa độ quả mồi
    srand((int)time(0)); // Thiết lập random
    Qua.x = rand() % (MAXX - MINX - 1) + MINX + 1;
    Qua.y = rand() % (MAXY - MINY - 1) + MINY + 1;

    system("cls");
    VeKhung(); // Vẽ khung trò chơi

    // đừng xóa nha ae
    /*
    while (true) {
         if (_kbhit()) {
             t = _getch();
             if (t == 'a') Huong = 2;
             if (t == 'w') Huong = 3;
             if (t == 'd') Huong = 0;
             if (t == 's') Huong = 1;
         }
     }
     */

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
     }
     */

     // sử dụng cách phím di chuyển bằng phím mũi tên và các phím W,A,S,D
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
            cout << "Press Y to play again, X to exit (not case-sensitive): ";
            char c;
            cin >> c;
            if (c == 'Y' || c == 'y')
                goto _batDauLaiGame; // Quay lại từ đầu hàm main để chơi lại game
            else if (c == 'X' || c == 'x')
                return 0;
            else {
                cout << "Key input is not in correct format. Forced Exiting...";
                return 1;
            }
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

        // Tăng tốc độ của rằn tùy theo điểm
        // Nếu snakeSpeed - score == 0 thì chương trình sẽ chạy không đúng
        if (snakeSpeed == score)
            snakeSpeed += 10;
        Sleep(snakeSpeed - score);
    }

    ShowCursor(); // Hiển thị lại con trỏ khi cần
}
