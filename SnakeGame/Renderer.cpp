#include "Renderer.h"

#include <windows.h>

#include <algorithm>
#include <string>

Renderer::Renderer(int width, int height) : width_(width), height_(height) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo{};
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(out, &cursorInfo);

    // Keep screen buffer size consistent with our frame size.
    COORD bufferSize;
    bufferSize.X = static_cast<SHORT>(width_ + 2);
    bufferSize.Y = static_cast<SHORT>(height_ + 4);
    SetConsoleScreenBufferSize(out, bufferSize);

    SMALL_RECT windowSize;
    windowSize.Left = 0;
    windowSize.Top = 0;
    windowSize.Right = static_cast<SHORT>(width_ + 1);
    windowSize.Bottom = static_cast<SHORT>(height_ + 3);
    SetConsoleWindowInfo(out, TRUE, &windowSize);
}

void Renderer::Draw(const Snake& snake, const Point& food, int score, bool paused, bool gameOver) const {
    const int frameW = width_ + 2;
    const int frameH = height_ + 4;
    std::vector<char> buffer(static_cast<size_t>(frameW * frameH), ' ');

    for (int x = 0; x < frameW; ++x) {
        DrawCell(buffer, x, 1, '#');
        DrawCell(buffer, x, height_ + 2, '#');
    }

    for (int y = 1; y <= height_ + 2; ++y) {
        DrawCell(buffer, 0, y, '#');
        DrawCell(buffer, width_ + 1, y, '#');
    }

    std::string title = "Score: " + std::to_string(score) + "  [P]Pause [K]Save [L]Load [Q]Quit";
    for (size_t i = 0; i < title.size() && static_cast<int>(i) < frameW; ++i) {
        DrawCell(buffer, static_cast<int>(i), 0, title[i]);
    }

    const auto& body = snake.GetBody();
    for (size_t i = 0; i < body.size(); ++i) {
        const char bodyChar = (i == 0) ? '@' : 'o';
        DrawCell(buffer, body[i].x, body[i].y + 1, bodyChar);
    }

    DrawCell(buffer, food.x, food.y + 1, '*');

    if (paused) {
        const std::string msg = "PAUSED";
        const int startX = std::max(1, (frameW - static_cast<int>(msg.size())) / 2);
        const int y = height_ / 2 + 1;
        for (size_t i = 0; i < msg.size(); ++i) {
            DrawCell(buffer, startX + static_cast<int>(i), y, msg[i]);
        }
    }

    if (gameOver) {
        const std::string msg = "GAME OVER - Press L to Load or Q to Quit";
        const int startX = std::max(1, (frameW - static_cast<int>(msg.size())) / 2);
        const int y = height_ / 2 + 2;
        for (size_t i = 0; i < msg.size() && startX + static_cast<int>(i) < frameW - 1; ++i) {
            DrawCell(buffer, startX + static_cast<int>(i), y, msg[i]);
        }
    }

    std::string bottom(frameW, ' ');
    for (int x = 0; x < frameW; ++x) {
        DrawCell(buffer, x, frameH - 1, '\n');
    }

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD origin{0, 0};
    DWORD written = 0;

    SetConsoleCursorPosition(out, origin);
    WriteConsoleOutputCharacterA(
        out,
        buffer.data(),
        static_cast<DWORD>(buffer.size()),
        origin,
        &written);
}

void Renderer::DrawCell(std::vector<char>& buffer, int x, int y, char c) const {
    const int frameW = width_ + 2;
    const int frameH = height_ + 4;
    if (x < 0 || x >= frameW || y < 0 || y >= frameH) {
        return;
    }

    buffer[static_cast<size_t>(y * frameW + x)] = c;
}
