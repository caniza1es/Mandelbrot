#include <windows.h>
#include <iostream>
#include <complex>
#include <vector>
#include <chrono>
#include <thread>
#include <conio.h>

const int MAX_ITER = 1000;

int mandelbrot(std::complex<double> c) {
    std::complex<double> z = 0;
    int n = 0;
    while (abs(z) <= 2 && n < MAX_ITER) {
        z = z * z + c;
        n++;
    }
    return n;
}

WORD getColor(int iter) {
    if (iter == MAX_ITER) return 0;
    return iter % 16;
}

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwBytesWritten = 0;
    int WIDTH = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int HEIGHT = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    CHAR_INFO* charBuffer = new CHAR_INFO[WIDTH * HEIGHT];
    COORD bufferSize = { (short)WIDTH, (short)HEIGHT };
    COORD characterPos = { 0, 0 };
    SMALL_RECT writeArea = { 0, 0, (short)(WIDTH - 1), (short)(HEIGHT - 1) };

    double centerX = -0.7;
    double centerY = 0.0;
    double scale = 4.0;

    while (true) {
        // Check keyboard input
        if (_kbhit()) {
            int key = _getch();
            if (key == 'w' || key == 'W') {
                centerY -= 0.1 * scale;
            }
            else if (key == 's' || key == 'S') {
                centerY += 0.1 * scale;
            }
            else if (key == 'a' || key == 'A') {
                centerX -= 0.1 * scale;
            }
            else if (key == 'd' || key == 'D') {
                centerX += 0.1 * scale;
            }
            else if (key == '+' || key == '=') {
                scale *= 0.95;
            }
            else if (key == '-' || key == '_') {
                scale /= 0.95;
            }
        }

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                double real = centerX + (x - WIDTH / 2) * scale / WIDTH;
                double imag = centerY + (y - HEIGHT / 2) * scale / HEIGHT;
                std::complex<double> c(real, imag);

                int iter = mandelbrot(c);

                charBuffer[y * WIDTH + x].Char.AsciiChar = iter == MAX_ITER ? ' ' : '#';
                charBuffer[y * WIDTH + x].Attributes = getColor(iter);
            }
        }

        WriteConsoleOutput(hConsole, charBuffer, bufferSize, characterPos, &writeArea);
    }

    delete[] charBuffer;

    return 0;
}
